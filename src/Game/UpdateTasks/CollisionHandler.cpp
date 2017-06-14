
#include "CollisionHandler.h"
#include "Fighter.h"
#include "Explosion.h"
#include "Events/SpawnEntityEvent.h"
#include "EventHandler/EventHandler.h"

#include "Math/MathFunctions.h"

#include <cstdio>

CollisionHandler::CollisionHandler(
    mono::EventHandler& event_handler,
    const std::shared_ptr<Fighter>& fighter1,
    const std::shared_ptr<Fighter>& fighter2,
    const std::vector<math::Quad>& props)
    : m_event_handler(event_handler),
      m_fighter1(fighter1),
      m_fighter2(fighter2),
      m_props(props)
{ }

void CollisionHandler::doUpdate(unsigned int delta)
{
    const bool attacking = m_fighter1->IsAttacking();
    if(!attacking)
        return;

    const bool invulnerable = m_fighter2->IsInvulnerable();
    if(invulnerable)
        return;

    const math::Vector& attack_position = m_fighter1->AttackPosition();
    const std::vector<math::Quad>& hit_boxes = m_fighter2->HitBoxes();

    for(const math::Quad& prop : hit_boxes)
    {
        const bool hit = math::PointInsideQuad(attack_position, prop);
        if(hit)
        {
            m_fighter2->OnHit(attack_position);
            SpawnHitEffect(attack_position);
            break;
        }
    }
}

void CollisionHandler::SpawnHitEffect(const math::Vector& position)
{
    ExplosionConfiguration config;
    config.position = position;
    config.scale = 10.0f;
    config.sprite_file = "sprites/explosion.sprite";

    const SpawnEntityEvent event(std::make_shared<Explosion>(config, m_event_handler));
    m_event_handler.DispatchEvent(event);
}
