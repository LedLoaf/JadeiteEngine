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
	, m_Rotation{ 0.f}
	, m_bNeedsUpdate{ true }
{
	Initialize();
}

Camera::~Camera() = default;

void Camera::Update()
{
	if (!m_bNeedsUpdate) 
		return;
	
	// Calculate scale vector
	glm::vec3 scale(m_Scale, m_Scale, 0.f);
	// Calculate translation vector
	glm::vec3 translate(-m_Position.x, -m_Position.y, 0.f);
	
	// Apply scale to camera view
	glm::mat4 cameraView = glm::scale(glm::mat4(1.f), scale);
	// Apply rotation to camera view in radians. (Z-axis)
	cameraView = glm::rotate(cameraView, glm::radians(m_Rotation), glm::vec3(0.f, 0.f, 1.f));
	// Apply translation to camera view
	cameraView = glm::translate(cameraView, translate);

	// Combine the camera view to the projection matrix
	m_CameraMatrix = m_ProjectionMatrix * cameraView;

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
		"get", [&camera](sol::this_state s) 		{ return camera; },
		"getWidth", [&camera]   					{ return camera.GetWidth();	 },
		"getHeight",[&camera]   					{ return camera.GetHeight(); },
		"setScale", [&camera](float scale)			{ camera.SetScale(scale); },
		"getScale", [&camera] 						{ return camera.GetScale(); },
		"getRotation",[&camera] 					{ return camera.GetRotation(); },
		"setRotation", [&camera] (float rotation)	{ camera.SetRotation(rotation);	},
		"setPosition", sol::overload(
			[&camera](float x, float y)
			{
				camera.SetPosition(x, y);
			},
			[&camera](const glm::vec2& position)
			{
				camera.SetPosition(position);
			}
		),
		"getPosition", [&camera]
		{
				//const auto& position = camera.GetPosition();
				//return std::make_tuple(position.x, position.y);
				return camera.GetPosition();
		},
		"move",[&camera](const glm::vec2& distance)
		{
			camera.SetPosition(camera.GetPosition() + distance);
		}
	);
}

} // jadeite
