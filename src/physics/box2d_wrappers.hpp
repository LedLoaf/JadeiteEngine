#pragma once
#include <box2d/box2d.h>
#include <memory>

namespace jadeite
{
	using PhysicsWorld = std::shared_ptr<b2World>;

	/* Custom deleter for std::shared_ptr<b2Body>. Box2D bodies must be destroyed via b2World::DestroyBody(), */
	struct BodyDestroyer
	{
		void operator()(b2Body* pBody) const;
	};

	/* Wraps a raw b2Body pointer (typically obtained from b2World::CreateBody) in a shared_ptr with the custom deleter */
	inline std::shared_ptr<b2Body> MakeSharedBody( b2Body* pBody )
	{
		return std::shared_ptr<b2Body>( pBody, BodyDestroyer{});
	}
} // jadeite

