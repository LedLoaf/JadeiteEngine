#include "contact_listener.hpp"
#include "user_data.hpp"


namespace jadeite
{
	
void ContactListener::BeginContact( b2Contact* contact )
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

void ContactListener::PreSolve( b2Contact* contact, const b2Manifold* oldManifold )
{
	// TODO: Handle this later on.
}

void ContactListener::PostSolve( b2Contact* contact, const b2ContactImpulse* impulse )
{
	// TODO: Handle this later on.
}

void ContactListener::SetUserContacts(UserData* a, UserData* b)
{
	m_pUserDataA = a;
	m_pUserDataB = b;
}

} // jadeite
