
#pragma once

enum class Input
{
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    PUNCH,
    KICK
};

struct InputData
{
    Input input_key = Input::NONE;
    int input_time = 500;
};

struct FightMove
{
    const char* name = nullptr;
    Input sequence[4];
};
