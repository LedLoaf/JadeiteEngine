#include "registry.hpp"

namespace jadeite
{
template <typename TContext>
TContext Registry::AddToContext(TContext context)
{
	return m_pRegistry->ctx().emplace<TContext>(context);
}

template <typename TContext>
TContext& Registry::GetContext()
{
	return m_pRegistry->ctx().get<TContext>();
}

template <typename TContext>
TContext* Registry::TryGetContext()
{
	return m_pRegistry->ctx().find<TContext>();
}

template <typename TContext>
bool Registry::RemoveContext()
{
	return m_pRegistry->ctx().erase<TContext>();
}

template <typename TContext>
bool Registry::HasContext()
{
	return m_pRegistry->ctx().contains<TContext>();
}

template <typename TComponent>
entt::runtime_view& addComponentToView(Registry* pRegistry, entt::runtime_view& view)
{
	return view.iterate(pRegistry->GetRegistry().storage<TComponent>());
}

template <typename TComponent>
auto excludeComponentFromView(Registry* pRegistry, entt::runtime_view* view)
{
	view->exclude( pRegistry->GetRegistry().storage<TComponent>() );
}

template <typename TComponent>
void Registry::RegisterMetaComponent()
{
	using namespace entt::literals;
	entt::meta_factory<TComponent>()
		.type(entt::type_hash<TComponent>::value())
		.template func<&addComponentToView<TComponent>>("addComponentToView"_hs)
		.template func<&excludeComponentFromView<TComponent>>("excludeComponentFromView"_hs);
}

} // jadeite
