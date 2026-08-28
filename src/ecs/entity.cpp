#include "entity.hpp"
#include "meta_utilities.hpp"

namespace jadeite
{
Entity::Entity(Registry& registry)
	: m_Registry{ registry }
	, m_Entity{ registry.CreateEntity() }
{
}

Entity::Entity(Registry& registry, const entt::entity& entity)
	: m_Registry{ registry }
	, m_Entity{ entity }
{
}

Entity& Entity::operator=(const Entity& other)
{
	this->m_Entity = other.m_Entity;
	return *this;
}

void Entity::CreateLuaBind(sol::state& lua, Registry& registry)
{
	using namespace entt::literals;
	lua.new_usertype<Entity>(
		"Entity",
		sol::call_constructor,
		sol::factories(
			[](Registry& reg){ return Entity{ reg }; },
			[&registry]{ return Entity{ registry }; },
			[&registry](std::uint32_t id) { return Entity{ registry, static_cast<entt::entity>(id) }; }
		),
		"addComponent",
		[](Entity& entity, const sol::table& comp, sol::this_state s) -> sol::object 
		{
			if (!comp.valid())
			{
				return sol::lua_nil_t{};
			}
			
			const auto component = InvokeMetaFunction(GetIdType(comp), 
				"addComponent"_hs, entity, comp, s
			);
				
			return component ? component.cast<sol::reference>() : sol::lua_nil_t{};
		},
		"hasComponent", 
		[](Entity& entity, const sol::table& comp)
		{
			if (!comp.valid())
			{
				return false;
			}
			
			const auto hasComp = InvokeMetaFunction(GetIdType(comp), 
				"hasComponent"_hs, entity);
				
			return hasComp ? hasComp.cast<bool>() : false;
		},
		"getComponent",
		[](Entity& entity, const sol::table& comp, sol::this_state s) -> sol::object
		{
			if (!comp.valid())
			{
				return sol::lua_nil_t{};
			}
			
			const auto component = InvokeMetaFunction(GetIdType(comp), 
				"getComponent"_hs, entity, s
			);
				
			return component ? component.cast<sol::reference>() : sol::lua_nil_t{};
		},
		"removeComponent",
		[](Entity& entity, const sol::table& comp) -> sol::object
		{
			if (!comp.valid())
			{
				return sol::lua_nil_t{};
			}
			
			const auto removed = InvokeMetaFunction(GetIdType(comp), 
				"removeComponent"_hs, entity);
				
			return removed ? removed.cast<sol::reference>() : sol::lua_nil_t{};
		},
		"destroy", &Entity::Destroy,
		"id", [](Entity& entity) { return static_cast<uint32_t>(entity.GetEntity()); }
	);
}	

} // jadeite
