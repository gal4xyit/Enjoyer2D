#pragma once

#include "TestbedState.hpp"
#include "scenes/Scenes.hpp"

void resetTestbedSceneState(TestbedState& testbed);

void loadTestbedScene(TestbedState& testbed, SceneId scene);

void resetCurrentTestbedScene(TestbedState& testbed);
