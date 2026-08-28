#include "physics_component.hpp"
#include "raycast_callback.hpp"
#include "boxtrace_callback.hpp"
#include "utilities/core_data.hpp"
#include <iostream>
#include <cassert>
	
namespace jadeite
{
PhysicsComponent::PhysicsComponent()
	: PhysicsComponent(PhysicsAttributes{})
{
}

PhysicsComponent::PhysicsComponent( const PhysicsAttributes& physicsAttr )
	: m_pRigidBody{ nullptr }
	, m_pUserData{ nullptr }
	, m_InitialAttribs{ physicsAttr }
{
	
}

PhysicsComponent::~PhysicsComponent() = default;
	
void PhysicsComponent::Init( PhysicsWorld pPhysicsWorld, int windowWidth, int windowHeight )
{
	if ( !pPhysicsWorld )
	{
		std::cerr << "Failed to intialize the physics component - Physics world is invalid\n";
		return;
	}
	
	bool bCircle{ m_InitialAttribs.bCircle };
	
	// Create the body def
	b2BodyDef bodyDef{};
	bodyDef.type = static_cast<b2BodyType>(m_InitialAttribs.eType);
	float PIXELS_TO_METERS = CORE_DATA().PixelsToMeters();
	
	// Set the initial position of the body 
	bodyDef.position.Set(
		( m_InitialAttribs.position.x + m_InitialAttribs.offset.x - ( windowWidth * 0.5f ) + 
		( ( bCircle ? m_InitialAttribs.radius : m_InitialAttribs.boxSize.x) * m_InitialAttribs.scale.y) * 0.5f ) * PIXELS_TO_METERS,
		( m_InitialAttribs.position.y + m_InitialAttribs.offset.y - ( windowHeight * 0.5f ) + 
		( ( bCircle ? m_InitialAttribs.radius : m_InitialAttribs.boxSize.y) * m_InitialAttribs.scale.y) * 0.5f ) * PIXELS_TO_METERS
	);
	
	bodyDef.gravityScale = m_InitialAttribs.gravityScale;
	bodyDef.fixedRotation = m_InitialAttribs.bFixedRotation;
	
	m_pRigidBody = MakeSharedBody( pPhysicsWorld->CreateBody( &bodyDef ) );
	
	if ( !m_pRigidBody )
	{
		std::cerr << "Failed to create rigid body\n";
		return;
	}
	
	// Create the shape
	b2PolygonShape polyShape;
	b2CircleShape circleShape;
	
	if ( bCircle )
	{
		circleShape.m_radius = PIXELS_TO_METERS * m_InitialAttribs.radius * m_InitialAttribs.scale.x;
	}
	else if (m_InitialAttribs.bBoxShape)
	{
		polyShape.SetAsBox( PIXELS_TO_METERS * m_InitialAttribs.boxSize.x * m_InitialAttribs.scale.x * 0.5f,
							PIXELS_TO_METERS * m_InitialAttribs.boxSize.y * m_InitialAttribs.scale.y * 0.5f);
	}
	else
	{
		// TODO: Add the ability to create various convex polygon shapes based on the number
		// of vertices and their position.
		// Currently hardcoded to a 4 vertice shape, square, rect, diamond, etc.
		auto halfWidth = PIXELS_TO_METERS * m_InitialAttribs.boxSize.x * m_InitialAttribs.scale.x * 0.5f;
		auto halfHeight = PIXELS_TO_METERS * m_InitialAttribs.boxSize.y * m_InitialAttribs.scale.y * 0.5f;

		b2Vec2 vertices[ 4 ];

		vertices[ 0 ].Set( 0.f, 0.f );
		vertices[ 1 ].Set( halfWidth, -halfHeight );
		vertices[ 2 ].Set( -halfWidth, -halfHeight );
		vertices[ 3 ].Set( 0.f, -halfHeight * 2.f );

		polyShape.Set( vertices, 4 );
	}
	
	// Create the user data 
	m_pUserData = std::make_shared<UserData>();
	m_pUserData->userData = m_InitialAttribs.objectData;
	m_pUserData->typeId = entt::type_hash<ObjectData>::value();
	
	// Create the fixture def 
	b2FixtureDef fixtureDef{};
	if ( bCircle )
		fixtureDef.shape = &circleShape;
	else 
		fixtureDef.shape = &polyShape;
	
	fixtureDef.density = m_InitialAttribs.density;
	fixtureDef.friction = m_InitialAttribs.friction;
	fixtureDef.restitution = m_InitialAttribs.restitution;
	fixtureDef.isSensor = m_InitialAttribs.bSensor;
	fixtureDef.userData = reinterpret_cast<void*>( m_pUserData.get() );
	
	auto pFixture = m_pRigidBody->CreateFixture( &fixtureDef );
	assert(pFixture && "fixture was not created successfully.");
	
	if (!pFixture)
	{
		std::cerr << "Failed to create the rigid body fixture\n";
	}
}

const bool PhysicsComponent::IsSensor() const
{
	if ( !m_pRigidBody )
		return false;
	
	return m_pRigidBody->GetFixtureList()->IsSensor();
}

ObjectData PhysicsComponent::CastRay(const b2Vec2& point1, const b2Vec2& point2 ) const
{
	if ( !m_pRigidBody )
		return {};
	
	// Get the world 
	auto* pWorld = m_pRigidBody->GetWorld();
	if ( !pWorld )
		return {};
	
	auto& coreData = CORE_DATA();
	const float P2M = coreData.PixelsToMeters();
	
	const float scaledHalfWidth = coreData.ScaledWidth() * 0.5f;
	const float scaledHalfHeight = coreData.ScaledHeight() * 0.5f;
	
	auto ax = ( point1.x * P2M ) - scaledHalfWidth;
	auto ay = ( point1.y * P2M ) - scaledHalfHeight;
	
	auto bx = ( point2.x * P2M ) - scaledHalfWidth;
	auto by = ( point2.y * P2M ) - scaledHalfHeight;
	
	RayCastCallback callback{};
	
	pWorld->RayCast(&callback, b2Vec2{ ax, ay }, b2Vec2{ bx, by } );
	
	if ( callback.IsHit() )
	{
		if (auto* userData = callback.HitFixture()->GetUserData())
		{
			if (UserData* pData = reinterpret_cast<UserData*>( userData ) )
			{
				try
				{
					auto objectData = std::any_cast<ObjectData>( pData->userData );
					return objectData;
				}
				catch( const std::bad_any_cast& ex )
				{
					std::cerr << "Failed to cast to object data from raycast. Error: " 
								<< ex.what() << "\n";
				}
			}
		}
	}
	
	return {};
}

std::vector<ObjectData> PhysicsComponent::BoxTrace( const b2Vec2& lowerBounds, const b2Vec2& upperBounds) const
{
	std::vector<ObjectData> objectDataVec;
	
	if ( !m_pRigidBody )
		return objectDataVec;
	
	// Get the world 
	auto* pWorld = m_pRigidBody->GetWorld();
	if ( !pWorld )
		return objectDataVec;
	
	auto& coreData = CORE_DATA();
	const float P2M = coreData.PixelsToMeters();
	
	const float scaledHalfWidth = coreData.ScaledWidth() * 0.5f;
	const float scaledHalfHeight = coreData.ScaledHeight() * 0.5f;
	
	b2AABB aabb{};
	aabb.lowerBound = 
		b2Vec2{ 
			( lowerBounds.x * P2M ) - scaledHalfWidth, ( lowerBounds.y * P2M ) - scaledHalfHeight 
		};
	
	aabb.upperBound = 
		b2Vec2{ 
			( upperBounds.x * P2M ) - scaledHalfWidth, ( upperBounds.y * P2M ) - scaledHalfHeight 
		};
	
	BoxTraceCallback callback{};
	
	pWorld->QueryAABB( &callback, aabb );
	
	const auto& hitBodies = callback.GetBodies();
	if ( hitBodies.empty() )
		return objectDataVec;
	
	
	for ( const auto* pBody : hitBodies )
	{
		if (auto* userData = pBody->GetFixtureList()->GetUserData())
		{
			if (UserData* pData = reinterpret_cast<UserData*>( userData ) )
			{
				try
				{
					auto objectData = std::any_cast<ObjectData>( pData->userData );
					objectDataVec.emplace_back( objectData );
				}
				catch( const std::bad_any_cast& ex )
				{
					std::cerr << "Failed to cast to object data from boxtrace. Error: " 
								<< ex.what() << "\n";
				}
			}
		}
	}
	
	return objectDataVec;
}

ObjectData PhysicsComponent::GetCurrentObjectData()
{
	if ( !m_pRigidBody )
	{
		return {};
	}
	
	auto* userData = m_pRigidBody->GetFixtureList()->GetUserData();
	UserData* pData = reinterpret_cast<UserData*>( userData );
	
	try
	{
		auto objectData = std::any_cast<ObjectData>( pData->userData );
		return objectData;
	}
	catch ( const std::bad_any_cast& ex )
	{
		std::cerr << "Failed to cast object data - " << ex.what() << "\n";
	}
	
	return {};
}

void PhysicsComponent::SetFilterCategory( uint16_t category )
{
	if ( !m_pRigidBody )
	{
		m_InitialAttribs.filterCategory = category;
		return;
	}

	auto pFixtureList = m_pRigidBody->GetFixtureList();
	if ( !pFixtureList )
	{
		return;
	}

	b2Filter copyFilter{ pFixtureList->GetFilterData() };
	copyFilter.categoryBits = category;
	pFixtureList->SetFilterData( copyFilter );
	m_InitialAttribs.filterCategory = category;
}

void PhysicsComponent::SetFilterCategory( )
{
	SetFilterCategory( m_InitialAttribs.filterCategory );
}

void PhysicsComponent::SetFilterMask( uint16_t mask )
{
	if ( !m_pRigidBody )
	{
		m_InitialAttribs.filterMask = mask;
		return;
	}

	auto pFixtureList = m_pRigidBody->GetFixtureList();
	if ( !pFixtureList )
	{
		return;
	}

	b2Filter copyFilter{ pFixtureList->GetFilterData() };
	copyFilter.maskBits = mask;
	pFixtureList->SetFilterData( copyFilter );
	m_InitialAttribs.filterMask = mask;
}

void PhysicsComponent::SetFilterMask( )
{
	SetFilterMask( m_InitialAttribs.filterMask );
}

void PhysicsComponent::SetGroupIndex( int16_t index )
{
	if ( !m_pRigidBody )
	{
		m_InitialAttribs.groupIndex = index;
		return;
	}

	auto pFixtureList = m_pRigidBody->GetFixtureList();
	if ( !pFixtureList )
	{
		return;
	}

	b2Filter copyFilter{ pFixtureList->GetFilterData() };
	copyFilter.groupIndex = index;
	pFixtureList->SetFilterData( copyFilter );
	m_InitialAttribs.groupIndex = index;
}

void PhysicsComponent::SetGroupIndex( )
{
	SetGroupIndex( m_InitialAttribs.groupIndex );
}
	
void PhysicsComponent::CreateLuaBind( sol::state& lua, PhysicsWorld& pPhysicsWorld )\
{
	lua.new_usertype<ObjectData>(
		"ObjectData",
		"type_id", &entt::type_hash<ObjectData>::value,
		sol::call_constructor,
		sol::factories(
			[](const std::string& tag, const std::string& group, bool bCollider, 
				bool bTrigger, bool bFriendly, std::uint32_t entityID)
			{
				return ObjectData{
					tag,
					group,
					bCollider,
					bTrigger,
					bFriendly,
					entityID
				};
			},
			[]( const sol::table& objectData )
			{
				return ObjectData {
					objectData[ "tag" ].get_or(std::string{ } ),
					objectData[ "group" ].get_or(std::string{ } ),
					objectData[ "bCollider" ].get_or( false ),
					objectData[ "bTrigger" ].get_or( false ),
					objectData[ "bIsFriendly" ].get_or( false ),
					objectData[ "entityID" ].get_or( static_cast<std::uint32_t>(entt::null))
				};
			}
		),
		"tag", &ObjectData::tag,
		"group", &ObjectData::group,
		"bCollider", &ObjectData::bCollider,
		"bTrigger", &ObjectData::bTrigger,
		"bIsFriendly", &ObjectData::bIsFriendly,
		"entityID", &ObjectData::entityID,
		"userData", &ObjectData::userData,
		"contactEntities", 
			sol::readonly_property(
				[](ObjectData& objData) { return objData.GetContactEntities(); }
			),
		"setOnPreSolve",[](ObjectData& obj, sol::protected_function func)
		{
			obj.onPreSolve = func;
		},
		"setOnPostSolve",[](ObjectData& obj, sol::protected_function func)
		{
			obj.onPostSolve = func;
		}
	);
	
	lua.new_enum<RigidBodyType>(
		"BodyType",
		{
			{"Static", RigidBodyType::STATIC },
			{"Kinematic", RigidBodyType::KINEMATIC },
			{"Dynamic", RigidBodyType::DYNAMIC },
		}
	);
	
	lua.new_usertype<PhysicsAttributes>(
		"PhysicsAttributes",
		sol::call_constructor,
		sol::factories(
			[]{ return PhysicsAttributes{}; },
			[](const sol::table& physAttr) 
			{
				return PhysicsAttributes{
					.eType = physAttr["eType"].get_or( RigidBodyType::STATIC ),
					.density = physAttr["density"].get_or( 100.f ),
					.friction = physAttr["friction"].get_or( 0.2f ),
					.restitution = physAttr["restitution"].get_or( 0.2f ),
					.radius = physAttr["radius"].get_or( 0.f ),
					.gravityScale = physAttr["gravityScale"].get_or( 1.f ),
					.position = glm::vec2{ 
						physAttr["position"]["x"].get_or( 0.f ),
						physAttr["position"]["y"].get_or( 0.f ) 
					},
					.scale = glm::vec2{ 
						physAttr["scale"]["x"].get_or( 1.f ),
						physAttr["scale"]["y"].get_or( 1.f ) 
					},
					.boxSize = glm::vec2{ 
						physAttr["boxSize"]["x"].get_or( 0.f ),
						physAttr["boxSize"]["y"].get_or( 0.f ) 
					},
					.offset = glm::vec2{ 
						physAttr["offset"]["x"].get_or( 0.f ),
						physAttr["offset"]["y"].get_or( 0.f ) 
					},
					.bCircle = physAttr["bCircle"].get_or( false ),
					.bBoxShape = physAttr["bBoxShape"].get_or( true ),
					.bFixedRotation = physAttr["bFixedRotation"].get_or( true ),
					.bSensor = physAttr["bIsSensor"].get_or( false),
					.filterCategory = physAttr["filterCategory"].get_or(static_cast<uint16_t>(0)),
					.filterMask = physAttr["filterMask"].get_or(static_cast<uint16_t>(0)),
					.objectData = ObjectData{
						physAttr["objectData"]["tag"].get_or(std::string{}),
						physAttr["objectData"]["group"].get_or(std::string{}),
						physAttr["objectData"]["bCollder"].get_or( false ),
						physAttr["objectData"]["bTrigger"].get_or( false ),
						physAttr["objectData"]["bIsFriendly"].get_or( false ),
						physAttr["objectData"]["entityID"].get_or( static_cast<std::uint32_t>(0) )
					}						
				};
			}
		),
		"eType", &PhysicsAttributes::eType,
		"density", &PhysicsAttributes::density,
		"friction", &PhysicsAttributes::friction,
		"restitution", &PhysicsAttributes::restitution,
		"radius", &PhysicsAttributes::radius,
		"gravityScale", &PhysicsAttributes::gravityScale,
		"position", &PhysicsAttributes::position,
		"scale", &PhysicsAttributes::scale,
		"boxSize", &PhysicsAttributes::boxSize,
		"offset", &PhysicsAttributes::offset,
		"bCircle", &PhysicsAttributes::bCircle,
		"bBoxShape", &PhysicsAttributes::bBoxShape,
		"bFixedRotation", &PhysicsAttributes::bFixedRotation,
		"bIsSensor", &PhysicsAttributes::bSensor,
		"objectData", &PhysicsAttributes::objectData
	);
	
	if ( !pPhysicsWorld )
	{
		std::cerr << "Physics world is not valid. Failed to bind physics component.\n";
		return;
	}
	
	lua.new_usertype<PhysicsComponent>(
		"PhysicsComp",
		"type_id", &entt::type_hash<PhysicsComponent>::value,
		sol::call_constructor,
		sol::factories(
			[&pPhysicsWorld](const PhysicsAttributes& attr)
			{
				PhysicsComponent pc{ attr };
				pc.Init( pPhysicsWorld, 640, 480 );
				return pc;
			}
		),
		"linearImpulse", []( PhysicsComponent& pc, const glm::vec2& impulse )
		{
			auto body = pc.GetBody();
			if (!body)
			{
				return;
			}
			
			body->ApplyLinearImpulse( b2Vec2{ impulse.x, impulse.y }, body->GetPosition(), true );
		},
		"angularImpulse", []( PhysicsComponent& pc, float impulse )
		{
			auto body = pc.GetBody();
			if (!body)
			{
				return;
			}
			
			body->ApplyAngularImpulse( impulse, true );
		},
		"setLinearVelocity", []( PhysicsComponent& pc, const glm::vec2& velocity )
		{
			auto body = pc.GetBody();
			if (!body)
			{
				return;
			}
			
			body->SetLinearVelocity( b2Vec2{ velocity.x, velocity.y } );
		},
		"getLinearVelocity", []( PhysicsComponent& pc )
		{
			auto body = pc.GetBody();
			if (!body)
			{
				return glm::vec2{ 0.f };
			}
			
			const auto& linearVelocity = body->GetLinearVelocity( );
			return glm::vec2{ linearVelocity.x, linearVelocity.y };
		},
		"setAngularVelocity", []( PhysicsComponent& pc, float angularVelocity )
		{
			auto body = pc.GetBody();
			if (!body)
			{
				return;
			}
			
			body->SetAngularVelocity( angularVelocity );
		},
		"getAngularVelocity", []( PhysicsComponent& pc )
		{
			auto body = pc.GetBody();
			if (!body)
			{
				return 0.f;
			}
			
			return body->GetAngularVelocity();
		},
		"setGravityScale", []( PhysicsComponent& pc, float gravityScale )
		{
			auto body = pc.GetBody();
			if (!body)
			{
				return;
			}
			
			body->SetGravityScale( gravityScale );			
		},
		"getGravityScale", []( PhysicsComponent& pc )
		{
			auto body = pc.GetBody();
			if (!body)
			{
				return 0.f;
			}
			
			return body->GetGravityScale( );			
		},
		"setTransform", []( PhysicsComponent& pc, const glm::vec2& position )
		{
			auto body = pc.GetBody();
			if (!body)
			{
				return;
			}
			
			auto& coreData = CORE_DATA();
			const float p2m = coreData.PixelsToMeters();
			
			const float scaledHalfWidth = coreData.ScaledWidth() * 0.5f;
			const float scaledHalfHeight = coreData.ScaledHeight() * 0.5f;
			
			auto bx = ( position.x * p2m ) - scaledHalfWidth;
			auto by = ( position.y * p2m ) - scaledHalfHeight;
			
			body->SetTransform( b2Vec2{ bx, by }, 0.f );			
		},
		"getTransform", []( PhysicsComponent& pc )
		{
			// TODO: Get Transform from body
		},
		"setBodyType", []( PhysicsComponent& pc, RigidBodyType eType )
		{
			auto body = pc.GetBody();
			if (!body)
			{
				return;
			}
			
			b2BodyType bodyType = b2_dynamicBody;
			
			switch( eType )
			{
			case RigidBodyType::STATIC: bodyType = b2_staticBody; break;
			case RigidBodyType::DYNAMIC: bodyType = b2_dynamicBody; break;
			case RigidBodyType::KINEMATIC: bodyType = b2_kinematicBody; break;
			default: assert(false && "body type is invalid");
			}
			
			body->SetType( bodyType );
		},
		"setBullet", []( PhysicsComponent& pc, bool bBullet )
		{
			auto body = pc.GetBody();
			if (!body)
			{
				return;
			}
			
			body->SetBullet( bBullet );
		},
		"isBullet", []( PhysicsComponent& pc )
		{
			auto body = pc.GetBody();
			if (!body)
			{
				return false;
			}
			
			return body->IsBullet( );
		},
		// TODO: Handle filter categories
		"castRay", []( PhysicsComponent& pc, const glm::vec2& p1,  const glm::vec2& p2, sol::this_state s )
		{
			auto objectData = pc.CastRay( b2Vec2{ p1.x, p1.y }, b2Vec2{ p2.x, p2.y } );
			return objectData.entityID == entt::null ? sol::lua_nil_t{} : sol::make_object( s, objectData );
		},
		"boxTrace", []( PhysicsComponent& pc, const glm::vec2& lowerBounds,  const glm::vec2& upperBounds, 
					sol::this_state s )
		{
			auto objectDataVec = pc.BoxTrace( b2Vec2{ lowerBounds.x, lowerBounds.y }, 
										b2Vec2{ upperBounds.x, upperBounds.y } );
										
			return objectDataVec.empty() ? sol::lua_nil_t{} : sol::make_object( s, objectDataVec );
		},
		"objectData", []( PhysicsComponent& pc ) { return pc.GetCurrentObjectData(); }
	);
}

} // jadeite
