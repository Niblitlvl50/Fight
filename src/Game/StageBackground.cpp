
#include "StageBackground.h"
#include "Rendering/IRenderer.h"
#include "Math/Vector.h"

void DrawBackground(mono::IRenderer& renderer, const StageBackground& background)
{
    const std::vector<math::Vector> vertices = {
        math::Vector(0.0f, 0.0f),
        math::Vector(0.0f, background.height),
        math::Vector(background.width, background.height),
        math::Vector(background.width, 0.0f)
    };

    const std::vector<math::Vector> texture_coords = {
        math::Vector(0.0f, 0.0f),
        math::Vector(0.0f, -1.0f * background.repeat),
        math::Vector(1.0f * background.repeat, -1.0f * background.repeat),
        math::Vector(1.0f * background.repeat, 0.0f)
    };

    const std::vector<unsigned short> indices = {
        0, 1, 2,
        0, 2, 3
    };

    renderer.DrawGeometry(vertices, texture_coords, indices, background.texture);
}
