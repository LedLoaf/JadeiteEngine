#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace jadeite
{
Camera::Camera()
	: Camera(1366, 768)
{
}

Camera::Camera(int width, int height)
	: m_Position{ 0.f }
	, m_CameraMatrix{ 1.f }
	, m_ProjectionMatrix{ 1.f }
	, m_Width{ width }
	, m_Height{ height }
	, m_Scale{ 1.f }
	, m_bNeedsUpdate{ true }
{
	Initialize();
}

Camera::~Camera() = default;

void Camera::Update()
{
	if (!m_bNeedsUpdate) 
		return;
	
	// Translate 
	glm::vec3 translate(-m_Position.x, -m_Position.y, 0.f);
	m_CameraMatrix = glm::translate(m_ProjectionMatrix, translate);
	
	// Scale 
	glm::vec3 scale(m_Scale, m_Scale, 0.f);
	m_CameraMatrix *= glm::scale(glm::mat4{1.f}, scale);
	
	m_bNeedsUpdate = false;
}

void Camera::Initialize()
{
	m_ProjectionMatrix = glm::ortho(
		0.f, 								// Left
		static_cast<float>(m_Width),		// Right
		static_cast<float>(m_Height),		// Bottom
		0.f,								// Top
		-1.f, 								// Near
		1.f									// Far
	);
}


void Camera::CreateLuaBind(sol::state& lua, Camera& camera)
{
	lua.new_usertype<Camera>(
		"Camera",
		sol::no_constructor,
		"get", [&camera](sol::this_state s) { return camera; },
		"setPosition", sol::overload(
			[&camera](float x, float y)
			{
				camera.SetPosition(x, y);
			}
		),
		"setScale", [&camera](float scale){ camera.SetScale(scale); },
		"getPosition", [&camera]
		{
				const auto& position = camera.GetPosition();
				return std::make_tuple(position.x, position.y);
		},
		"move",[&camera](const glm::vec2& distance)
		{
			camera.SetPosition(camera.GetPosition() + distance);
		}
	);
}

} // jadeite
