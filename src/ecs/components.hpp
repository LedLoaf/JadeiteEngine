#pragma once
#include "rendering/vertex.hpp"
#include <entt.hpp>
#include <SDL2/SDL.h>
#include <sol/sol.hpp>

namespace jadeite 
{

struct Identification
{
	std::string sTag{ "GameObject" };
	std::string sGroup{ "" };
	uint32_t entityId{ entt::null };
};
	
struct TransformComponent
{
	glm::vec2 position{ 0.f };
	glm::vec2 scale{ 1.f };
	float rotation{ 0.f };
};	

struct SpriteComponent
{
	std::string sTexture{ "" };
	float width{ 16.f };
	float height{ 16.f };
	UV uvs{ };
	int layer{ 0 };
	int startX{ 0 };
	int startY{ 0 };
	bool bHidden{ false };
	Color color{ 255, 255, 255, 255 };
	
	// Values used in animations
	float uvWidthUnpadded{0.f};		/* The actual UV width, without texel compensation*/
	float uvHeightUnpadded{0.f};	/* The actual UV height, without texel compensation*/
	float texelOffsetX{0.f};		/* The size of a texel for the x axis based on the width of the texture*/
	float texelOffsetY{0.f}; 		/* The size of a texel for the y axis based on the height of the texture*/
	
	void generateUVs(int textureWidth, int textureHeight)
	{
		/*uvs.uvWidth = width / textureWidth;
		uvs.uvHeight = height / textureHeight;
		
		uvs.u = startX * uvs.uvWidth;
		uvs.v = startY * uvs.uvHeight;
		*/
		
		// The standard UV scale
		uvWidthUnpadded = width / static_cast<float>(textureWidth);
		uvHeightUnpadded = height / static_cast<float>(textureHeight);
		
		// Half a texel (in UV space)
		texelOffsetX = 0.5f / textureWidth;
		texelOffsetY = 0.5f / textureHeight;
		
		// Shrink width/height by 1 texel total 
		uvs.uvWidth = uvWidthUnpadded - texelOffsetX * 2;
		uvs.uvHeight = uvHeightUnpadded - texelOffsetY * 2;
		
		// Keeps alignment correct and prevents texture bleeding
		float u0 = startX * uvWidthUnpadded + texelOffsetX;
		float v0 = startY * uvHeightUnpadded + texelOffsetY;
			
		// Store new adjusted UVs
		uvs.u = u0;
		uvs.v = v0;
	}
	
	void inspectUVs()
	{
		uvs.u = startX * uvs.uvWidth;
		uvs.v = startY * uvs.uvHeight;
	}
	
	void inspectY()
	{
		uvs.v = startY * uvs.uvHeight;
	}
	
	void inspectX()
	{
		uvs.u = startX * uvs.uvWidth;
	}
};

struct AnimationComponent
{
	int numFrames{ 0 };
	int currentFrame{ 0 };
	int frameRate{ 0 };
	int startTime{ static_cast<int>(SDL_GetTicks()) };
	bool bVertical{ false };
	bool bLooped{ false };
	bool bStop{ false };
	
	void reset()
	{
		currentFrame = 0;
		startTime = static_cast<int>(SDL_GetTicks());
	}
};

struct BoxCollider
{
	int width{ 16 };
	int height{ 16 };
	glm::vec2 offset{ 0.f };
	bool bTrigger{ false };
	bool bCollider{ true };
};

struct CircleCollider
{
	float radius{ 16.f };
	glm::vec2 offset{ 0.f };
	bool bTrigger{ false };
	bool bCollider{ false };
};

struct RigidBodyComponent
{
	glm::vec2 velocity{ 0.f };
	float maxVelocity{ 100.f };
};

struct TextComponent
{
	std::string sFontName{ "pixel" };
	std::string sText{ "Default Text!" };
	Color color{ 255, 255, 255, 255 };
	bool bHidden{ false };
};

struct ComponentBinder
{
	static void CreateLuaBind(sol::state& lua);
};

} // jadeite