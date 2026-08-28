#pragma once
#include <cstdint>
#include <any>
#include <string>
#include <vector>
#include <entt.hpp>
#include <sol/sol.hpp>

namespace jadeite
{
struct UserData
{
	std::any userData{};
	std::uint32_t typeId{ 0 };
};

struct ObjectData
{
	std::string tag{};
	std::string group{};
	bool bCollider{ false };
	bool bTrigger{ false };
	bool bIsFriendly{ false };
	std::uint32_t entityID{ entt::null };
	
	sol::protected_function onPreSolve{sol::lua_nil};
	sol::protected_function onPostSolve{sol::lua_nil};
	
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

} // jadeite