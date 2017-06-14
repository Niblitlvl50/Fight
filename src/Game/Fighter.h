
#pragma once

#include "Entity/EntityBase.h"
#include "Rendering/RenderPtrFwd.h"
#include "Moves/FightMove.h"
#include "StateMachine.h"

#include <string>

enum class FightStates
{
    IDLE,
    HIT,
    WALKING,
    JUMPING,
    ATTACKING
};

enum class Direction
{
    LEFT,
    RIGHT
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
    void Walk(Direction direction);

    bool ExecutePunchSpecial();

    // Transisions
    void ToIdle();
    void ToHit();
    void ToWalking();
    void ToJumping();
    void ToAttacking();

    // States
    void HitState(unsigned int delta);
    void Walking(unsigned int delta);
    void Jumping(unsigned int delta);

    bool IsAttacking() const;
    bool IsInvulnerable() const;
    const math::Vector AttackPosition() const;
    std::vector<math::Quad> HitBoxes() const;

    void OnHit(const math::Vector& hit_position);

    std::unique_ptr<IFighterController> m_controller;

    bool m_invulnerable;
    int m_invulnerable_counter;
    Direction m_walk_direction;

    mono::ISpritePtr m_sprite;

    math::Vector m_attack_position;
    std::string m_attack_animation;

    std::vector<FightMove> m_moves;
    std::vector<std::unique_ptr<mono::IAction>> m_actions;

    using FightStateMachine = StateMachine<FightStates, unsigned int>;
    FightStateMachine m_machine;
};
