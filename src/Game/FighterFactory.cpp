
#include "FighterFactory.h"
#include "Fighter.h"

#include "PlayerController.h"
#include "AIController.h"

std::shared_ptr<Fighter> CreateFighter(mono::EventHandler& event_handler)
{
    return std::make_shared<Fighter>(new PlayerController(event_handler));
}

std::shared_ptr<Fighter> CreateAIFighter(mono::EventHandler& event_handler, const std::shared_ptr<Fighter>& opponent)
{
    return std::make_shared<Fighter>(new AIController(opponent));
}
