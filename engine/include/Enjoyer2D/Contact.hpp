#pragma once

#include <cstddef>

#include <glm/vec2.hpp>

namespace Enjoyer2D {
    struct Contact {
        bool collided {false};

        std::size_t bodyA {0};
        std::size_t bodyB {0};

        glm::vec2 point {0.0f, 0.0f};
        glm::vec2 normal {0.0f, 0.0f};
        float penetration {0.0f};
    };
}
