#include "box2d_wrappers.hpp"

namespace jadeite
{

void BodyDestroyer::operator()(b2Body* pBody) const
{
	pBody->GetWorld()->DestroyBody( pBody );
}


} // jadeite
