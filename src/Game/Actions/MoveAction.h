
#pragma once

#include "IAction.h"
#include "Math/Vector.h"

#include <functional>

class MoveAction : public mono::IAction
{
public:
    
    MoveAction(math::Vector& source, const math::Vector& target, unsigned int duration);

private:

    virtual void Prepare();
    virtual void Update(unsigned int delta);
    virtual bool Finished() const;
    
    math::Vector& m_source;
    const math::Vector m_target;
    const unsigned int m_duration;

    math::Vector m_start;
    math::Vector m_vector;


    unsigned int m_counter = 0;
    bool m_prepared = false;
};
