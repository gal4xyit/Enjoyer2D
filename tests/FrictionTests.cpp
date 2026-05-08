#include <Enjoyer2D/Shape.hpp>
#include <Enjoyer2D/World.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Friction slows horizontal sliding on static floor") {
    Enjoyer2D::World world;
    world.setGravity({0.0f, 0.0f});

    const Enjoyer2D::BodyHandle floorHandle = world.createBody();
    Enjoyer2D::Body& floor = world.getBody(floorHandle);
    floor.position = {0.0f, 0.0f};
    floor.shape = Enjoyer2D::Shape::makeBox(10.0f, 1.0f);
    floor.setStatic();
    floor.friction = 1.0f;
    floor.restitution = 0.0f;

    const Enjoyer2D::BodyHandle boxHandle = world.createBody();
    Enjoyer2D::Body& box = world.getBody(boxHandle);
    box.position = {0.0f, 0.75f};
    box.velocity = {5.0f, -1.0f};
    box.shape = Enjoyer2D::Shape::makeBox(1.0f, 1.0f);
    box.setMass(1.0f);
    box.friction = 1.0f;
    box.restitution = 0.0f;

    world.step(1.0f / 60.0f);

    REQUIRE(world.getBody(boxHandle).velocity.x < 5.0f);
}

TEST_CASE("Zero friction doesnt slow horizontal sliding on static floor") {
    Enjoyer2D::World world;
    world.setGravity({0.0f, 0.0f});

    const Enjoyer2D::BodyHandle floorHandle = world.createBody();
    Enjoyer2D::Body& floor = world.getBody(floorHandle);
    floor.position = {0.0f, 0.0f};
    floor.shape = Enjoyer2D::Shape::makeBox(10.0f, 1.0f);
    floor.setStatic();
    floor.friction = 0.0f;
    floor.restitution = 0.0f;

    const Enjoyer2D::BodyHandle boxHandle = world.createBody();
    Enjoyer2D::Body& box = world.getBody(boxHandle);
    box.position = {0.0f, 0.75f};
    box.velocity = {5.0f, -1.0f};
    box.shape = Enjoyer2D::Shape::makeBox(1.0f, 1.0f);
    box.setMass(1.0f);
    box.friction = 0.0f;
    box.restitution = 0.0f;

    world.step(1.0f / 60.0f);

    REQUIRE(world.getBody(boxHandle).velocity.x == 5.0f);
}


