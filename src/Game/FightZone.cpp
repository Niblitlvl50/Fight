
#include "FightZone.h"

#include "Fighter.h"
#include "FighterFactory.h"
#include "RenderLayers.h"

#include "Events/GameEventFuncFwd.h"
#include "Events/SpawnEntityEvent.h"
#include "Events/RemoveEntityEvent.h"
#include "UpdateTasks/CollisionHandler.h"
#include "UpdateTasks/HealthbarUpdater.h"

#include "EventHandler/EventHandler.h"
#include "Rendering/Texture/TextureFactory.h"
#include "Rendering/ICamera.h"

namespace
{
    class StageBackgroundDrawable : public mono::IDrawable
    {
    public:

        StageBackgroundDrawable(const StageBackground& background)
            : m_background(background)
        { }

        virtual void doDraw(mono::IRenderer& renderer) const
        {
            DrawBackground(renderer, m_background);
        }

        virtual math::Quad BoundingBox() const
        {
            return math::Quad(-math::INF, -math::INF, math::INF, math::INF);
        }

        const StageBackground& m_background;
    };
}

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

void FightZone::OnLoad(mono::ICameraPtr& camera)
{
    auto fighter = CreateFighter(m_event_handler);
    fighter->SetPosition(math::Vector(100, 100));

    auto ai_fighter = CreateAIFighter(m_event_handler, fighter);
    ai_fighter->SetPosition(math::Vector(200, 100));

    m_damage_controller.CreateRecord(fighter->Id());
    m_damage_controller.CreateRecord(ai_fighter->Id());

    const math::Quad& viewport = camera->GetViewport();
    const float size = std::max(viewport.mB.x, viewport.mB.y);

    m_background.texture = mono::CreateTexture("textures/placeholder.png");
    m_background.repeat = 6.0f;
    m_background.width = size;
    m_background.height = size;

    AddUpdatable(std::make_shared<HealthbarUpdater>(m_healthbars, m_damage_controller, *this));
    AddUpdatable(std::make_shared<CollisionHandler>(m_event_handler, m_damage_controller, fighter, ai_fighter, m_props));

    AddDrawable(std::make_shared<StageBackgroundDrawable>(m_background), BACKGROUND);
    AddEntity(ai_fighter, MIDDLEGROUND);
    AddEntity(fighter, MIDDLEGROUND);
    AddDrawable(std::make_shared<HealthbarDrawer>(m_healthbars), FOREGROUND);
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
