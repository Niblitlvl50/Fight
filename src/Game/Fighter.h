
#pragma once

#include "Entity/EntityBase.h"
#include "Rendering/RenderPtrFwd.h"
#include "Moves/FightMove.h"
#include "StateMachine.h"

enum class FightStates
{
    IDLE,
    WALK_LEFT,
    WALK_RIGHT,
    PUNCHING,
    KICKING
};

class IFighterController
{
public:

    virtual ~IFighterController()
    { }

    virtual void Initialize(class Fighter* fighter) = 0;
    virtual void Update(unsigned int delta) = 0;
};

class Fighter : public mono::EntityBase
{
public:

    Fighter(IFighterController* controller);
    ~Fighter();

    virtual void Draw(mono::IRenderer& renderer) const;
    virtual void Update(unsigned int delta);
    void UpdateActions(unsigned int delta);

    void Punch();
    void Kick();
    void Jump();

    bool ExecutePunchSpecial();

    // Transisions
    void ToIdle();
    void ToWalking();
    void ToPunching();
    void ToKicking();

    // States
    void WalkLeft(float delta);
    void WalkRight(float delta);

    const math::Vector PunchPosition() const;
    const math::Vector KickPosition() const;

    std::unique_ptr<IFighterController> m_controller;

    bool m_left;
    bool m_right;

    mono::ISpritePtr m_sprite;

    math::Vector m_punch_position;
    math::Vector m_kick_position;

    std::vector<FightMove> m_moves;
    std::vector<std::unique_ptr<mono::IAction>> m_actions;

    using FightStateMachine = StateMachine<FightStates, unsigned int>;
    FightStateMachine m_machine;
};
