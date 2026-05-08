#pragma once

#include "View.hpp"
#include "DebugRenderSettings.hpp"
#include "DebugState.hpp"


#include <Enjoyer2D/Body.hpp>
#include <Enjoyer2D/World.hpp>
#include <Enjoyer2D/Collision.hpp>
#include <Enjoyer2D/Contact.hpp>

#include <SDL3/SDL.h>
#include <glm/vec2.hpp>

#include <vector>

struct RenderContext {
    const View& view;
    const DebugRenderSettings& debugSettings;
    const DebugState& debugState;
    int selectedBodyIndex = -1;
};

SDL_FRect boxToScreenRect(const Enjoyer2D::Body& body, const View& view);

void drawCircle(SDL_Renderer* renderer, const Enjoyer2D::Body& body, const View& view);

void drawBody(SDL_Renderer* renderer, const Enjoyer2D::Body& body, const View& view);

void drawBodyCenter(SDL_Renderer* renderer, const Enjoyer2D::Body& body, const View& view);

void drawBodyPath(SDL_Renderer* renderer, const std::vector<glm::vec2>& points, const View& view);

void drawBodyVelocity(SDL_Renderer* renderer, const Enjoyer2D::Body& body, const View& view, float scale);

void drawAabb(SDL_Renderer* renderer, const Enjoyer2D::AABB& aabb, const View& view);

void drawContactPoint(SDL_Renderer* renderer, const Enjoyer2D::Contact& contact, const View& view);

void drawContactNormal(SDL_Renderer* renderer, const Enjoyer2D::Contact& contact, const View& view, float scale);

void drawWorld(SDL_Renderer* renderer, const Enjoyer2D::World& world, const RenderContext& context);
