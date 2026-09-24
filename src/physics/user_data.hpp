#pragma once
#include <cstdint>
#include <any>
#include <string>
#include <vector>
#include <entt.hpp>
#include <sol/sol.hpp>

namespace jadeite
{
/* UserData that keeps track of any userData and a type id */
struct UserData
{
	std::any userData{};
	std::uint32_t typeId{ 0 };
};

/* ObjectData used for tags, groups, flags for colliders, triggers, and isFriendly, and functions for pre-solve and post-solve */
struct ObjectData
{
	std::string tag{};
	std::string group{};
	bool bCollider{ false };
	bool bTrigger{ false };
	bool bIsFriendly{ false };
	std::uint32_t entityID{ entt::null };
	
	// Triggered in the contact listener and processed before Box2D collision to decide if it should happen (passthrough tiles)
	sol::protected_function onPreSolve{sol::lua_nil};
	// Triggered after the collision is processed (allowed for things like playing sounds, applying damage)
	sol::protected_function onPostSolve{sol::lua_nil};
	// Allows us to hold any lua or data that associates with the physics object.
	sol::object userData{sol::lua_nil};
	
	ObjectData() = default;
	ObjectData( const std::string& tag, const std::string& group, bool collider,
				bool trigger, bool isFriendly, std::uint32_t entity_id = entt::null);
				
	inline const std::vector<const ObjectData*>& GetContactEntities() const { return contactEntities; }
	friend bool operator==(const ObjectData& a, const ObjectData& b);

	[[nodiscard]] std::string toString() const;
	
private:
	bool AddContact(const ObjectData* objectData);
	bool RemoveContact(const ObjectData* objectData);
	inline void ClearContacts() { contactEntities.clear(); }
	
	friend class ContactListener;
	
private:
	std::vector<const ObjectData*> contactEntities;
};

/* Holding all the forces */
struct ContactImpulseInfo
{
	std::vector<float> normalImpulses;	// perpendicular to the surface
	std::vector<float> tangentImpulses;	// along the surface
	int32_t count;						// the number of contact points	
};

} // jadeite