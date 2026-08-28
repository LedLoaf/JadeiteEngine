#pragma once
#include <box2d/box2d.h>
#include <vector>

namespace jadeite
{

class BoxTraceCallback : public b2QueryCallback
{
public:
	BoxTraceCallback() = default;
	~BoxTraceCallback() = default;
	
	virtual bool ReportFixture( b2Fixture* pFixture ) override;
	
	inline std::vector<b2Body*>& GetBodies() { return m_Bodies; }
	
private:
	std::vector<b2Body*> m_Bodies;
};

} // jadeite