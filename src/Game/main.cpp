
#include "System/System.h"
#include "Audio/AudioSystem.h"
#include "Rendering/RenderSystem.h"

#include "Camera/TraceCamera.h"
#include "Rendering/Text/TextFunctions.h"
#include "EventHandler/EventHandler.h"
#include "Engine.h"

#include "FightZone.h"
#include "FontIds.h"

int main(int argc, char* argv[])
{
    System::Initialize();
    mono::InitializeAudio();
    mono::InitializeRender();

    {
        // The "global" event handler used throughout the game
        mono::EventHandler eventHandler;

        const math::Vector size(1280, 800);

        System::IWindow* window = System::CreateWindow("Fight me!", size.x, size.y, false);
        window->SetBackgroundColor(0.6, 0.6, 0.6);

        mono::ICameraPtr camera = std::make_shared<mono::TraceCamera>(320, 200);
        
        mono::LoadFont(game::FontId::SMALL,  "pixelette.ttf", 10.0f, 1.0f / 30.0f);
        mono::LoadFont(game::FontId::MEDIUM, "pixelette.ttf", 10.0f, 1.0f / 20.0f);
        mono::LoadFont(game::FontId::LARGE,  "pixelette.ttf", 10.0f, 1.0f / 10.0f);

        mono::Engine engine(window, camera, eventHandler);
        engine.Run(std::make_shared<FightZone>(eventHandler));

        delete window;
    }

    mono::ShutdownRender();
    mono::ShutdownAudio();
    System::Shutdown();

    return 0;
}

