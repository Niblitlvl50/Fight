
#include "Fighter.h"
#include "Rendering/IRenderer.h"
#include "Rendering/Color.h"
#include "Rendering/Sprite/ISprite.h"
#include "Rendering/Sprite/SpriteFactory.h"

#include "Math/Quad.h"
#include "Math/Matrix.h"

#include "IAction.h"
#include "Actions/MoveAction.h"
#include "Actions/SequenceAction.h"

#include "FontIds.h"

namespace
{
    const char* FightStateToText(FightStates state)
    {
        switch(state)
        {
            case FightStates::IDLE:
                return "Idle";
            case FightStates::WALK_LEFT:
                return "Walking Left";
            case FightStates::WALK_RIGHT:
                return "Walking Right";
            case FightStates::PUNCHING:
                return "Punching";
            case FightStates::KICKING:
                return "Kicking";
        }

        return "Unknown";
    }
}

Fighter::Fighter(IFighterController* controller)
    : m_controller(controller),
      m_left(false),
      m_right(false)
{
    m_sprite = mono::CreateSprite("sprites/fighter.sprite");
    mScale = math::Vector(50, 50);

    m_moves = {
        { "hello", { Input::LEFT, Input::RIGHT, Input::PUNCH, Input::NONE } },
        { "apa",   { Input::LEFT, Input::UP,    Input::KICK,  Input::NONE } }
    };

    using namespace std::placeholders;

    const std::unordered_map<FightStates, FightStateMachine::State> state_table = {
        { FightStates::IDLE,       { std::bind(&Fighter::ToIdle, this),     nullptr } },
        { FightStates::WALK_LEFT,  { std::bind(&Fighter::ToWalking, this),  std::bind(&Fighter::WalkLeft, this, _1)  } },
        { FightStates::WALK_RIGHT, { std::bind(&Fighter::ToWalking, this),  std::bind(&Fighter::WalkRight, this, _1) } },
        { FightStates::PUNCHING,   { std::bind(&Fighter::ToPunching, this), nullptr } },
        { FightStates::KICKING,    { std::bind(&Fighter::ToKicking, this),  nullptr } },
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

    constexpr mono::Color::RGBA punch_color(1, 0, 1);
    constexpr mono::Color::RGBA kick_color(1, 1, 0);
    renderer.DrawCircle(m_punch_position, 0.08f, 20, 2.0f, punch_color);
    renderer.DrawCircle(m_kick_position, 0.08f, 20, 2.0f, kick_color);

    //const char* text = FightStateToText(m_machine.ActiveState());
    //renderer.DrawText(game::FontId::SMALL, text, math::Vector(0, 1), true, mono::Color::RGBA());
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

    m_machine.TransitionTo(FightStates::PUNCHING);
}

void Fighter::Kick()
{
    if(m_machine.ActiveState() != FightStates::IDLE)
        return;

    m_machine.TransitionTo(FightStates::KICKING);
}

void Fighter::Jump()
{
    //auto sequence = std::make_unique<SequenceAction>();
    //sequence->AddAction(std::make_unique<MoveAction>(m_kick_position, math::Vector(0, 1), 50));
    //sequence->AddAction(std::make_unique<MoveAction>(m_kick_position, math::Vector(0, 0), 100));

    //m_actions.push_back(std::move(sequence));
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

void Fighter::ToWalking()
{
    m_sprite->SetAnimation("walking");
}

void Fighter::WalkLeft(float delta)
{ 
    constexpr float mps = 0.10f;
    mPosition.x -= mps * delta;
}

void Fighter::WalkRight(float delta)
{
    constexpr float mps = 0.10f;
    mPosition.x += mps * delta;
}

void Fighter::ToPunching()
{
    auto sequence = std::make_unique<SequenceAction>();
    sequence->AddAction(std::make_unique<MoveAction>(m_punch_position, math::Vector(0.4, 0.2), 100));
    sequence->AddAction(std::make_unique<MoveAction>(m_punch_position, math::Vector(0, 0), 100));
    m_actions.push_back(std::move(sequence));

    auto func = std::bind(&FightStateMachine::TransitionTo, &m_machine, FightStates::IDLE);
    m_sprite->SetAnimation("punch", func);
}

void Fighter::ToKicking()
{
    auto sequence = std::make_unique<SequenceAction>();
    sequence->AddAction(std::make_unique<MoveAction>(m_kick_position, math::Vector(0.5, 0.1), 100));
    sequence->AddAction(std::make_unique<MoveAction>(m_kick_position, math::Vector(0, 0), 100));
    m_actions.push_back(std::move(sequence));

    auto func = std::bind(&FightStateMachine::TransitionTo, &m_machine, FightStates::IDLE);
    m_sprite->SetAnimation("kick1", func);
}

const math::Vector Fighter::PunchPosition() const
{
    return math::Transform(Transformation(), m_punch_position);
}

const math::Vector Fighter::KickPosition() const
{
    return math::Transform(Transformation(), m_kick_position);
}

