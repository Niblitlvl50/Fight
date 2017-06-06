
#include "ShuttleController.h"
#include "EventHandler/EventHandler.h"
#include "Events/GameEventFuncFwd.h"
#include "Events/EventFuncFwd.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"
#include "Events/TimeScaleEvent.h"
#include "System/Keycodes.h"
#include "Math/MathFunctions.h"


using namespace game;

ShuttleController::ShuttleController(mono::EventHandler& eventHandler)
    : mEventHandler(eventHandler),
      mMouseDown(false)
{
    using namespace std::placeholders;

    const event::MouseUpEventFunc mouseUpFunc = std::bind(&ShuttleController::OnMouseUp, this, _1);
    const event::MouseDownEventFunc mouseDownFunc = std::bind(&ShuttleController::OnMouseDown, this, _1);
    const event::MouseMotionEventFunc mouseMotionFunc = std::bind(&ShuttleController::OnMouseMotion, this, _1);
    const event::KeyDownEventFunc keyDownFunc = std::bind(&ShuttleController::OnKeyDown, this, _1);
    const event::KeyUpEventFunc key_up_func = std::bind(&ShuttleController::OnKeyUp, this, _1);

    mMouseUpToken = mEventHandler.AddListener(mouseUpFunc);
    mMouseDownToken = mEventHandler.AddListener(mouseDownFunc);
    mMouseMotionToken = mEventHandler.AddListener(mouseMotionFunc);
    mKeyDownToken = mEventHandler.AddListener(keyDownFunc);
    m_keyUpToken = mEventHandler.AddListener(key_up_func);
}

ShuttleController::~ShuttleController()
{
    mEventHandler.RemoveListener(mMouseUpToken);
    mEventHandler.RemoveListener(mMouseDownToken);
    mEventHandler.RemoveListener(mMouseMotionToken);
    mEventHandler.RemoveListener(mKeyDownToken);
    mEventHandler.RemoveListener(m_keyUpToken);
}

bool ShuttleController::OnMouseDown(const event::MouseDownEvent& event)
{
    mMouseDownPosition = math::Vector(event.screenX, -event.screenY);
    mMouseDown = true;

    return true;
}

bool ShuttleController::OnMouseUp(const event::MouseUpEvent& event)
{
    mMouseDown = false;

    return true;
}

bool ShuttleController::OnMouseMotion(const event::MouseMotionEvent& event)
{
    if(!mMouseDown)
        return false;

    return false;
}

bool ShuttleController::OnKeyDown(const event::KeyDownEvent& event)
{
    if(event.key == Keycode::SPACE)
        mEventHandler.DispatchEvent(event::TimeScaleEvent(0.5f));

    return true;
}

bool ShuttleController::OnKeyUp(const event::KeyUpEvent& event)
{
    if(event.key != Keycode::SPACE)
        return false;

    mEventHandler.DispatchEvent(event::TimeScaleEvent(1.0f));
    return true;
}
