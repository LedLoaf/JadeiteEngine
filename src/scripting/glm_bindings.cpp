#include "glm_bindings.hpp"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

namespace jadeite
{
	
// glm::vec2 
static void CreateVec2Bind(sol::state& lua)
{
	auto vec2_multiply_overloads = sol::overload(
		[](const glm::vec2& v1, const glm::vec2& v2) { return v1 * v2; },
		[](const glm::vec2& v1, float value) { return v1 * value; },
		[]( float value, const glm::vec2& v1 ) { return v1 * value; }
	);
	
	auto vec2_divide_overloads = sol::overload(
		[](const glm::vec2& v1, const glm::vec2& v2) { return v1 / v2; },
		[](const glm::vec2& v1, float value) { return v1 / value; },
		[]( float value, const glm::vec2& v1 ) { return v1 / value; }
	);
	
	auto vec2_addition_overloads = sol::overload(
		[](const glm::vec2& v1, const glm::vec2& v2) { return v1 + v2; },
		[](const glm::vec2& v1, float value) { return v1 + value; },
		[]( float value, const glm::vec2& v1 ) { return v1 + value; }
	);
	
	auto vec2_subraction_overloads = sol::overload(
		[](const glm::vec2& v1, const glm::vec2& v2) { return v1 - v2; },
		[](const glm::vec2& v1, float value) { return v1 - value; },
		[]( float value, const glm::vec2& v1 ) { return v1 - value; }
	);
	
	lua.new_usertype<glm::vec2>(
		"vec2",
		sol::call_constructor,
		sol::constructors<glm::vec2(float), glm::vec2(float, float)>(),
		"x", &glm::vec2::x,
		"y", &glm::vec2::y,
		sol::meta_function::multiplication, vec2_multiply_overloads,
		sol::meta_function::division, vec2_divide_overloads,
		sol::meta_function::addition, vec2_addition_overloads,
		sol::meta_function::subtraction, vec2_subraction_overloads,
		"length", [](const glm::vec2& v) { return glm::length(v); },
		"lengthSq", [](const glm::vec2& v) { return glm::length2(v); },
		"normalize", [](const glm::vec2& v) { return glm::normalize(v); },
		"normalize2", [](const glm::vec2& v1, const glm::vec2& v2) { return glm::normalize(v2 - v1); },
		"nearlyZeroX", [](const glm::vec2& v) { return glm::epsilonEqual(v.x, 0.f, 0.001f); },
		"nearlyZeroY", [](const glm::vec2& v) { return glm::epsilonEqual(v.y, 0.f, 0.001f); }
	);
}

// glm::vec3 
static void CreateVec3Bind(sol::state& lua)
{
	auto vec3_multiply_overloads = sol::overload(
		[](const glm::vec3& v1, const glm::vec3& v2) { return v1 * v2; },
		[](const glm::vec3& v1, float value) { return v1 * value; },
		[]( float value, const glm::vec3& v1 ) { return v1 * value; }
	);
	
	auto vec3_divide_overloads = sol::overload(
		[](const glm::vec3& v1, const glm::vec3& v2) { return v1 / v2; },
		[](const glm::vec3& v1, float value) { return v1 / value; },
		[]( float value, const glm::vec3& v1 ) { return v1 / value; }
	);
	
	auto vec3_addition_overloads = sol::overload(
		[](const glm::vec3& v1, const glm::vec3& v2) { return v1 + v2; },
		[](const glm::vec3& v1, float value) { return v1 + value; },
		[]( float value, const glm::vec3& v1 ) { return v1 + value; }
	);
	
	auto vec3_subraction_overloads = sol::overload(
		[](const glm::vec3& v1, const glm::vec3& v2) { return v1 - v2; },
		[](const glm::vec3& v1, float value) { return v1 - value; },
		[]( float value, const glm::vec3& v1 ) { return v1 - value; }
	);
	
	lua.new_usertype<glm::vec3>(
		"vec3",
		sol::call_constructor,
		sol::constructors<glm::vec3(float), glm::vec3(float, float, float)>(),
		"x", &glm::vec3::x,
		"y", &glm::vec3::y,
		"z", &glm::vec3::z,
		sol::meta_function::multiplication, vec3_multiply_overloads,
		sol::meta_function::division, vec3_divide_overloads,
		sol::meta_function::addition, vec3_addition_overloads,
		sol::meta_function::subtraction, vec3_subraction_overloads,
		"length", [](const glm::vec3& v) { return glm::length(v); },
		"lengthSq", [](const glm::vec3& v) { return glm::length2(v); },
		"normalize", [](const glm::vec3& v) { return glm::normalize(v); },
		"normalize2", [](const glm::vec3& v1, const glm::vec3& v2) { return glm::normalize(v2 - v1); },
		"nearlyZeroX", [](const glm::vec3& v) { return glm::epsilonEqual(v.x, 0.f, 0.001f); },
		"nearlyZeroY", [](const glm::vec3& v) { return glm::epsilonEqual(v.y, 0.f, 0.001f); },
		"nearlyZeroZ", [](const glm::vec3& v) { return glm::epsilonEqual(v.z, 0.f, 0.001f); }
	);
}

// glm::vec4 
static void CreateVec4Bind(sol::state& lua)
{
	auto vec4_multiply_overloads = sol::overload(
		[](const glm::vec4& v1, const glm::vec4& v2) { return v1 * v2; },
		[](const glm::vec4& v1, float value) { return v1 * value; },
		[]( float value, const glm::vec4& v1 ) { return v1 * value; }
	);
	
	auto vec4_divide_overloads = sol::overload(
		[](const glm::vec4& v1, const glm::vec4& v2) { return v1 / v2; },
		[](const glm::vec4& v1, float value) { return v1 / value; },
		[]( float value, const glm::vec4& v1 ) { return v1 / value; }
	);
	
	auto vec4_addition_overloads = sol::overload(
		[](const glm::vec4& v1, const glm::vec4& v2) { return v1 + v2; },
		[](const glm::vec4& v1, float value) { return v1 + value; },
		[]( float value, const glm::vec4& v1 ) { return v1 + value; }
	);
	
	auto vec4_subraction_overloads = sol::overload(
		[](const glm::vec4& v1, const glm::vec4& v2) { return v1 - v2; },
		[](const glm::vec4& v1, float value) { return v1 - value; },
		[]( float value, const glm::vec4& v1 ) { return v1 - value; }
	);
	
	lua.new_usertype<glm::vec4>(
		"vec4",
		sol::call_constructor,
		sol::constructors<glm::vec4(float), glm::vec4(float, float, float, float)>(),
		"x", &glm::vec4::x,
		"y", &glm::vec4::y,
		"z", &glm::vec4::z,
		"w", &glm::vec4::w,
		sol::meta_function::multiplication, vec4_multiply_overloads,
		sol::meta_function::division, vec4_divide_overloads,
		sol::meta_function::addition, vec4_addition_overloads,
		sol::meta_function::subtraction, vec4_subraction_overloads,
		"length", [](const glm::vec4& v) { return glm::length(v); },
		"lengthSq", [](const glm::vec4& v) { return glm::length2(v); },
		"normalize", [](const glm::vec4& v) { return glm::normalize(v); },
		"normalize2", [](const glm::vec4& v1, const glm::vec4& v2) { return glm::normalize(v2 - v1); },
		"nearlyZeroX", [](const glm::vec4& v) { return glm::epsilonEqual(v.x, 0.f, 0.001f ); },
		"nearlyZeroY", [](const glm::vec4& v) { return glm::epsilonEqual(v.y, 0.f, 0.001f ); },
		"nearlyZeroZ", [](const glm::vec4& v) { return glm::epsilonEqual(v.z, 0.f, 0.001f ); },
		"nearlyZeroW", [](const glm::vec4& v) { return glm::epsilonEqual(v.w, 0.f, 0.001f ); }
	);
}

static void MathFreeHelperFunctions(sol::state& lua)
{
	lua.set_function("J2D_distance", sol::overload(
		[](const glm::vec2& v1, const glm::vec2& v2) { return glm::distance(v1, v2); },
		[](const glm::vec3& v1, const glm::vec3& v2) { return glm::distance(v1, v2); },
		[](const glm::vec4& v1, const glm::vec4& v2) { return glm::distance(v1, v2); }
	));
	
	lua.set_function("J2D_lerp", [](float a, float b, float t) { return std::lerp(a, b, t); });
	
	lua.set_function("J2D_clamp", sol::overload(
		[](float value, float min, float max) { return std::clamp(value, min, max); },
		[](double value, double min, double max) { return std::clamp(value, min, max); },
		[](int value, int min, int max) { return std::clamp(value, min, max); }
	));
	
	lua.set_function("J2D_dot", sol::overload(
		[](const glm::vec2& v1, const glm::vec2 v2) {  return glm::dot(v1, v2); },
		[](const glm::vec3& v1, const glm::vec3 v2) {  return glm::dot(v1, v2); },
		[](const glm::vec4& v1, const glm::vec4 v2) {  return glm::dot(v1, v2); }
	));
	
	lua.set_function("J2D_cross", sol::overload(
		[](const glm::vec2& v1, const glm::vec2& v2) { return v1.x * v2.y - v2.x * v1.y; },
		[](const glm::vec3& v1, const glm::vec3& v2) { return glm::cross(v1, v2); }
	));
}

static void MathConstants(sol::state& lua)
{
	lua.set("J2D_PI", 3.14159265359f);
	lua.set("J2D_TWO_PI", 6.28318530717f);
	lua.set("J2D_PI_OVER_2", 1.57079632679f);
	lua.set("J2D_PI_OVER_4", 0.78539816339f);
	lua.set("J2D_SQRT_2", 1.41421356237f);
	lua.set("J2D_INV_SQRT_2", 0.707106781187f);
}


void GlmBinder::CreateLuaBind(sol::state& lua)
{
	CreateVec2Bind(lua);
	CreateVec3Bind(lua);
	CreateVec4Bind(lua);
	
	MathFreeHelperFunctions(lua);
	MathConstants(lua);
}

} // jadeite
