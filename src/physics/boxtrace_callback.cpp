#include "boxtrace_callback.hpp"

namespace jadeite
{

bool BoxTraceCallback::ReportFixture( b2Fixture* pFixture ) 
{
	m_Bodies.push_back( pFixture->GetBody() );
	
	return pFixture != nullptr;
}

} // jadeite