#include <Enjoyer2D/World.hpp>
#include <Enjoyer2D/Shape.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Position correction separates overlapping dynamic boxes") {
    Enjoyer2D::World world;
    world.setGravity({0.0f, 0.0f});

    const Enjoyer2D::BodyHandle aHandle = world.createBody();
    Enjoyer2D::Body& a = world.getBody(aHandle);
    a.position = {0.0f, 0.0f};
    a.shape = Enjoyer2D::Shape::makeBox(2.0f, 2.0f);
    a.setMass(1.0f);

    const Enjoyer2D::BodyHandle bHandle = world.createBody();
    Enjoyer2D::Body& b = world.getBody(bHandle);
    b.position = {1.0f, 0.0f};
    b.shape = Enjoyer2D::Shape::makeBox(2.0f, 2.0f);
    b.setMass(1.0f);

    world.step(1.0f / 60.0f);

    REQUIRE(world.getBody(aHandle).position.x < 0.0f);
    REQUIRE(world.getBody(bHandle).position.x > 1.0f);
}

TEST_CASE("Position correction does not move static body") {
    Enjoyer2D::World world;
    world.setGravity({0.0f, 0.0f});

    const Enjoyer2D::BodyHandle floorHandle = world.createBody();
    Enjoyer2D::Body& floor = world.getBody(floorHandle);
    floor.position = {0.0f, 0.0f};
    floor.shape = Enjoyer2D::Shape::makeBox(4.0f, 1.0f);
    floor.setStatic();

    const Enjoyer2D::BodyHandle boxHandle = world.createBody();
    Enjoyer2D::Body& box = world.getBody(boxHandle);
    box.position = {0.0f, 0.25f};
    box.shape = Enjoyer2D::Shape::makeBox(1.0f, 1.0f);
    box.setMass(1.0f);

    world.step(1.0f / 60.0f);

    REQUIRE(world.getBody(floorHandle).position.x == 0.0f);
    REQUIRE(world.getBody(floorHandle).position.y == 0.0f);
    REQUIRE(world.getBody(boxHandle).position.y > 0.25f);
}

TEST_CASE("Collision response stops body moving into static floor") {
    Enjoyer2D::World world;
    world.setGravity({0.0f, 0.0f});

    const Enjoyer2D::BodyHandle floorHandle = world.createBody();
    Enjoyer2D::Body& floor = world.getBody(floorHandle);
    floor.position = {0.0f, 0.0f};
    floor.shape = Enjoyer2D::Shape::makeBox(4.0f, 1.0f);
    floor.setStatic();
    floor.restitution = 0.0f;

    const Enjoyer2D::BodyHandle boxHandle = world.createBody();
    Enjoyer2D::Body& box = world.getBody(boxHandle);
    box.position = {0.0f, 0.25f};
    box.velocity = {0.0f, -10.0f};
    box.shape = Enjoyer2D::Shape::makeBox(1.0f, 1.0f);
    box.setMass(1.0f);
    box.restitution = 0.0f;

    world.step(1.0f / 60.0f);

    REQUIRE(world.getBody(boxHandle).velocity.y == 0.0f);
}

TEST_CASE("Collision response bounces body with restitution") {
    Enjoyer2D::World world;
    world.setGravity({0.0f, 0.0f});

    const Enjoyer2D::BodyHandle floorHandle = world.createBody();
    Enjoyer2D::Body& floor = world.getBody(floorHandle);
    floor.position = {0.0f, 0.0f};
    floor.shape = Enjoyer2D::Shape::makeBox(4.0f, 1.0f);
    floor.setStatic();
    floor.restitution = 1.0f;

    const Enjoyer2D::BodyHandle boxHandle = world.createBody();
    Enjoyer2D::Body& box = world.getBody(boxHandle);
    box.position = {0.0f, 0.25f};
    box.velocity = {0.0f, -10.0f};
    box.shape = Enjoyer2D::Shape::makeBox(1.0f, 1.0f);
    box.setMass(1.0f);
    box.restitution = 1.0f;

    world.step(1.0f / 60.0f);

    REQUIRE(world.getBody(boxHandle).velocity.y > 0.0f);
}
