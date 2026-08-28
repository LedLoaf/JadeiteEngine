#pragma once
#include "registry.hpp"

namespace jadeite
{
class Entity
{
public:
	Entity(Registry& registry);
	Entity(Registry& registry, const entt::entity& entity);
	Entity& operator=(const Entity& other);
	~Entity() = default;
	
	inline std::uint32_t Destroy() { return m_Registry.GetRegistry().destroy(m_Entity); }
	inline entt::entity& GetEntity() { return m_Entity; }
	// TODO: Get the registry from the entity. Is this needed? 
	
	template <typename TComponent, typename ...Args> 
	TComponent& AddComponent(Args&& ...args);
	
	template <typename TComponent, typename ...Args> 
	TComponent& ReplaceComponent(Args&& ...args);
	
	template <typename TComponent> 
	TComponent& GetComponent();
	
	template <typename TComponent> 
	TComponent* TryGetComponent();
	
	template <typename TComponent> 
	bool HasComponent();
	
	template <typename TComponent> 
	auto RemoveComponent();
	
	static void CreateLuaBind(sol::state& lua, Registry& registry);
	
	template <typename TComponent>
	static void RegisterMetaComponent();
	
private:
	Registry& m_Registry;
	entt::entity m_Entity;
};

template <typename TComponent>
auto addComponent(Entity& entity, const sol::table& comp, sol::this_state s);

template <typename TComponent>
auto getComponent(Entity& entity, sol::this_state s);

template <typename TComponent>
bool hasComponent(Entity& entity);

template <typename TComponent>
auto removeComponent(Entity& entity);

} // jadeite

#include "entity.inl"
