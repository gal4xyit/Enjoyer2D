#include "InputHandler.hpp"

#include "editor/Editor.hpp"
#include "rendering/DebugState.hpp"

#include <Enjoyer2D/World.hpp>

#include <imgui_impl_sdl3.h>

void handleEvent(const SDL_Event& event, TestbedState& testbed, InputState& input, const View& view, bool& running) {
    // ImGui proceeds SDL window event first so editor can capture mouse/keyboard inputs
    ImGui_ImplSDL3_ProcessEvent(&event);
    const ImGuiIO& currentIo = ImGui::GetIO();

    if (event.type == SDL_EVENT_QUIT) {
        running = false;
    }

    if (!currentIo.WantCaptureKeyboard) {
        if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat) {
            switch (event.key.key) {
                case SDLK_W:
                    input.cameraUp = true;
                    break;
                case SDLK_S:
                    input.cameraDown = true;
                    break;
                case SDLK_A:
                    input.cameraLeft = true;
                    break;
                case SDLK_D:
                    input.cameraRight = true;
                    break;
                case SDLK_I:
                    input.zoomIn = true;
                    break;
                case SDLK_O:
                    input.zoomOut = true;
                    break;
                case SDLK_ESCAPE:
                    input.quitRequested = true;
                    break;
                case SDLK_SPACE:
                    testbed.sim.paused = !testbed.sim.paused;
                    break;
                case SDLK_R:
                    testbed.sim.resetRequest = true;
                    break;
                case SDLK_N:
                    testbed.sim.singleStepRequest = true;
                    break;
                case SDLK_LEFT:
                    input.sceneChange = -1;
                    break;
                case SDLK_RIGHT:
                    input.sceneChange = 1;
                    break;
                case SDLK_BACKSPACE:
                    testbed.world.destroyBody(static_cast<Enjoyer2D::BodyHandle>(testbed.editor.selectedBodyIndex));
                    testbed.editor.selectedBodyIndex = -1;
                    resetDebugTrails(testbed.debug, testbed.world);
                    break;

                default:
                    break;
            }
        }
    }

    if (event.type == SDL_EVENT_KEY_UP) {
        switch (event.key.key) {
            case SDLK_W:
                input.cameraUp = false;
                break;
            case SDLK_S:
                input.cameraDown = false;
                break;
            case SDLK_A:
                input.cameraLeft = false;
                break;
            case SDLK_D:
                input.cameraRight = false;
                break;
            case SDLK_I:
                input.zoomIn = false;
                break;
            case SDLK_O:
                input.zoomOut = false;
                break;

            default:
                break;
        }
    }

    if (!currentIo.WantCaptureMouse) {
        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT) {

            const SDL_FPoint mouseScreen {event.button.x, event.button.y};
            testbed.editor.mouseWorld = screenToWorld(mouseScreen, view);

            testbed.editor.hasMouseWorld = true;

            if (testbed.editor.activeTool == EditorTool::Select) {
                testbed.editor.selectedBodyIndex = pickBodyAtPoint(testbed.world, testbed.editor.mouseWorld);
                testbed.editor.statusMessage = testbed.editor.selectedBodyIndex != -1 ? "Body selected" : "No body selected";

            }
            if (testbed.editor.activeTool == EditorTool::SpawnBox || testbed.editor.activeTool == EditorTool::SpawnCircle) {

                const Enjoyer2D::Body preview = makePreviewBody(testbed.editor);

                if (!overlapsWorld(testbed.world, preview)) {
                    const Enjoyer2D::BodyDef def = makeSpawnBodyDef(testbed.editor);

                    testbed.editor.selectedBodyIndex = static_cast<int>(testbed.world.createBody(def));

                    testbed.editor.statusMessage = "Body spawned";
                    resetDebugTrails(testbed.debug, testbed.world);
                } else {
                    testbed.editor.statusMessage = "Spawn blocked: overlaps existing body";
                }
            }
        }

        if (event.type == SDL_EVENT_MOUSE_MOTION) {
            const SDL_FPoint mouseScreen {event.motion.x, event.motion.y};
            testbed.editor.mouseWorld = screenToWorld(mouseScreen, view);
            testbed.editor.hasMouseWorld = true;
        }
    }
}
