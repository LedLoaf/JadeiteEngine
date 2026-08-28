#pragma once 
#include <box2d/box2d.h>


namespace jadeite
{

struct UserData;

class ContactListener : public b2ContactListener
{
public:
	void BeginContact( b2Contact* contact ) override;
	void EndContact( b2Contact* contact ) override;
	void PreSolve( b2Contact* contact, const b2Manifold* oldManifold ) override;
	void PostSolve( b2Contact* contact, const b2ContactImpulse* impulse ) override;
	
private:
	void SetUserContacts(UserData* a, UserData* b);
	
private:
	UserData* m_pUserDataA{ nullptr };
	UserData* m_pUserDataB{ nullptr };
};
	
} // jadeite
