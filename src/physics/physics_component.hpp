#pragma once 
#include "box2d_wrappers.hpp"
#include "user_data.hpp"
#include "physics_utilities.hpp"
#include <sol/sol.hpp>
#include <glm/glm.hpp>


namespace jadeite
{

struct PhysicsAttributes
{
	RigidBodyType eType{ RigidBodyType::STATIC };
	/* The density of the body, usually in kg/m^2. */
	float density{ 1.f }; 
	/* The Coulomb (dry) friction coefficient, usually in the range [0, 1]. */
	float friction{ 0.2f };
	/* The restitution (bounciness), usually in the range [0, 1]. */
	float restitution{ 0.2f };
	/* If a circle, the radius of the circle shape. */
	float radius{ 0.f };
	/* Scale the gravity applied to this body. */
	float gravityScale{ 1.f };
	/* Initial world position of the body. */
	glm::vec2 position{ 0.f };
	/* The amount to scale the body based on it's shape's size. */
	glm::vec2 scale{ 1.f };
	/* If a box, the width and the height of the box shape. */
	glm::vec2 boxSize{ 1.f };
	/* X/Y offset of the collider based on the TL position of the owner. */
	glm::vec2 offset{ 1.f };
	/* If set, the circle collider should be used in construction for the size. */
	bool bCircle{ false };
	/* If set, the box collider should be used in the construction of the size. */
	bool bBoxShape{ true };
	/* Should the body be prevented from rotating? */
	bool bFixedRotation{ true };
	/* If set, the sensor will generate overlap events; however, no collisions. */
	bool bSensor{ false };
	/* If set, treated as a high speed object with continuous collision detection. */
	bool bBullet{ false };
	/* Do you want to use filters with this body? */
	bool bUseFilters{ false };
	/* The filter category of the body itself. */
	uint16_t filterCategory{ 0 };
	/* The collision mask bits. The categories that this shape will collide with. */
	uint16_t filterMask{ 0 };
	/* A group index of zero has no effect. Non-zero groups follow specific rules. */
	int16_t groupIndex{ 0 };
	/* The user specified data for each body. */
	ObjectData objectData{};
};

class PhysicsComponent
{
public:
	PhysicsComponent();
	PhysicsComponent( const PhysicsAttributes& physicsAttr );
	~PhysicsComponent();
	
	void Init( PhysicsWorld pPhysicsWorld, int windowWidth, int windowHeight );
	const bool IsSensor() const;
	ObjectData CastRay(const b2Vec2& point1, const b2Vec2& point2 ) const;
	std::vector<ObjectData> BoxTrace( const b2Vec2& lowerBounds, const b2Vec2& upperBounds) const;
	ObjectData GetCurrentObjectData();
	
	void SetFilterCategory( uint16_t category );
	void SetFilterCategory( );
	void SetFilterMask( uint16_t mask );
	void SetFilterMask( );
	void SetGroupIndex( int16_t index );
	void SetGroupIndex( );
	
	bool UseFilters() const { return m_InitialAttribs.bUseFilters; }
	
	inline b2Body* GetBody() { return m_pRigidBody.get(); }
	inline UserData* GetUserData() { return m_pUserData.get(); }
	
	inline const PhysicsAttributes& GetAttributes() const { return m_InitialAttribs; }
	inline PhysicsAttributes& GetChangableAttributes() { return m_InitialAttribs; }
	
	static void CreateLuaBind( sol::state& lua, PhysicsWorld& pPhysicsWorld );
	
private:
	std::shared_ptr<b2Body> m_pRigidBody;
	std::shared_ptr<UserData> m_pUserData;
	PhysicsAttributes m_InitialAttribs;
};

} // jadeite
