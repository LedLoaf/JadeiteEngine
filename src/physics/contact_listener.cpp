#include "contact_listener.hpp"
#include "user_data.hpp"


namespace jadeite
{	

	/* Called once when two fixtures begin to overlap. Maps the Box2D fixtures back to your game objects, then dispatches to the appropriate handler */
	void ContactListener::BeginContact( b2Contact* contact )
	{
		auto* fixtureA = contact->GetFixtureA();
		auto* fixtureB = contact->GetFixtureB();
		
		// Null-check: both fixtures and their userData pointers must exist.
		if (!fixtureA || !fixtureB || !fixtureA->GetUserData() || !fixtureB->GetUserData())
		{
			SetUserContacts( nullptr, nullptr );
			return;
		}
		
		UserData* a_data = reinterpret_cast<UserData*>( fixtureA->GetUserData() );
		UserData* b_data = reinterpret_cast<UserData*>( fixtureB->GetUserData() );
		
		constexpr auto expectedType = entt::type_hash<ObjectData>::value();
		
		// UserData->typeId must match the EnTT type hash for ObjectData
		if (!a_data || !b_data || a_data->typeId != expectedType || b_data->typeId != expectedType )
		{
			SetUserContacts( nullptr, nullptr );
			return;
		}	
		
		try
		{
			// Extracts the concrete ObjectData from the std::any payload.
			auto* a_any = std::any_cast<ObjectData>( &a_data->userData );
			auto* b_any = std::any_cast<ObjectData>( &b_data->userData );
			
			if ( !a_any || !b_any )
			{
				SetUserContacts(nullptr, nullptr);
				return;
			}
			
			a_any->AddContact( b_any );
			b_any->AddContact( a_any );
			
			SetUserContacts( a_data, b_data );
		}
		catch( const std::bad_any_cast& ex)
		{
			SetUserContacts( nullptr, nullptr );
		}
	}

	/* Called once when two fixtures stop overlapping. Used to clear state (grounded = false, stop looping sound, etc.) */
	void ContactListener::EndContact( b2Contact* contact )
	{
		auto* fixtureA = contact->GetFixtureA();
		auto* fixtureB = contact->GetFixtureB();
		
		if (!fixtureA || !fixtureB || !fixtureA->GetUserData() || !fixtureB->GetUserData())
		{
			SetUserContacts( nullptr, nullptr );
			return;
		}
		
		UserData* a_data = reinterpret_cast<UserData*>( fixtureA->GetUserData() );
		UserData* b_data = reinterpret_cast<UserData*>( fixtureB->GetUserData() );
		
		constexpr auto expectedType = entt::type_hash<ObjectData>::value();
		
		if (!a_data || !b_data || a_data->typeId != expectedType || b_data->typeId != expectedType )
		{
			SetUserContacts( nullptr, nullptr );
			return;
		}	
		
		try
		{
			auto* a_any = std::any_cast<ObjectData>( &a_data->userData );
			auto* b_any = std::any_cast<ObjectData>( &b_data->userData );
			
			if (!a_any && b_any )
			{
				b_any->ClearContacts();
				SetUserContacts( nullptr, nullptr );
				return;
			}
			
			if (a_any && !b_any )
			{
				a_any->ClearContacts();
				SetUserContacts( nullptr, nullptr );
				return;
			}
			
			a_any->RemoveContact( b_any );
			b_any->RemoveContact( a_any );
		}
		catch( const std::bad_any_cast& ex)
		{
			// ...
		}
		
		SetUserContacts( nullptr, nullptr );
	}

