
#pragma once

#include "Zone/PhysicsZone.h"
#include "EventHandler/EventToken.h"

#include "Math/Quad.h"
#include <vector>

struct SpawnEntityEvent;
struct RemoveEntityEvent;

class FightZone : public mono::PhysicsZone
{
public:

    FightZone(mono::EventHandler& event_handler);
    ~FightZone();

    virtual void OnLoad(mono::ICameraPtr camera);
    virtual void OnUnload();

    bool SpawnEntity(const SpawnEntityEvent& event);
    bool DespawnEntity(const RemoveEntityEvent& event);

    mono::EventHandler& m_event_handler;
    std::vector<math::Quad> m_props;

    mono::EventToken<SpawnEntityEvent> m_spawn_token;
    mono::EventToken<RemoveEntityEvent> m_despawn_token;
};
