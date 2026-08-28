#include "core_data.hpp"

namespace jadeite
{

constexpr float METERS_TO_PIXELS = 12.f;
constexpr float PIXELS_TO_METERS = 1.f / METERS_TO_PIXELS;

	
CoreData& CoreData::GetInstance()
{
	static CoreData instance{};
	return instance;
}

void CoreData::SetWindowWidth( int windowWidth )
{
	m_WindowWidth = windowWidth;
	m_ScaledWidth = m_WindowWidth / METERS_TO_PIXELS;
}

void CoreData::SetWindowHeight( int windowHeight)
{
	m_WindowHeight = windowHeight;
	m_ScaledHeight = m_WindowHeight / METERS_TO_PIXELS;
}

float CoreData::MetersToPixels() const
{
	return METERS_TO_PIXELS;
}

float CoreData::PixelsToMeters() const
{
	return PIXELS_TO_METERS;
}

void CoreData::SetScaledWidth( int newWidth )
{
	m_ScaledWidth = newWidth / METERS_TO_PIXELS;
}

void CoreData::SetScaledHeight( int newHeight)
{
	m_ScaledHeight = newHeight / METERS_TO_PIXELS;
}

CoreData::CoreData()
	: m_ScaledWidth{ 0.f }
	, m_ScaledHeight{ 0.f }
	, m_Gravity{ 9.8f }
	, m_WindowWidth{ 640 }
	, m_WindowHeight{ 480 }
	, m_VelocityIterations{ 10 }
	, m_PositionIterations{ 8 }
	, m_bPhysicsEnabled{ true }
	, m_bPhysicsPaused{ false }
{
	m_ScaledWidth = m_WindowWidth / METERS_TO_PIXELS;
	m_ScaledHeight = m_WindowHeight / METERS_TO_PIXELS;
}

} // jadeite
