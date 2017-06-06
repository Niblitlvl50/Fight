
#include "MoveAction.h"
#include <algorithm>

MoveAction::MoveAction(math::Vector& source, const math::Vector& target, unsigned int duration)
    : m_source(source),
      m_target(target),
      m_duration(duration)
{ }

void MoveAction::Prepare()
{
    m_start = m_source;
    m_vector = m_target - m_source;
    m_prepared = true;
}

void MoveAction::Update(unsigned int delta)
{
    if(!m_prepared)
        Prepare();

    m_counter += delta;
    m_counter = std::min(m_counter, m_duration);
    const float percent = float(m_counter) / float(m_duration);
    m_source = m_start + (m_vector * percent);
}

bool MoveAction::Finished() const
{
    return (m_counter >= m_duration);
}
