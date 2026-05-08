#include <Enjoyer2D/World.hpp>
#include <Enjoyer2D/Shape.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("World detects box circle collision") {
    Enjoyer2D::World world;
    world.setGravity({0.0f, 0.0f});

    Enjoyer2D::BodyHandle boxHandle = world.createBody();
    Enjoyer2D::Body& boxBody = world.getBody(boxHandle);
    boxBody.position = {0.0f, 0.0f};
    boxBody.shape = Enjoyer2D::Shape::makeBox(2.0f, 2.0f);
    boxBody.setMass(1.0f);

    Enjoyer2D::BodyHandle circleHandle = world.createBody();
    Enjoyer2D::Body& circleBody = world.getBody(circleHandle);
    circleBody.position = {1.5f, 0.0f};
    circleBody.shape = Enjoyer2D::Shape::makeCircle(1.0f);
    circleBody.setMass(1.0f);

    world.step(1/60.0f);

    REQUIRE(world.getContacts().size() == 1);
}

TEST_CASE("World separates box circle collision") {
    Enjoyer2D::World world;
    world.setGravity({0.0f, 0.0f});

    Enjoyer2D::BodyHandle boxHandle = world.createBody();
    Enjoyer2D::Body& boxBody = world.getBody(boxHandle);
    boxBody.position = {0.0f, 0.0f};
    boxBody.shape = Enjoyer2D::Shape::makeBox(2.0f, 2.0f);
    boxBody.setMass(1.0f);

    Enjoyer2D::BodyHandle circleHandle = world.createBody();
    Enjoyer2D::Body& circleBody = world.getBody(circleHandle);
    circleBody.position = {1.5f, 0.0f};
    circleBody.shape = Enjoyer2D::Shape::makeCircle(1.0f);
    circleBody.setMass(1.0f);

    world.step(1/60.0f);

    REQUIRE(world.getBody(boxHandle).position.x < 0.0f);
    REQUIRE(world.getBody(circleHandle).position.x > 1.5f);
}

TEST_CASE("World bounces circle against static box") {
    Enjoyer2D::World world;
    world.setGravity({0.0f, 0.0f});

    Enjoyer2D::BodyHandle boxHandle = world.createBody();
    Enjoyer2D::Body& boxBody = world.getBody(boxHandle);
    boxBody.position = {0.0f, 0.0f};
    boxBody.shape = Enjoyer2D::Shape::makeBox(2.0f, 2.0f);
    boxBody.setStatic();
    boxBody.restitution = 1.0f;

    Enjoyer2D::BodyHandle circleHandle = world.createBody();
    Enjoyer2D::Body& circleBody = world.getBody(circleHandle);
    circleBody.position = {1.9f, 0.0f};
    circleBody.velocity = {-5.0f, 0.0f};
    circleBody.shape = Enjoyer2D::Shape::makeCircle(1.0f);
    circleBody.setMass(1.0f);
    circleBody.restitution = 1.0f;

    world.step(1/60.0f);

    REQUIRE(world.getBody(circleHandle).velocity.x == 5.0f);
}