
#include "PlayerController.h"

#include "EventHandler/EventHandler.h"
#include "Events/KeyEvent.h"
#include "Events/EventFuncFwd.h"

#include "System/Keycodes.h"

PlayerController::PlayerController(mono::EventHandler& event_handler)
    : m_event_handler(event_handler),
      m_fighter(nullptr)
{
    using namespace std::placeholders;
    const event::KeyUpEventFunc& key_up_func = std::bind(&PlayerController::OnKeyUp, this, _1);
    const event::KeyDownEventFunc& key_down_func = std::bind(&PlayerController::OnKeyDown, this, _1);

    m_key_up_token = event_handler.AddListener(key_up_func);
    m_key_down_token = event_handler.AddListener(key_down_func);
}

PlayerController::~PlayerController()
{
    m_event_handler.RemoveListener(m_key_up_token);
    m_event_handler.RemoveListener(m_key_down_token);
}

void PlayerController::Initialize(Fighter* fighter)
{
    m_fighter = fighter;
}

void PlayerController::Update(unsigned int delta)
{
//    for(InputData& input : m_input_buffer)
//        input.input_time -= delta;
//
//    const auto func = [](const InputData& input_data) {
//        return input_data.input_time <= 0;
//    };
//
//    const auto it = std::remove_if(m_input_buffer.begin(), m_input_buffer.end(), func);
//    if(it != m_input_buffer.end())
//        m_input_buffer.erase(it, m_input_buffer.end());
}

bool PlayerController::OnKeyDown(const event::KeyDownEvent& event)
{
    InputData input;

    switch(event.key)
    {
        case Keycode::UP:
            input.input_key = Input::UP;
            break;
        case Keycode::DOWN:
            input.input_key = Input::DOWN;
            break;
        case Keycode::LEFT:
            input.input_key = Input::LEFT;
            m_fighter->Walk(Direction::LEFT);
            break;
        case Keycode::RIGHT:
            input.input_key = Input::RIGHT;
            m_fighter->Walk(Direction::RIGHT);
            break;
        default:
            break;
    }

    if(input.input_key != Input::NONE)
        m_input_buffer.push_back(input);

    return false;
}

bool PlayerController::OnKeyUp(const event::KeyUpEvent& event)
{
    switch(event.key)
    {
        case Keycode::Q:
            m_fighter->Punch();
            break;
        case Keycode::W:
            m_fighter->Kick();
            break;
        case Keycode::LEFT:
        case Keycode::RIGHT:
            m_fighter->m_machine.TransitionTo(FightStates::IDLE);
            break;
        case Keycode::SPACE:
            m_fighter->Jump();
            break;
        default:
            break;
     }

    return false;
}