#include "meta_utilities.hpp"

namespace jadeite
{
	
entt::id_type GetIdType(const sol::table& comp)
{
	if (!comp.valid())
	{
		std::cerr << "Failed to get the type id -- Component has not been exposed to lua!\n";
		assert(comp.valid() && "Failed to get the type id -- Component has not been exposed to lua!");
		return -1;
	}
	
	const auto func = comp["type_id"].get<sol::function>();
	assert(func.valid() && 
		"[type_id()] - function has not been exposed to lua!\n"
		"Please ensure all components and types have a type_id function\n"
		"when creating a new usertype."
	);
	
	return func.valid() ? func().get<entt::id_type>() : -1;
}

} // jadeite
