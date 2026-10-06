#pragma once
#include <entt.hpp>
#include <sol/sol.hpp>

namespace jadeite
{
	/* Registry Types */
	enum ERegistryType
	{
		Lua,
		Jadeite
	};
		
	class Registry
	{
	public:
		Registry();
		~Registry() = default;
		
		/* Checks if the given entity is still valid (not destroyed). */
		inline bool IsValid(entt::entity entity) const { return m_pRegistry->valid(entity); }
		/* Returns a reference to the underlying entt::registry. */
		inline entt::registry& GetRegistry() { return *m_pRegistry; }
		
		/* Creates and returns a new entity. */
		inline entt::entity CreateEntity() { return m_pRegistry->create(); }
		/* Destroys all entities and clears all components. */
		inline void ClearRegistry() { m_pRegistry->clear(); }
		
		// == Context Functions ==
		
		/* Stores a context object and returns it. */
		template <typename TContext>
		TContext AddToContext(TContext context);
		
		/* Returns a reference to the stored context. */
		template <typename TContext>
		TContext& GetContext();
		
		/* Returns a pointer to the stored context */
		template <typename TContext>
		TContext* TryGetContext();
		
		/* Removes the stored context; returns true if it existed. */
		template <typename TContext>
		bool RemoveContext();
		
		/* Returns true if a context of this type is stored. */
		template <typename TContext>
		bool HasContext();
		
		/* Binds the Registry class and all registered meta components to the given Lua state. */
		static void CreateLuaBind(sol::state& lua, Registry& registry);
		
		/* Registers a component type with Entt's meta system so that the external systems can interact with it by name at runtime without knowing the concrete type at compile time */
		template <typename TComponent>
		static void RegisterMetaComponent();
		
	private:
		std::shared_ptr<entt::registry> m_pRegistry;
	};

	/* Adds a TComponent filter to the given runtime view and returns it. */
	template <typename TComponent>
	entt::runtime_view& addComponentToView(Registry* pRegistry, entt::runtime_view& view);

	/* Excludes TComponent from the given runtime view and returns it. */
	template <typename TComponent>
	auto excludeComponentFromView(Registry* pRegistry, entt::runtime_view* view);

} // jadeite::Registry

#include "registry.inl"
