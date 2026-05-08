#include "TestbedActions.hpp"

void resetTestbedSceneState(TestbedState& testbed) {
    resetDebugTrails(testbed.debug, testbed.world);

    testbed.sim.accumulator = 0.0f;
    testbed.sim.singleStepRequest = false;
    testbed.sim.resetRequest = false;

    testbed.stats.physicsStepMs = 0.0f;
    testbed.stats.physicsStepsThisFrame = 0;

    testbed.editor.selectedBodyIndex = -1;
    testbed.editor.activeTool = EditorTool::Select;
}

void loadTestbedScene(TestbedState& testbed, SceneId scene) {
    testbed.scene.load(testbed.world, scene);
    resetTestbedSceneState(testbed);
}

void resetCurrentTestbedScene(TestbedState& testbed) {
    testbed.scene.reset(testbed.world);
    resetTestbedSceneState(testbed);
}