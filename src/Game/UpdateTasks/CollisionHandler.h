
#pragma once

#include "IUpdatable.h"
#include "Math/Quad.h"
#include "MonoFwd.h"

#include <vector>
#include <memory>

class Fighter;
class DamageController;

class CollisionHandler : public mono::IUpdatable
{
public:

    CollisionHandler(
        mono::EventHandler& event_handler,
        DamageController& damage_controller,
        const std::shared_ptr<Fighter>& fighter1,
        const std::shared_ptr<Fighter>& fighter2,
        const std::vector<math::Quad>& props);

    void doUpdate(unsigned int delta);
    void SpawnHitEffect(const math::Vector& position);

    mono::EventHandler& m_event_handler;
    DamageController& m_damage_controller;
    const std::shared_ptr<Fighter> m_fighter1;
    const std::shared_ptr<Fighter> m_fighter2;
    const std::vector<math::Quad>& m_props;
};
