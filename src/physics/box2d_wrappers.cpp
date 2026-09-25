#include "box2d_wrappers.hpp"

namespace jadeite
{
	/* Box2D bodies must be destroyed via b2World::DestroyBody(), */
	void BodyDestroyer::operator()(b2Body* pBody) const
	{
		pBody->GetWorld()->DestroyBody( pBody );
	}
} // jadeite::BodyDestroyer
