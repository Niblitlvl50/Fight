
#pragma once

#include "Rendering/RenderPtrFwd.h"

struct StageBackground
{
    mono::ITexturePtr texture;
    float repeat = 1.0f;
    float width = 0.0f;
    float height = 0.0f;
};

void DrawBackground(mono::IRenderer& renderer, const StageBackground& background);
