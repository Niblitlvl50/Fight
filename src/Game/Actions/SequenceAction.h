
#pragma once

#include "IAction.h"
#include <queue>

class SequenceAction : public mono::IAction
{
public:

    void AddAction(std::unique_ptr<mono::IAction> action);

private:

    virtual void Update(unsigned int delta);
    virtual bool Finished() const;

    std::queue<std::unique_ptr<mono::IAction>> m_actions;
};
