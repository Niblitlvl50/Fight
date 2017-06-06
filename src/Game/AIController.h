
#pragma once

#include "Fighter.h"

class AIController : public IFighterController
{
public:

    AIController(const std::shared_ptr<Fighter>& opponent);
    void Initialize(Fighter* fighter) override;
    void Update(unsigned int delta) override;

private:

    std::shared_ptr<Fighter> m_opponent;
    Fighter* m_fighter = nullptr;
};
