
#pragma once

#include "Fighter.h"
#include "Moves/FightMove.h"

#include "Events/EventFwd.h"
#include "EventHandler/EventToken.h"

class PlayerController : public IFighterController
{
public:

    PlayerController(mono::EventHandler& event_handler);
    ~PlayerController();

    void Initialize(Fighter* fighter) override;
    void Update(unsigned int delta) override;

private:

    bool OnKeyDown(const event::KeyDownEvent& event);
    bool OnKeyUp(const event::KeyUpEvent& event);

    mono::EventHandler& m_event_handler;
    Fighter* m_fighter;

    mono::EventToken<event::KeyUpEvent> m_key_up_token;
    mono::EventToken<event::KeyDownEvent> m_key_down_token;

    std::vector<InputData> m_input_buffer;
};
