#include "entity.hpp"

/* Entity Template Functions */
namespace jadeite
{
	// ========================================================
	template <typename TComponent, typename ...Args> 
	TComponent& Entity::AddComponent(Args&& ...args)
	{
		auto& registry = m_Registry.GetRegistry();
		return registry.emplace<TComponent>(m_Entity, std::forward<Args>(args)...);
	}

	// ========================================================
	template <typename TComponent, typename ...Args> 
	TComponent& Entity::ReplaceComponent(Args&& ...args)
	{
		auto& registry = m_Registry.GetRegistry();
		if (registry.all_of<TComponent>(m_Entity))
			return registry.replace<TComponent>(m_Entity, std::forward<Args>(args)...);
		else
			return registry.emplace<TComponent>(m_Entity, std::forward<Args>(args)...);
	}

	// ========================================================
	template <typename TComponent> 
	TComponent& Entity::GetComponent()
	{
		auto& registry = m_Registry.GetRegistry();
		return registry.get<TComponent>(m_Entity);
	}

	// ========================================================
	template <typename TComponent> 
	TComponent* Entity::TryGetComponent()
	{
		auto& registry = m_Registry.GetRegistry();
		return registry.try_get<TComponent>(m_Entity);
	}

	// ========================================================
	template <typename TComponent> 
	bool Entity::HasComponent()
	{
		auto& registry = m_Registry.GetRegistry();
		return registry.all_of<TComponent>(m_Entity);
	}

	// ========================================================
	template <typename TComponent> 
	auto Entity::RemoveComponent()
	{
		auto& registry = m_Registry.GetRegistry();
		return registry.remove<TComponent>(m_Entity);
	}

	// ========================================================
	template <typename TComponent>
	auto addComponent(Entity& entity, const sol::table& comp, sol::this_state s)
	{
		// If a sol::table is provided, deserializes it into TComponent; otherwise constructs a default TComponent.
		auto& component = entity.AddComponent<TComponent>(
			comp.valid() ? std::move(comp.as<TComponent&&>()) : TComponent{}
		);
		
		return sol::make_reference(s, std::ref(component));
	}

	// ========================================================
	template <typename TComponent>
	auto getComponent(Entity& entity, sol::this_state s)
	{
		auto* pComp = entity.TryGetComponent<TComponent>();
		return pComp ? sol::make_reference(s, std::ref(*pComp)) : sol::lua_nil_t{};
	}

	// ========================================================
	template <typename TComponent>
	bool hasComponent(Entity& entity)
	{
		return entity.HasComponent<TComponent>();
	}

	// ========================================================
	template <typename TComponent>
	auto removeComponent(Entity& entity)
	{
		return entity.RemoveComponent<TComponent>();
	}

	/* EnTT's registry is type-erased at runtime — you can look up components by their numeric ID without knowing the C++ type. */
	template <typename TComponent>
	void Entity::RegisterMetaComponent()
	{
		using namespace entt::literals;
		// Creates a reflection factory for the component type.
		// Assigns the type's numeric ID (an FNV-1a hash of the type name) so it matches EnTT's internal runtime type identification.
		// Attaches a free function as a callable "meta function" accessible by the string "addComponent" at runtime. 
		// The _hs suffix creates a hashed_string (EnTT's compact string identifier).
		entt::meta_factory<TComponent>()
			.type(entt::type_hash<TComponent>::value())								
			.template func<&addComponent<TComponent>>("addComponent"_hs)
			.template func<&getComponent<TComponent>>("getComponent"_hs)
			.template func<&hasComponent<TComponent>>("hasComponent"_hs)
			.template func<&removeComponent<TComponent>>("removeComponent"_hs);
	}
} // jadeite
