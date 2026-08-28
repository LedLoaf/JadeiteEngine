#include "components.hpp"

namespace jadeite
{

void ComponentBinder::CreateLuaBind(sol::state& lua)
{
	lua.new_usertype<Identification>(
		"Identification",
		"type_id", &entt::type_hash<Identification>::value,
		sol::call_constructor,
		sol::factories(
			[]{ return Identification{}; },
			[](const std::string& sTag, const std::string& sGroup)
			{
				return Identification{ .sTag = sTag, .sGroup = sGroup };
			}
		),
		"tag", &Identification::sTag,
		"group", &Identification::sGroup,
		"entityId", &Identification::entityId
	);
	
	lua.new_usertype<TransformComponent>(
		"Transform",
		"type_id", &entt::type_hash<TransformComponent>::value,
		sol::call_constructor,
		sol::factories(
			[]{ return TransformComponent{}; },
			[](const glm::vec2& pos, const glm::vec2& scale, float rotation)
			{
				return TransformComponent{
					.position = pos,
					.scale = scale,
					.rotation = rotation
				};
			},
			[](float x, float y, float scaleX, float scaleY, float rotation)
			{
				return TransformComponent{
					.position = glm::vec2{ x, y },
					.scale = glm::vec2{ scaleX, scaleY },
					.rotation = rotation
				};
			}
		),
		"position", &TransformComponent::position,
		"scale", &TransformComponent::scale,
		"rotation", &TransformComponent::rotation
	);
	
	lua.new_usertype<AnimationComponent>(
		"Animation",
		"type_id", &entt::type_hash<AnimationComponent>::value,
		sol::call_constructor,
		sol::factories(
			[]{ return AnimationComponent{}; },
			[](int numFrames, int frameRate, int frameOffset, bool bVertical, bool bLooped)
			{
				return AnimationComponent{
					.numFrames = numFrames,
					.frameRate = frameRate,
					.frameOffset = frameOffset,
					.bVertical = bVertical,
					.bLooped = bLooped
				};
			}
		), 
		"numFrames", &AnimationComponent::numFrames,
		"currentFrame", &AnimationComponent::currentFrame,
		"frameRate", &AnimationComponent::frameRate,
		"frameOffset", &AnimationComponent::frameOffset,
		"startTime", &AnimationComponent::startTime,
		"bVertical", &AnimationComponent::bVertical,
		"bLooped", &AnimationComponent::bLooped,
		"bStop", &AnimationComponent::bStop,
		"reset", &AnimationComponent::reset
	);
	
	lua.new_usertype<BoxCollider>(
		"BoxCollider",
		"type_id", &entt::type_hash<BoxCollider>::value,
		sol::call_constructor,
		sol::factories(
			[]{ return BoxCollider{}; },
			[](int width, int height , glm::vec2 offset )
			{
				return BoxCollider{ .width = width, .height = height, .offset = offset };
			}
		),
		"width", &BoxCollider::width,
		"height", &BoxCollider::height,
		"offset", &BoxCollider::offset
	);
	
	lua.new_usertype<CircleCollider>(
		"CircleCollider",
		"type_id", &entt::type_hash<CircleCollider>::value,
		sol::call_constructor,
		sol::factories(
			[]{ return CircleCollider{}; },
			[](float radius, glm::vec2 offset )
			{
				return CircleCollider{ .radius = radius, .offset = offset };
			}
		),
		"radius", &CircleCollider::radius,
		"offset", &CircleCollider::offset
	);
	
	lua.new_usertype<SpriteComponent>(
		"Sprite",
		"type_id", &entt::type_hash<SpriteComponent>::value,
		sol::call_constructor,
		sol::factories(
			[]{ return SpriteComponent{}; },
			[](const std::string& sTexture, float width, float height,
				int layer, int startX, int startY, Color color)
			{
				return SpriteComponent{
					.sTexture = sTexture,
					.width = width,
					.height = height,
					.layer = layer,
					.startX = startX,
					.startY = startY,
					.color = color
				};
			}
		),
		"sTexture", &SpriteComponent::sTexture,
		"width", &SpriteComponent::width,
		"height", &SpriteComponent::height,
		"layer", &SpriteComponent::layer,
		"startX", &SpriteComponent::startX,
		"startY", &SpriteComponent::startY,
		"uvs", &SpriteComponent::uvs,
		"bHidden", &SpriteComponent::bHidden,
		"color", &SpriteComponent::color,
		"generateUVs", &SpriteComponent::generateUVs,
		"inspectUVs", &SpriteComponent::inspectUVs,
		"inspectY", &SpriteComponent::inspectY,
		"inspectX", &SpriteComponent::inspectX
	);
	
	lua.new_usertype<RigidBodyComponent>(
		"type_id", &entt::type_hash<RigidBodyComponent>::value,
		"RigidBody",
		sol::call_constructor,
		sol::factories(
			[]{ return RigidBodyComponent{}; },
			[]( float maxVelocity ) 
			{ 
				return RigidBodyComponent{ .maxVelocity = maxVelocity }; 
			}
		),
		"velocity", [](RigidBodyComponent& rb) 
		{ 
			return std::make_tuple(rb.velocity.x, rb.velocity.y); 
		},
		"maxVelocity", &RigidBodyComponent::maxVelocity
	);
	
	lua.new_usertype<TextComponent>(
		"TextComponent",
		"type_id", &entt::type_hash<TextComponent>::value,
		sol::call_constructor,
		sol::factories(
			[]{ return TextComponent{};},
			[](const std::string& sFont, const std::string& sText)
			{
				return TextComponent{ .sFontName = sFont, .sText = sText };
			},
			[](const std::string& sFont, const std::string& sText, const Color& color)
			{
				return TextComponent{ .sFontName = sFont, .sText = sText, .color = color };
			}
		),
		"sFontName", &TextComponent::sFontName,
		"sText", &TextComponent::sText,
		"color", &TextComponent::color,
		"bHidden", &TextComponent::bHidden
	);
}

} // jadeite
