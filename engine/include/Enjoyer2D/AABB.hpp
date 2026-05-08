#pragma once

#include <glm/vec2.hpp>

namespace Enjoyer2D {
    struct AABB {
        glm::vec2 min {0.0f, 0.0f};
        glm::vec2 max {0.0f, 0.0f};

        [[nodiscard]] float width() const {
            return max.x - min.x;
        }

        [[nodiscard]] float height() const {
            return max.y - min.y;
        }

        [[nodiscard]] glm::vec2 center() const {
            return (min + max) * 0.5f;
        }
    };
}
