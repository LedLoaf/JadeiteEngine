#include "raycast_callback.hpp"

namespace jadeite
{
	
RayCastCallback::RayCastCallback()
	: m_pHitFixture{ nullptr }
	, m_Point{}
	, m_Normal{}
	, m_Fraction{0.f}
	, m_bHit{ false }
{
}

RayCastCallback::~RayCastCallback() = default;

float RayCastCallback::ReportFixture(b2Fixture* pFixture, const b2Vec2& point, const b2Vec2& normal,
	float fraction ) 
{
	m_bHit = true;//pFixture != nullptr;
	m_pHitFixture = pFixture;
	m_Point = point;
	m_Normal = normal;
	m_Fraction = fraction;
	return 0.f;	
}

} // jadeite