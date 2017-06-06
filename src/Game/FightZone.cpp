
#include "FightZone.h"

#include "Math/Vector.h"
#include "Math/MathFunctions.h"

#include "Rendering/ICamera.h"
#include "Rendering/IDrawable.h"
#include "Rendering/IRenderer.h"
#include "Rendering/Color.h"

#include "Fighter.h"
#include "FighterFactory.h"

#include <cstdio>

namespace
{
    class PropDrawer : public mono::IDrawable
    {
    public:

        PropDrawer(const std::vector<math::Quad>& props)
            : m_props(props)
        { }

        void doDraw(mono::IRenderer& renderer) const
        {
            constexpr mono::Color::RGBA color(1, 0, 0);

            for(const math::Quad& prop : m_props)
                renderer.DrawQuad(prop, color, 2.0f);
        }

        math::Quad BoundingBox() const
        {
            return math::Quad(-math::INF, -math::INF, math::INF, math::INF);
        }

        const std::vector<math::Quad>& m_props;
    };

    class CollisionDetector : public mono::IUpdatable
    {
    public:

        CollisionDetector(const std::shared_ptr<Fighter>& fighter, const std::vector<math::Quad>& props)
            : m_fighter(fighter),
              m_props(props)
        { }

        void doUpdate(unsigned int delta)
        {
            const math::Vector& punch = m_fighter->PunchPosition();
            const math::Vector& kick = m_fighter->KickPosition();

            for(const math::Quad& prop : m_props)
            {
                const bool punch_hit = math::PointInsideQuad(punch, prop);
                if(punch_hit)
                    std::printf("Punch hit!\n");

                const bool kick_hit = math::PointInsideQuad(kick, prop);
                if(kick_hit)
                    std::printf("Kick hit!\n");
            }
        }
        
        const std::shared_ptr<Fighter> m_fighter;
        const std::vector<math::Quad>& m_props;
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
}

FightZone::~FightZone()
{ }

void FightZone::OnLoad(mono::ICameraPtr camera)
{
    auto fighter = CreateFighter(m_event_handler);
    fighter->SetPosition(math::Vector(100, 100));

    auto ai_fighter = CreateAIFighter(m_event_handler, fighter);
    ai_fighter->SetPosition(math::Vector(250, 100));

    AddEntity(ai_fighter, 1);
    AddEntity(fighter, 1);

    AddDrawable(std::make_shared<PropDrawer>(m_props), 0);
    //AddUpdatable(std::make_shared<CollisionDetector>(fighter, m_props));
}

void FightZone::OnUnload()
{ }
