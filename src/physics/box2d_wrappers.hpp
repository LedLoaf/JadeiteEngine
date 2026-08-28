#pragma once
#include <box2d/box2d.h>
#include <memory>

namespace jadeite
{
using PhysicsWorld = std::shared_ptr<b2World>;

struct BodyDestroyer
{
	void operator()(b2Body* pBody) const;
};

inline std::shared_ptr<b2Body> MakeSharedBody( b2Body* pBody )
{
	return std::shared_ptr<b2Body>( pBody, BodyDestroyer{});
}

} // jadeite

