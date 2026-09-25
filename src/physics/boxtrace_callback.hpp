#pragma once
#include <box2d/box2d.h>
#include <vector>

namespace jadeite
{
	
	/* 	Broad-phase query callback used by BoxTrace.
		During the swept-box query, Box2D calls ReportFixture once per fixture whose AABB overlaps the swept region.
		This callback collects the candidate bodies into m_Bodies for the subsequent narrow-phase TOI (time-of-impact) pass, 
		where the actual closest hit is determined. */
	class BoxTraceCallback : public b2QueryCallback
	{
	public:
		BoxTraceCallback() = default;
		~BoxTraceCallback() = default;
		
		virtual bool ReportFixture( b2Fixture* pFixture ) override;
		
		inline std::vector<b2Body*>& GetBodies() { return m_Bodies; }
		
	private:
		std::vector<b2Body*> m_Bodies;		// candidate bodies for the time of impact pass
	};
} // jadeite