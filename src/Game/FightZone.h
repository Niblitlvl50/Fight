
#pragma once

#include "Zone/PhysicsZone.h"
#include "EventHandler/EventToken.h"

#include "Math/Quad.h"
#include <vector>

class FightZone : public mono::PhysicsZone
{
public:

    FightZone(mono::EventHandler& event_handler);
    ~FightZone();

    virtual void OnLoad(mono::ICameraPtr camera);
    virtual void OnUnload();

    mono::EventHandler& m_event_handler;

    std::vector<math::Quad> m_props;
};
