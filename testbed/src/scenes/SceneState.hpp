#pragma once

#include "Scenes.hpp"

#include <Enjoyer2D/World.hpp>

#include <array>
#include <cstddef>

struct SceneState {
    SceneId current {SceneId::FreeFall};

     static constexpr std::array<SceneId, 5> availableScenes {
        SceneId::FreeFall,
        SceneId::Bounce,
        SceneId::Stacking,
        SceneId::InclinedPlane,
        SceneId::Stress
    };

    [[nodiscard]] static constexpr std::size_t count() {
        return availableScenes.size();
    }

    [[nodiscard]] static SceneId sceneAt(std::size_t index) {
        return availableScenes.at(index);
    }

    [[nodiscard]] static std::size_t indexOf(SceneId scene)
    {
        for (std::size_t i = 0; i < availableScenes.size(); ++i) {
            if (availableScenes[i] == scene) {
                return i;
            }
        }

        return 0;
    }

    [[nodiscard]] const char* name() const {
        return sceneName(current);
    }

    void load(Enjoyer2D::World& world, SceneId scene) {
        current = scene;
        loadScene(world, current);
    }

    void reset(Enjoyer2D::World& world) {
        loadScene(world, current);
    }

    void next(Enjoyer2D::World& world) {
        std::size_t index = indexOf(current);
        index = (index + 1) % availableScenes.size();

        load(world, availableScenes[index]);
    }

    void previous(Enjoyer2D::World& world) {
        std::size_t index = indexOf(current);

        if (index == 0) {
            index = availableScenes.size() - 1;
        } else {
            --index;
        }

        load(world, availableScenes[index]);
    }
};
