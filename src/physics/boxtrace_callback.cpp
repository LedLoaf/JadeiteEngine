#include "boxtrace_callback.hpp"

namespace jadeite
{
	/* Broad-phase collection callback for BoxTrace. Called once per fixture whose AABB overlaps the swept query region. */
	bool BoxTraceCallback::ReportFixture( b2Fixture* pFixture ) 
	{
		m_Bodies.push_back( pFixture->GetBody() );
		
		return pFixture != nullptr;
	}
} // jadeite::BoxTraceCallback