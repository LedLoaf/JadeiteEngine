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
	
	void generateUVs(int textureWidth, int textureHeight)
	{
		uvs.uvWidth = width / textureWidth;
		uvs.uvHeight = height / textureHeight;
		
		uvs.u = startX * uvs.uvWidth;
		uvs.v = startY * uvs.uvHeight;
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
	int frameOffset{ 0 };
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