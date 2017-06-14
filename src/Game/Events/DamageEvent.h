
#pragma once

#include "Physics/CMFwd.h"

struct DamageEvent
{
    DamageEvent(const mono::IBodyPtr& body, int damage)
        : body(body),
          damage(damage)
    { }

    const mono::IBodyPtr body;
    const int damage;
};
