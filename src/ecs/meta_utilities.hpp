#pragma once
#include <entt.hpp>
#include <sol/sol.hpp>
#include <iostream>

namespace jadeite
{

[[nodiscard]] entt::id_type GetIdType(const sol::table& comp);

template <typename ...Args>
inline auto InvokeMetaFunction(entt::meta_type meta, entt::id_type funcId, Args&& ...args)
{
	if (!meta)
	{
		std::cerr << "No entt::meta_type has been provided or is invalid.\n";
		assert(false && "No entt::meta_type has been provided or is invalid.\n");
		
		return entt::meta_any{};
	}
	
	if (auto metaFunction = meta.func(funcId); metaFunction)
	{
		return metaFunction.invoke({}, std::forward<Args>(args)...);
	}
	
	std::cerr << "No meta.func has been provided or is invalid.\n";
	assert(false && "No meta.func has been provided or is invalid.\n");
	return entt::meta_any{};
}

template <typename... Args>
inline auto InvokeMetaFunction(entt::id_type id, entt::id_type funcId, Args&& ...args)
{
	return InvokeMetaFunction(entt::resolve(id), funcId, std::forward<Args>(args) ...);
}

} // jadeite
