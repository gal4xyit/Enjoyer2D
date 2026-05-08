#pragma once

#include <Enjoyer2D/World.hpp>

enum class SceneId {
    FreeFall,
    Bounce,
    Stacking,
    InclinedPlane,
    Stress
};

const char* sceneName(SceneId scene);

void loadScene(Enjoyer2D::World& world, SceneId scene);
