
#pragma once

struct RemoveEntityEvent
{
    constexpr RemoveEntityEvent(unsigned int id)
        : id(id)
    { }

    const unsigned int id;
};
