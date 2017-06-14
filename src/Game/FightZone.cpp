
#include "FightZone.h"

#include "Fighter.h"
#include "FighterFactory.h"

#include "Events/GameEventFuncFwd.h"
#include "Events/SpawnEntityEvent.h"
#include "Events/RemoveEntityEvent.h"
#include "UpdateTasks/CollisionHandler.h"

#include "EventHandler/EventHandler.h"

FightZone::FightZone(mono::EventHandler& event_handler)
    : PhysicsZone(math::Vector(0.0f, 9.81f), 0.9f),
      m_event_handler(event_handler)
{
    m_props = {
        math::Quad(100, 80, 120, 120),
        math::Quad(200, 80, 220, 220)
    };

    using namespace std::placeholders;

    const SpawnEntityFunc& spawn_func = std::bind(&FightZone::SpawnEntity, this, _1);
    const RemoveEntityFunc& despawn_func = std::bind(&FightZone::DespawnEntity, this, _1);

    m_spawn_token = m_event_handler.AddListener(spawn_func);
    m_despawn_token = m_event_handler.AddListener(despawn_func);
}

FightZone::~FightZone()
{
    m_event_handler.RemoveListener(m_spawn_token);
    m_event_handler.RemoveListener(m_despawn_token);    
}

void FightZone::OnLoad(mono::ICameraPtr camera)
{
    auto fighter = CreateFighter(m_event_handler);
    fighter->SetPosition(math::Vector(100, 100));

    auto ai_fighter = CreateAIFighter(m_event_handler, fighter);
    ai_fighter->SetPosition(math::Vector(200, 100));

    AddEntity(ai_fighter, 1);
    AddEntity(fighter, 1);
    AddUpdatable(std::make_shared<CollisionHandler>(m_event_handler, fighter, ai_fighter, m_props));
}

void FightZone::OnUnload()
{ }

bool FightZone::SpawnEntity(const SpawnEntityEvent& event)
{
    AddEntity(event.entity, 2);
    return false;
}

bool FightZone::DespawnEntity(const RemoveEntityEvent& event)
{
    const auto entity = FindEntityFromId(event.id);
    if(entity)
        RemoveEntity(entity);

    return false;
}
