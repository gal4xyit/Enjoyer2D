#pragma once

#include "rendering/DebugRenderSettings.hpp"
#include "InputState.hpp"
#include "TestbedState.hpp"
#include "rendering/View.hpp"

#include <SDL3/SDL.h>

class TestbedApp {
public:
    TestbedApp() = default;
    ~TestbedApp();

    bool init();
    void run();

private:
    void handleEvents();
    void updateFrame();
    void renderFrame();
    void shutdown();

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    View view;
    TestbedState testbed;
    InputState input;
    DebugRenderSettings debugRender;

    bool running = false;

    Uint64 previousCounter = 0;
    double frequency = 0.0;
};
