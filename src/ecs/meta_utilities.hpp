#pragma once
#include <entt.hpp>
#include <sol/sol.hpp>
#include <iostream>
#include "utilities/logger.hpp"

namespace jadeite
{
	/* Extracts the component type ID from a Lua table (typically from a "type" or "_type" field) and resolves it to an entt::id_type via the meta registry. */
	[[nodiscard]] entt::id_type GetIdType(const sol::table& comp);

	/* Invokes a named function on a given entt::meta_type at runtime and returns value as an entt::meta_any. */
	template <typename ...Args>
	inline auto InvokeMetaFunction(entt::meta_type meta, entt::id_type funcId, Args&& ...args)
	{
		if (!meta)
		{
			LogError(BrightRed, "[InvokeMetaFunction] No entt::meta_type has been provided or is invalid...");
			assert(false && "No entt::meta_type has been provided or is invalid...\n");
			
			return entt::meta_any{};
		}
		
		if (auto metaFunction = meta.func(funcId); metaFunction)
		{
			return metaFunction.invoke({}, std::forward<Args>(args)...);
		}
		
		LogError(BrightRed,"[InvokeMetaFunction] No meta.func has been provided or is invalid...");
		
		assert(false && "No meta.func has been provided or is invalid...\n");
		return entt::meta_any{};
	}

	/* Convenience overload that resolves a meta_type by ID before invoking and return value as an entt::meta_any */
	template <typename... Args>
	inline auto InvokeMetaFunction(entt::id_type id, entt::id_type funcId, Args&& ...args)
	{
		return InvokeMetaFunction(entt::resolve(id), funcId, std::forward<Args>(args) ...);
	}
	
} // jadeite::InvokeMetaFunction
