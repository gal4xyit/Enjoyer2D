#pragma once

#include <cstddef>

namespace Enjoyer2D {

    struct WorldStats {
        std::size_t bodyCount {0};
        std::size_t contactCount {0};

        int velocityIterations {0};
        int positionIterations {0};
    };
}
