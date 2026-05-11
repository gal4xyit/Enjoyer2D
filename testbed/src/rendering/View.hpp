#pragma once

#include <SDL3/SDL.h>
#include <glm/vec2.hpp>

struct View {
    float cameraX {0.0f};
    float cameraY {2.0f};
    float pixelsPerUnit {60.0f};
    float screenWidth {1280.0f};
    float screenHeight {720.0f};
};

// Converts world's coordinates to screen ones (world use Y up, screen use Y down)
inline SDL_FPoint worldToScreen(const glm::vec2& point, const View& view) {
    return {
        (point.x - view.cameraX) * view.pixelsPerUnit + view.screenWidth * 0.5f,
        view.screenHeight * 0.5f - (point.y - view.cameraY) * view.pixelsPerUnit
    };
}

inline glm::vec2 screenToWorld(const SDL_FPoint& point, const View& view) {
    return {
        (point.x - view.screenWidth * 0.5f) / view.pixelsPerUnit + view.cameraX,
        (view.screenHeight * 0.5f - point.y) / view.pixelsPerUnit + view.cameraY
    };
}

