
#include "AIController.h"
#include "Rendering/Sprite/ISprite.h"

AIController::AIController(const std::shared_ptr<Fighter>& opponent)
    : m_opponent(opponent)
{ }

void AIController::Initialize(Fighter* fighter)
{
    m_fighter = fighter;
    m_fighter->m_sprite->SetHorizontalDirection(mono::HorizontalDirection::LEFT);
}

void AIController::Update(unsigned int delta)
{
    
}
