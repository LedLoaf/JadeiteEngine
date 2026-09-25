#pragma once 
#include <box2d/box2d.h>

namespace jadeite
{
	/* 	Ray-cast query callback used by PhysicsComponent::CastRay.
		During b2World::RayCast, Box2D calls ReportFixture once per fixture	the segment intersects. 
		This callback records the closest hit (fixture, point, normal) and returns `fraction` to clip the ray, so only the nearest intersection is reported.
		If no fixture is hit, m_fixture remains nullptr and CastRay	returns false. */
	class RayCastCallback : public b2RayCastCallback
	{
	public:
		RayCastCallback();
		~RayCastCallback() = default;
		
		virtual float ReportFixture(b2Fixture* pFixture, const b2Vec2& point, const b2Vec2& normal,
			float fraction ) override;
			
		inline const bool IsHit() const { return m_bHit; }
		inline b2Fixture* HitFixture() { return m_pHitFixture; }
		inline const b2Vec2& HitPoint() const { return m_Point; }
		inline const b2Vec2& HitNormal() const { return m_Normal; }
		inline const float HitFraction() const { return m_Fraction; }
		
	private:
		/* The fixture hit by the ray. */
		b2Fixture* m_pHitFixture;
		/* The point of initial intersection. */
		b2Vec2 m_Point;
		/* The normal vector at the point of intersection. */
		b2Vec2 m_Normal;
		/* The fraction along the ray at the point of intersection. */
		float m_Fraction;
		/* Did the ray hit something. */
		bool m_bHit;
	};
} // jadeite