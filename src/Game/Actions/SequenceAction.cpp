
#include "SequenceAction.h"

void SequenceAction::AddAction(std::unique_ptr<mono::IAction> action)
{
    m_actions.push(std::move(action));
}

void SequenceAction::Update(unsigned int delta)
{
    if(m_actions.empty())
        return;

    std::unique_ptr<mono::IAction>& action = m_actions.front();
    action->Update(delta);

    if(action->Finished())
        m_actions.pop();
}

bool SequenceAction::Finished() const
{
    return m_actions.empty();
}
