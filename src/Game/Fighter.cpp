
#include "Fighter.h"
#include "Rendering/IRenderer.h"
#include "Rendering/Color.h"
#include "Rendering/Sprite/ISprite.h"
#include "Rendering/Sprite/SpriteFactory.h"

#include "Math/Quad.h"
#include "Math/Matrix.h"
#include "Math/MathFunctions.h"

#include "IAction.h"
#include "Actions/MoveAction.h"
#include "Actions/SequenceAction.h"

#include "FontIds.h"

#include <cstdio>

namespace
{
    const char* FightStateToText(FightStates state)
    {
        switch(state)
        {
            case FightStates::IDLE:
                return "Idle";
            case FightStates::HIT:
                return "Hit";
            case FightStates::WALKING:
                return "Walking";
            case FightStates::JUMPING:
                return "Jumping";
            case FightStates::ATTACKING:
                return "Attacking";
        }

        return "Unknown";
    }
}

Fighter::Fighter(IFighterController* controller)
    : m_controller(controller),
      m_invulnerable(false)
{
    m_sprite = mono::CreateSprite("sprites/fighter.sprite");
    m_scale = math::Vector(50, 50);

    m_moves = {
        { "hello", { Input::LEFT, Input::RIGHT, Input::PUNCH, Input::NONE } },
        { "apa",   { Input::LEFT, Input::UP,    Input::KICK,  Input::NONE } }
    };

    using namespace std::placeholders;

    const std::unordered_map<FightStates, FightStateMachine::State> state_table = {
        { FightStates::IDLE,      { std::bind(&Fighter::ToIdle,      this), nullptr } },
        { FightStates::HIT,       { std::bind(&Fighter::ToHit,       this), std::bind(&Fighter::HitState, this, _1) } },
        { FightStates::WALKING,   { std::bind(&Fighter::ToWalking,   this), std::bind(&Fighter::Walking, this, _1)  } },
        { FightStates::JUMPING,   { std::bind(&Fighter::ToJumping,   this), std::bind(&Fighter::Jumping, this, _1)  } },
        { FightStates::ATTACKING, { std::bind(&Fighter::ToAttacking, this), nullptr } },
    };

    m_machine.SetStateTable(state_table);
    m_machine.TransitionTo(FightStates::IDLE);

    m_controller->Initialize(this);
}

Fighter::~Fighter()
{ }

void Fighter::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(*m_sprite);

    //constexpr mono::Color::RGBA color(1, 0, 1);
    //renderer.DrawCircle(m_attack_position, 0.08f, 20, 2.0f, color);

    const char* text = FightStateToText(m_machine.ActiveState());
    renderer.DrawText(game::FontId::SMALL, text, math::Vector(0, -0.7), true, mono::Color::RGBA(1, 1, 0));
}

void Fighter::Update(unsigned int delta)
{
    m_controller->Update(delta);
    m_sprite->doUpdate(delta);
    m_machine.UpdateState(delta);
    UpdateActions(delta);
}

void Fighter::UpdateActions(unsigned int delta)
{
    const auto func = [](const std::unique_ptr<mono::IAction>& action) {
        return action->Finished();
    };

    const auto it = std::remove_if(m_actions.begin(), m_actions.end(), func);
    if(it != m_actions.end())
        m_actions.erase(it, m_actions.end());

    for(std::unique_ptr<mono::IAction>& action : m_actions)
        action->Update(delta);
}

void Fighter::Punch()
{
    if(m_machine.ActiveState() != FightStates::IDLE)
        return;

    const bool executed = ExecutePunchSpecial();
    if(executed)
        return;

    auto sequence = std::make_unique<SequenceAction>();
    sequence->AddAction(std::make_unique<MoveAction>(m_attack_position, math::Vector(0.4, 0.2), 100));
    sequence->AddAction(std::make_unique<MoveAction>(m_attack_position, math::Vector(0, 0), 100));
    m_actions.push_back(std::move(sequence));

    m_attack_animation = "punch";
    m_machine.TransitionTo(FightStates::ATTACKING);
}

void Fighter::Kick()
{
    if(m_machine.ActiveState() != FightStates::IDLE)
        return;

    auto sequence = std::make_unique<SequenceAction>();
    sequence->AddAction(std::make_unique<MoveAction>(m_attack_position, math::Vector(0.5, 0.1), 100));
    sequence->AddAction(std::make_unique<MoveAction>(m_attack_position, math::Vector(0, 0), 100));
    m_actions.push_back(std::move(sequence));

    m_attack_animation = "kick1";
    m_machine.TransitionTo(FightStates::ATTACKING);
}

void Fighter::Jump()
{
    auto sequence = std::make_unique<SequenceAction>();
    sequence->AddAction(std::make_unique<MoveAction>(m_position, m_position + math::Vector(0, 40), 100));
    sequence->AddAction(std::make_unique<MoveAction>(m_position, m_position, 100));

    m_actions.push_back(std::move(sequence));
}

void Fighter::Walk(Direction direction)
{
    m_walk_direction = direction;
    m_machine.TransitionTo(FightStates::WALKING);
}

bool Fighter::ExecutePunchSpecial()
{
    for(const FightMove& move : m_moves)
    {
    }

    return false;
}

void Fighter::ToIdle()
{
    m_sprite->SetAnimation("idle");
}

void Fighter::ToHit()
{
    m_invulnerable = true;
    m_invulnerable_counter = 500;
}

void Fighter::ToWalking()
{
    m_sprite->SetAnimation("walking");
}

void Fighter::ToJumping()
{ }

void Fighter::HitState(unsigned int delta)
{
    m_invulnerable_counter -= delta;
    m_invulnerable = (m_invulnerable_counter > 0);
    if(!m_invulnerable)
        m_machine.TransitionTo(FightStates::IDLE);
}

void Fighter::Walking(unsigned int delta)
{ 
    constexpr float mps = 0.10f;
    const float modifier = (m_walk_direction == Direction::LEFT) ? -1.0f : 1.0f;
    m_position.x += mps * delta * modifier;
}

void Fighter::Jumping(unsigned int delta)
{ }

void Fighter::ToAttacking()
{
    m_attack_position = math::zeroVec;

    auto func = std::bind(&FightStateMachine::TransitionTo, &m_machine, FightStates::IDLE);
    m_sprite->SetAnimation(m_attack_animation.c_str(), func);
}

bool Fighter::IsAttacking() const
{
    return m_machine.ActiveState() == FightStates::ATTACKING;
}

bool Fighter::IsInvulnerable() const
{
    return m_invulnerable;
}

const math::Vector Fighter::AttackPosition() const
{
    return math::Transform(Transformation(), m_attack_position);
}

std::vector<math::Quad> Fighter::HitBoxes() const
{
    math::Quad bb = BoundingBox();

    const float half_width = (bb.mB.x - bb.mA.x) / 2.0f;

    bb.mA.x = bb.mA.x + half_width - 5.0f;
    bb.mB.x = bb.mB.x - half_width + 5.0f;

    return {
        bb
    };
}

void Fighter::OnHit(const math::Vector& hit_position)
{
    math::Vector delta = m_position - hit_position;
    math::Normalize(delta);
    const math::Vector push_back = m_position + math::Vector(delta.x * 5, 0.0f);

    m_actions.push_back(std::make_unique<MoveAction>(m_position, push_back, 200));
    m_machine.TransitionTo(FightStates::HIT);
}
