#pragma once

#include "app/TestbedState.hpp"

#include <Enjoyer2D/Body.hpp>
#include <Enjoyer2D/Shape.hpp>
#include <Enjoyer2D/World.hpp>

#include <glm/vec2.hpp>

int pickBodyAtPoint(const Enjoyer2D::World& world, const glm::vec2& point);

bool overlapsWorld(const Enjoyer2D::World& world, const Enjoyer2D::Body& candidate);

Enjoyer2D::Shape makeSpawnShape(const EditorState& editor);

Enjoyer2D::BodyDef makeSpawnBodyDef(const EditorState& editor);

Enjoyer2D::Body makePreviewBody(const EditorState& editor);
