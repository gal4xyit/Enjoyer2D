#pragma once

#include <Enjoyer2D/World.hpp>

#include <glm/vec2.hpp>

#include <vector>
#include <cstddef>

struct BodyTrail {
    std::vector<glm::vec2> points;
};

struct DebugState {
    std::vector<BodyTrail> bodyTrails;
    std::size_t maxTrailPoints = 180;
};

inline void resetDebugTrails(DebugState& debug, const Enjoyer2D::World& world) {
    debug.bodyTrails.clear();
    debug.bodyTrails.resize(world.getBodyCount());
}

// Trails are indexed by body indexes (so they must be reset every body storage change)
inline void recordDebugTrails(DebugState& debug, const Enjoyer2D::World& world) {
    const std::vector<Enjoyer2D::Body>& bodies = world.getBodies();

    if (debug.bodyTrails.size() != bodies.size()) {
        debug.bodyTrails.resize(bodies.size());
    }

    for (std::size_t i = 0; i < bodies.size(); ++i) {
        if (bodies[i].isStatic()) {
            continue;
        }

        std::vector<glm::vec2>& points = debug.bodyTrails[i].points;
        points.push_back(bodies[i].position);

        if (points.size() > debug.maxTrailPoints) {
            points.erase(points.begin());
        }
    }
}
