#pragma once
#include <entt.hpp>
#include <sol/sol.hpp>

namespace jadeite
{

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
	
	inline bool IsValid(entt::entity entity) const { return m_pRegistry->valid(entity); }
	inline entt::registry& GetRegistry() { return *m_pRegistry; }
	
	inline entt::entity CreateEntity() { return m_pRegistry->create(); }
	inline void ClearRegistry() { m_pRegistry->clear(); }
	
	// Context Functions
	template <typename TContext>
	TContext AddToContext(TContext context);
	
	template <typename TContext>
	TContext& GetContext();
	
	template <typename TContext>
	TContext* TryGetContext();
	
	template <typename TContext>
	bool RemoveContext();
	
	template <typename TContext>
	bool HasContext();
	
	static void CreateLuaBind(sol::state& lua, Registry& registry);
	
	template <typename TComponent>
	static void RegisterMetaComponent();
	
private:
	std::shared_ptr<entt::registry> m_pRegistry;
};

template <typename TComponent>
entt::runtime_view& addComponentToView(Registry* pRegistry, entt::runtime_view& view);

template <typename TComponent>
auto excludeComponentFromView(Registry* pRegistry, entt::runtime_view* view);

} // jadeite

#include "registry.inl"
