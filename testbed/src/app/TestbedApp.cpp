#include "TestbedApp.hpp"

#include "editor/Editor.hpp"
#include "InputHandler.hpp"
#include "rendering/Renderer.hpp"
#include "scenes/Scenes.hpp"
#include "TestbedActions.hpp"
#include "editor/TestbedUi.hpp"
#include "TestbedUpdate.hpp"
#include "scenes/SceneFiles.hpp"

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>

#include <iostream>

bool TestbedApp::init() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
        return false;
    }

    if (!SDL_CreateWindowAndRenderer("Enjoyer2D Testbed", 1280, 720, 0, &window, &renderer)) {
        std::cerr << "SDL_CreateWindowAndRenderer failed: " << SDL_GetError() << '\n';
        SDL_Quit();
        return false;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    loadTestbedScene(testbed, SceneId::FreeFall);
    refreshSceneFiles(testbed.editor.sceneFiles);

    previousCounter = SDL_GetPerformanceCounter();
    frequency = static_cast<double>(SDL_GetPerformanceFrequency());

    running = true;

    return true;
}

TestbedApp::~TestbedApp() {
    shutdown();
}

void TestbedApp::shutdown() {
    if (renderer != nullptr) {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();

        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window != nullptr) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}

void TestbedApp::run() {
    while (running) {
        handleEvents();
        updateFrame();
        renderFrame();
    }
}

void TestbedApp::handleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        handleEvent(event, testbed, input, view, running);
    }
}

// SDL performance counters provide high resolution frame timings
void TestbedApp::updateFrame() {
    const Uint64 currentCounter = SDL_GetPerformanceCounter();

    const float frameDt = static_cast<float>(static_cast<double>(currentCounter - previousCounter) / frequency);

    testbed.stats.frameDt = frameDt;
    previousCounter = currentCounter;

    const Uint64 physicsStart = SDL_GetPerformanceCounter();

    updateSimulation(testbed, frameDt);

    const Uint64 physicsEnd = SDL_GetPerformanceCounter();

    testbed.stats.physicsStepMs = static_cast<float>(physicsEnd - physicsStart) * 1000.0f / static_cast<float>(frequency);

    processFrameRequests(testbed, input, running);
    updateCamera(view, input, testbed, frameDt);
}

void TestbedApp::renderFrame() {
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    drawTestbedUi(testbed, debugRender);

    SDL_SetRenderDrawColor(renderer, 18, 18, 24, 255);
    SDL_RenderClear(renderer);

    const RenderContext renderContext {view, debugRender, testbed.debug, testbed.editor.selectedBodyIndex};

    drawWorld(renderer, testbed.world, renderContext);

    // Overlay object helper to show look of spawned object
    if (testbed.editor.showSpawnPreview && testbed.editor.hasMouseWorld && testbed.editor.activeTool != EditorTool::Select) {

        const Enjoyer2D::Body preview = makePreviewBody(testbed.editor);
        const bool blocked = overlapsWorld(testbed.world, preview);

        if (blocked) {
            SDL_SetRenderDrawColor(renderer, 255, 80, 160, 150);
        } else {
            SDL_SetRenderDrawColor(renderer, 80, 255, 160, 150);
        }

        drawBody(renderer, preview, view);
    }

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

    SDL_RenderPresent(renderer);
}
