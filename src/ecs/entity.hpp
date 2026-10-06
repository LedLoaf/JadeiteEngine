#pragma once
#include "registry.hpp"

namespace jadeite
{
	/* Entity template member functions (thin wrappers over EnTT registry) */
	class Entity
	{
	public:
		Entity(Registry& registry);
		Entity(Registry& registry, const entt::entity& entity);
		Entity& operator=(const Entity& other);
		~Entity() = default;
		
		/* Destroys the underlying entity (removes all its components and invalidates the handle). */
		inline std::uint32_t Destroy() { return m_Registry.GetRegistry().destroy(m_Entity); }
		
		/* Returns a reference to the raw EnTT entity handle. */
		inline entt::entity& GetEntity() { return m_Entity; }
	
		/* Add a new component to this entity by emplacing it to the registry. */
		template <typename TComponent, typename ...Args> 
		TComponent& AddComponent(Args&& ...args);
		
		/* Replaces the component if it already exists, otherwise emplaces a new one.
		This makes it safe to call regardless of whether the entity already has TComponent. */
		template <typename TComponent, typename ...Args> 
		TComponent& ReplaceComponent(Args&& ...args);
		
		/* Returns a reference to the component. Use TryGetComponent for a safe check. */
		template <typename TComponent> 
		TComponent& GetComponent();
		
		/* Returns a pointer to the component, or nullptr if the entity doesn't have it. */
		template <typename TComponent> 
		TComponent* TryGetComponent();
		
		/* Returns true if the entity currently has TComponent. */
		template <typename TComponent> 
		bool HasComponent();
		
		/* Removes TComponent from the entity. Returns the number of components removed (0 or 1). */
		template <typename TComponent> 
		auto RemoveComponent();
		
		/* Binds the Entity class and all registered meta components to the Lua state. */
		static void CreateLuaBind(sol::state& lua, Registry& registry);
		
		/* Registers a component type with Entt's meta system so that the external systems can interact with it by name at runtime without knowing the concrete type at compile time */
		template <typename TComponent>
		static void RegisterMetaComponent();
		
	private:
		Registry& m_Registry;
		entt::entity m_Entity;
	};
	
	/* ================================================================================= */
	/* == These are the functions registered via entt::meta in RegisterMetaComponent. == */
	/* == They bridge Lua calls into the C++ Entity API. Lua (sol2) binding wrappers. == */
	/* ================================================================================= */	
	
	/* Add a component to an entity and if a sol::table is provided, deserializes it into TComponent; Returns a reference to the component (kept alive during the lua state) */
	template <typename TComponent>
	auto addComponent(Entity& entity, const sol::table& comp, sol::this_state s);
	
	/* Returns a reference to the component, or nil if the entity doesn't have it. */
	template <typename TComponent>
	auto getComponent(Entity& entity, sol::this_state s);
	
	/* Returns true or false if an entity has a component */
	template <typename TComponent>
	bool hasComponent(Entity& entity);
	
	/* Removes and returns the count of components removed. */
	template <typename TComponent>
	auto removeComponent(Entity& entity);
	
	/* ================================================================================= */
	
	
} // jadeite::Entity

#include "entity.inl"
