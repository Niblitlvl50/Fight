
#pragma once

#include "Math/Quad.h"
#include <vector>

class ICollisionObject
{
public:

    virtual ~ICollisionObject()
    { }

    virtual std::vector<math::Quad> CollisionBoxes() const = 0;
    virtual void CollideOnIndex(int index) = 0;
};
