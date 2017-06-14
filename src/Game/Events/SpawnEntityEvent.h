
#pragma once

#include "MonoFwd.h"

struct SpawnEntityEvent
{
    SpawnEntityEvent(const mono::IEntityPtr& entity)
        : entity(entity)
    { }

    const mono::IEntityPtr entity;
};
