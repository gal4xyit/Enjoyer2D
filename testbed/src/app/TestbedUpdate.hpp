#pragma once

#include "InputState.hpp"
#include "TestbedState.hpp"
#include "rendering/View.hpp"

void updateCamera(View& view, const InputState& input, const TestbedState& testbed, float dt);

void updateSimulation(TestbedState& testbed, float frameDt);

void processFrameRequests(TestbedState& testbed, InputState& input, bool& running);
