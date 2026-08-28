#pragma once
#include <cstdint>

namespace jadeite
{

enum class RigidBodyType
{
	/* Zero mass, zero velocity, may be manually moved. */
	STATIC = 0,
	/* Zero mass, velocity set by user, moved by solver. */
	KINEMATIC,
	/* Positive mass, velocity detemined by forces, moved by solver. */
	DYNAMIC
};
	
enum class FilterCategory : std::uint16_t
{
	NO_CATEGORY =			0,
	PLAYER = 				1 << 0, // ...0000 0001
	ENEMY = 				1 << 1, // ...0000 0010
	ITEM = 					1 << 2, // ...0000 0100
	WALLS = 				1 << 3, // ...0000 1000
	GROUND = 				1 << 4, // ...0001 0000
	TRIGGER = 				1 << 5, // ...0010 0000
	PROJECTILE = 			1 << 6, // ...0100 0000
	CATEGORY_7 = 			1 << 7, // ...1000 0000
	CATEGORY_8 = 			1 << 8, 
	CATEGORY_9 = 			1 << 9, 
	CATEGORY_10 = 			1 << 10, 
	CATEGORY_11 = 			1 << 11, 
	CATEGORY_12 = 			1 << 12, 
	CATEGORY_13 = 			1 << 13, 
	CATEGORY_14 = 			1 << 14, 
	CATEGORY_15 = 			1 << 15 
};	
	
} // jadeite
