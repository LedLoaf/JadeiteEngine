#include "registry.hpp"
#include "entity.hpp"
#include "meta_utilities.hpp"

namespace jadeite
{
Registry::Registry()
	: m_pRegistry{ std::make_shared<entt::registry>() }
{
}

void Registry::CreateLuaBind(sol::state& lua, Registry& registry)
{
	using namespace entt::literals;
	lua.new_enum<ERegistryType>(
		"RegistryType",
		{
			{ "Lua", ERegistryType::Lua },
			{ "Jadeite", ERegistryType::Jadeite }
		}
	);
	
	lua.new_usertype<entt::runtime_view>(
		"runtime_view", 
		sol::no_constructor,
		"for_each",
		sol::overload(
			[&registry]( const entt::runtime_view& view, const sol::function& callback )
			{
				if (!callback.valid())
					return;
				
				for ( auto entity : view )
				{
					Entity ent{ registry, entity };
					callback(ent);
				}
			}, 
			[]( const entt::runtime_view& view, Registry& reg, const sol::function& callback )
			{
				if (!callback.valid())
					return;
				
				for ( auto entity : view )
				{
					Entity ent{ reg, entity };
					callback(ent);
				}
			}
		),
		"exclude",
		[ &registry ]( entt::runtime_view& view, const sol::variadic_args& va ) {
			Registry* pRegistry = &registry;
			auto it = va.begin();

			// Check to see if the first argument is a registry
			if ( it != va.end() && it->is<Registry>() )
			{
				pRegistry = &it->as<Registry&>();
				++it; // Skip past the registry
			}

			for ( ; it != va.end(); ++it )
			{
				sol::object obj = *it;
				if ( !obj.is<sol::table>() )
					continue;

				sol::table type = obj.as<sol::table>();
				if ( !type.valid() )
					continue;

				InvokeMetaFunction( GetIdType( type ), "excludeComponentFromView"_hs, pRegistry, &view );
			}

			return view;
		}
	);
	
	lua.new_usertype<Registry>(
		"Registry",
		sol::call_constructor,
		sol::factories(
			[&registry](sol::this_state s) { return sol::make_reference(s, std::ref(registry)); },
			[](ERegistryType eType, sol::this_state s)
			{
				return eType != ERegistryType::Lua ? sol::lua_nil_t{} 
					: sol::make_object(s, Registry{} );
			}
		),
		"getEntities",
		[](Registry& reg, const sol::variadic_args& va)
		{
			entt::runtime_view view{};
			for ( const auto& type : va )
			{
				if (!type.as<sol::table>().valid())
						continue;
					
				const auto entities = InvokeMetaFunction(
					GetIdType(type), "addComponentToView"_hs, &reg, view
				);
				
				view = entities ? entities.cast<entt::runtime_view>() : view;
			}
			
			return view;
		},
		"createEntity",
		[](Registry& reg) { return Entity{ reg }; },
		"clear",
		[](Registry& reg) { reg.ClearRegistry(); }
	);
}

} // jadeite
