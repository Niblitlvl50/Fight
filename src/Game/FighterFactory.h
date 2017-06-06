
#pragma once

#include <memory>

class Fighter;

namespace mono
{
    class EventHandler;
}

std::shared_ptr<Fighter> CreateFighter(mono::EventHandler& event_handler);
std::shared_ptr<Fighter> CreateAIFighter(mono::EventHandler& event_handler, const std::shared_ptr<Fighter>& opponent);
