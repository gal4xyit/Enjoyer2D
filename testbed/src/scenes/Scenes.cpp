#include "Scenes.hpp"

#include <Enjoyer2D/Body.hpp>
#include <Enjoyer2D/Shape.hpp>

#include <glm/vec2.hpp>

namespace {
    void addBox(Enjoyer2D::World& world, const glm::vec2& position, const glm::vec2& size, bool isStatic, float mass, float friction, float restitution, const glm::vec2& velocity = {0.0f, 0.0f}) {
        Enjoyer2D::BodyDef body;
        body.position = position;
        body.shape = Enjoyer2D::Shape::makeBox(size.x, size.y);
        body.isStatic = isStatic;
        body.mass = mass;
        body.friction = friction;
        body.restitution = restitution;
        body.velocity = velocity;
        world.createBody(body);
    }

    void addCircle(Enjoyer2D::World& world, const glm::vec2& position, float radius, bool isStatic, float mass, float friction, float restitution, const glm::vec2& velocity = {0.0f, 0.0f}) {
        Enjoyer2D::BodyDef body;
        body.position = position;
        body.shape = Enjoyer2D::Shape::makeCircle(radius);
        body.isStatic = isStatic;
        body.mass = mass;
        body.friction = friction;
        body.restitution = restitution;
        body.velocity = velocity;
        world.createBody(body);
    }

    // Creates box-like bounds
    void addBoxBounds(Enjoyer2D::World& world, float floorY, float halfWidth, float height, float wallThickness, float friction, float restitution) {
        addBox(world, {0.0f, floorY}, {halfWidth * 2.0f, wallThickness}, true, 0.0f, friction, restitution);
        addBox(world, {-halfWidth, floorY + height * 0.5f}, {wallThickness, height}, true, 0.0f, friction, restitution);
        addBox(world, {halfWidth, floorY + height * 0.5f}, {wallThickness, height}, true, 0.0f, friction, restitution);
    }
}

// List of built-in scenes that are educational demos for specific engine features and world objects initializations
void loadScene(Enjoyer2D::World& world, SceneId scene) {
    world.clear();

    switch (scene) {
        case SceneId::FreeFall: {
            world.setGravity({0.0f, -9.8f});
            world.setVelocityIterations(8);
            world.setPositionIterations(3);

            addBox(world, {0.0f, -2.5f}, {16.0f, 1.0f}, true, 0.0f, 0.9f, 0.1f);
            addBox(world, {-4.0f, 5.0f}, {1.0f, 1.0f}, false, 1.0f, 0.4f, 0.1f);
            addBox(world, {0.0f, 7.0f}, {1.2f, 1.2f}, false, 3.0f, 0.4f, 0.1f);
            addCircle(world, {4.0f, 9.0f}, 0.6f, false, 1.0f, 0.4f, 0.1f);
            break;
        }

        case SceneId::Bounce: {
            world.setGravity({0.0f, -9.8f});
            world.setVelocityIterations(12);
            world.setPositionIterations(4);

            addBoxBounds(world, -3.0f, 7.0f, 11.0f, 0.5f, 0.0f, 1.0f);

            addCircle(world, {-4.0f, 5.5f}, 0.55f, false, 1.0f, 0.0f, 0.1f);
            addCircle(world, {0.0f, 5.5f}, 0.55f, false, 1.0f, 0.0f, 0.55f);
            addCircle(world, {4.0f, 5.5f}, 0.55f, false, 1.0f, 0.0f, 0.9f);
            break;
        }

        case SceneId::Stacking: {
            world.setGravity({0.0f, -9.8f});
            world.setVelocityIterations(14);
            world.setPositionIterations(5);

            addBox(world, {0.0f, -2.5f}, {18.0f, 1.0f}, true, 0.0f, 0.9f, 0.0f);

            constexpr float boxSize = 0.8f;
            constexpr float gap = 0.02f;
            constexpr float spacing = boxSize + gap;
            constexpr float floorTop = -2.0f;
            constexpr int baseCount = 9;

            for (int row = 0; row < 6; ++row) {
                const int count = baseCount - row;
                const float y = floorTop + boxSize * 0.5f + static_cast<float>(row) * spacing;
                const float firstX = -static_cast<float>(count - 1) * spacing * 0.5f;

                for (int column = 0; column < count; ++column) {
                    const float x = firstX + static_cast<float>(column) * spacing;
                    addBox(world, {x, y}, {boxSize, boxSize}, false, 1.0f, 0.7f, 0.0f);
                }
            }
            break;
        }

        case SceneId::InclinedPlane: {
            world.setGravity({0.0f, -9.8f});
            world.setVelocityIterations(10);
            world.setPositionIterations(4);

            constexpr float floorHeight = 0.3f;
            constexpr float bodySize = 0.8f;
            constexpr float leftX = -5.5f;
            constexpr float rightWallX = 6.0f;
            constexpr float speed = 6.0f;
            constexpr float floorYs[] = {-2.5f, 0.0f, 2.5f};
            constexpr float frictions[] = {0.0f, 0.1f, 1.0f};

            for (int lane = 0; lane < 3; ++lane) {
                const float floorY = floorYs[lane];
                const float bodyY = floorY + floorHeight * 0.5f + bodySize * 0.5f + 0.02f;

                addBox(world, {0.0f, floorY}, {13.0f, floorHeight}, true, 0.0f, 1.0f, 0.0f);
                addBox(world, {rightWallX, bodyY}, {0.3f, bodySize * 1.4f}, true, 0.0f, 1.0f, 0.0f);
                addBox(world, {leftX, bodyY}, {bodySize, bodySize}, false, 1.0f, frictions[lane], 0.0f, {speed, 0.0f});
            }
            break;
        }

        case SceneId::Stress: {
            world.setGravity({0.0f, -9.8f});
            world.setVelocityIterations(8);
            world.setPositionIterations(3);

            addBoxBounds(world, -3.5f, 6.5f, 12.0f, 0.5f, 0.6f, 0.1f);

            constexpr int columns = 20;
            constexpr int rows = 28;
            constexpr float spacing = 0.62f;

            for (int row = 0; row < rows; ++row) {
                for (int column = 0; column < columns; ++column) {
                    const float x = -6.0f + static_cast<float>(column) * spacing;
                    const float y = 0.0f + static_cast<float>(row) * spacing;

                    if ((row + column) % 2 == 0) {
                        addCircle(world, {x, y}, 0.23f, false, 1.0f, 0.35f, 0.1f);
                    } else {
                        addBox(world, {x, y}, {0.42f, 0.42f}, false, 1.0f, 0.35f, 0.1f);
                    }
                }
            }
            break;
        }
    }
}

const char* sceneName(SceneId scene)
{
    switch (scene) {
        case SceneId::FreeFall:
            return "Free fall";
        case SceneId::Bounce:
            return "Bounce";
        case SceneId::Stacking:
            return "Stacking";
        case SceneId::InclinedPlane:
            return "Friction";
        case SceneId::Stress:
            return "Stress";
    }

    return "Unknown";
}
