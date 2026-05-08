#include "TestbedUpdate.hpp"
#include "TestbedActions.hpp"

void updateCamera(View& view, const InputState& input, const TestbedState& testbed, float dt) {
    if (input.cameraUp) {
        view.cameraY += testbed.controls.cameraSpeed * dt;
    }

    if (input.cameraDown) {
        view.cameraY -= testbed.controls.cameraSpeed * dt;
    }

    if (input.cameraLeft) {
        view.cameraX -= testbed.controls.cameraSpeed * dt;
    }

    if (input.cameraRight) {
        view.cameraX += testbed.controls.cameraSpeed * dt;
    }

    if (input.zoomIn) {
        view.pixelsPerUnit += testbed.controls.zoomSpeed * dt;
    }

    if (input.zoomOut) {
        view.pixelsPerUnit -= testbed.controls.zoomSpeed * dt;
    }

    if (view.pixelsPerUnit < testbed.controls.minZoom) {
        view.pixelsPerUnit = testbed.controls.minZoom;
    }

    if (view.pixelsPerUnit > testbed.controls.maxZoom) {
        view.pixelsPerUnit = testbed.controls.maxZoom;
    }
}

void updateSimulation(TestbedState& testbed, float frameDt) {
    testbed.stats.physicsStepsThisFrame = 0;

    if (!testbed.sim.paused) {
        testbed.sim.accumulator += frameDt;

        while (testbed.sim.accumulator >= testbed.sim.fixedDt) {
            testbed.world.step(testbed.sim.fixedDt);
            recordDebugTrails(testbed.debug, testbed.world);
            testbed.sim.accumulator -= testbed.sim.fixedDt;
            ++testbed.stats.physicsStepsThisFrame;
        }
    }

    if (testbed.sim.singleStepRequest) {
        if (testbed.sim.paused) {
            testbed.world.step(testbed.sim.fixedDt);
            recordDebugTrails(testbed.debug, testbed.world);
            ++testbed.stats.physicsStepsThisFrame;
        }

        testbed.sim.singleStepRequest = false;
    }
}

void processFrameRequests(TestbedState& testbed, InputState& input, bool& running) {
    if (input.sceneChange != 0) {
        if (input.sceneChange < 0) {
            testbed.scene.previous(testbed.world);
            resetTestbedSceneState(testbed);
        }

        if (input.sceneChange > 0) {
            testbed.scene.next(testbed.world);
            resetTestbedSceneState(testbed);
        }

        input.sceneChange = 0;
    }

    if (input.quitRequested) {
        running = false;
    }

    if (testbed.sim.resetRequest) {
        resetCurrentTestbedScene(testbed);
    }
}


