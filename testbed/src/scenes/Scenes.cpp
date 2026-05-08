#include "Scenes.hpp"

#include <Enjoyer2D/Body.hpp>
#include <Enjoyer2D/Shape.hpp>

void loadScene(Enjoyer2D::World& world, SceneId scene)
{
    world.clear();

    switch (scene) {
        case SceneId::FreeFall: {
            world.setGravity({0.0f, -9.8f});

            Enjoyer2D::BodyDef floor;
            floor.position = {0.0f, -2.0f};
            floor.shape = Enjoyer2D::Shape::makeBox(14.0f, 1.0f);
            floor.isStatic = true;
            floor.friction = 1.0f;
            floor.restitution = 0.2f;
            world.createBody(floor);

            Enjoyer2D::BodyDef box;
            box.position = {-1.5f, 4.0f};
            box.shape = Enjoyer2D::Shape::makeBox(1.0f, 1.0f);
            box.mass = 1.0f;
            box.friction = 0.4f;
            box.restitution = 0.2f;
            box.velocity = {2.0f, 0.0f};
            world.createBody(box);

            Enjoyer2D::BodyDef circle;
            circle.position = {1.5f, 5.5f};
            circle.shape = Enjoyer2D::Shape::makeCircle(0.5f);
            circle.mass = 1.0f;
            circle.friction = 0.4f;
            circle.restitution = 0.6f;
            world.createBody(circle);
            break;
        }

        case SceneId::Bounce: {
            // placeholder for now
            break;
        }

        case SceneId::Stacking: {
            // placeholder for now
            break;
        }

        case SceneId::InclinedPlane: {
            world.setGravity({0.0f, -9.8f});

            Enjoyer2D::BodyDef floor;
            floor.position = {0.0f, -2.0f};
            floor.shape = Enjoyer2D::Shape::makeBox(20.0f, 1.0f);
            floor.isStatic = true;
            floor.friction = 0.3f;
            floor.restitution = 0.0f;
            world.createBody(floor);

            Enjoyer2D::BodyDef slipperyBox;
            slipperyBox.position = {-7.0f, 3.0f};
            slipperyBox.velocity = {5.0f, 0.0f};
            slipperyBox.shape = Enjoyer2D::Shape::makeBox(1.0f, 1.0f);
            slipperyBox.mass = 1.0f;
            slipperyBox.friction = 0.0f;
            slipperyBox.restitution = 0.0f;
            world.createBody(slipperyBox);

            Enjoyer2D::BodyDef roughBox;
            roughBox.position = {-10.0f, 3.0f};
            roughBox.velocity = {5.0f, 0.0f};
            roughBox.shape = Enjoyer2D::Shape::makeBox(1.0f, 1.0f);
            roughBox.mass = 1.0f;
            roughBox.friction = 0.2f;
            roughBox.restitution = 0.0f;
            world.createBody(roughBox);

            break;
        }

        case SceneId::Stress: {
            // placeholder for now
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
            return "Inclined plane";
        case SceneId::Stress:
            return "Stress";
    }

    return "Unknown";
}
