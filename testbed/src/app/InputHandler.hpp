#pragma once

#include "InputState.hpp"
#include "TestbedState.hpp"
#include "rendering/View.hpp"

#include <SDL3/SDL.h>

void handleEvent(const SDL_Event& event, TestbedState& testbed, InputState& input, const View& view, bool& running);