	/* 	Allows us to create special rules for certain interactions.
		Called every step BEFORE the solver computes impulses.
		Use to modify the manifold: disable specific contact points, override friction/restitution, 
		or make a body "pass through" by zeroing out all contact points. */
	void ContactListener::PreSolve( b2Contact* contact, const b2Manifold* oldManifold )
	{
		auto* pFixtureA = contact->GetFixtureA();
		auto* pFixtureB = contact->GetFixtureB();
		
		if(!pFixtureA || !pFixtureB)
		{
			return;
		}
		
		// Make sure we don't autom
		
		UserData* pUserDataA = reinterpret_cast<UserData*>(pFixtureA->GetUserData());
		UserData* pUserDataB = reinterpret_cast<UserData*>(pFixtureB->GetUserData());
		
		if(!pUserDataA || !pUserDataB)
		{
			return;
		}
		
		// Make sure we are casting to the correct types
		constexpr auto expectedType = entt::type_hash<ObjectData>::value();
		if(pUserDataA->typeId != expectedType || pUserDataB->typeId != expectedType)
		{
			return;
		}
		
		try
		{
			auto anyA = std::any_cast<ObjectData>(pUserDataA->userData);
			auto anyB = std::any_cast<ObjectData>(pUserDataB->userData);
			
			if(anyA.onPreSolve.valid())
			{
				auto result = anyA.onPreSolve(anyB);
				if(result.valid())
				{
					bool bAllowed = result.get<bool>();
					if(!bAllowed)
					{
						contact->SetEnabled(false);
						return;
					}
				}
			}
			
			if(anyB.onPreSolve.valid())
			{
				auto result = anyB.onPreSolve(anyA);
				if(result.valid())
				{
					bool bAllowed = result.get<bool>();
					if(!bAllowed)
					{
						contact->SetEnabled(false);
						return;
					}
				}
			}
		}
		catch(const std::bad_any_cast&)
		{
			// Eat this error
		}
	}

	/* 	Called every step AFTER the solver has applied impulses.
		The impulse data tells you HOW HARD the collision was — use it for sound volume, screen shake, particle burst, etc. */
	void ContactListener::PostSolve( b2Contact* contact, const b2ContactImpulse* impulse )
	{
		auto* pFixtureA = contact->GetFixtureA();
		auto* pFixtureB = contact->GetFixtureB();
		
		if(!pFixtureA || !pFixtureB)
		{
			return;
		}
		
		// Make sure we don't autom
		
		UserData* pUserDataA = reinterpret_cast<UserData*>(pFixtureA->GetUserData());
		UserData* pUserDataB = reinterpret_cast<UserData*>(pFixtureB->GetUserData());
		
		if(!pUserDataA || !pUserDataB)
		{
			return;
		}
		
		// Make sure we are casting to the correct types
		constexpr auto expectedType = entt::type_hash<ObjectData>::value();
		if(pUserDataA->typeId != expectedType || pUserDataB->typeId != expectedType)
		{
			return;
		}
		
		if(!impulse)
		{
			return;
		}
		
		// Handle impulses passed in
		ContactImpulseInfo contactInfo{};
		
		for(int i = 0; i < impulse->count; i++)
		{
			contactInfo.normalImpulses.push_back(impulse->normalImpulses[i]);
			contactInfo.tangentImpulses.push_back(impulse->tangentImpulses[i]);
		}
		
		contactInfo.count = impulse->count;
		
		try
		{
			auto anyA = std::any_cast<ObjectData>(pUserDataA->userData);
			auto anyB = std::any_cast<ObjectData>(pUserDataB->userData);
		
			if(anyA.onPostSolve.valid())
			{
				anyA.onPostSolve(anyB, contactInfo);
			}
			
			if(anyB.onPostSolve.valid())
			{
				anyB.onPostSolve(anyA, contactInfo);
			}
		}
		catch(const std::bad_any_cast&)
		{
			
		}
		
	}

	/* 	Stores the resolved game objects on the contact so downstream systems (audio, particles, UI) 
		can access them without re-casting from the fixture's userData every frame. */
	void ContactListener::SetUserContacts(UserData* a, UserData* b)
	{
		m_pUserDataA = a;
		m_pUserDataB = b;
	}
} // jadeite::ContactListener
