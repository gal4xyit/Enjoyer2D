#include <Enjoyer2D/World.hpp>
#include <Enjoyer2D/Collision.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("World detects overlapping boxes") {
    Enjoyer2D::World world;
    world.setGravity({0.0f, 0.0f});

    const Enjoyer2D::BodyHandle firstHandle = world.createBody();
    Enjoyer2D::Body& first = world.getBody(firstHandle);
    first.position = {0.0f, 0.0f};
    first.shape = Enjoyer2D::Shape::makeBox(2.0f, 2.0f);
    first.setStatic();

    const Enjoyer2D::BodyHandle secondHandle = world.createBody();
    Enjoyer2D::Body& second = world.getBody(secondHandle);
    second.position = {1.0f, 0.0f};
    second.shape = Enjoyer2D::Shape::makeBox(2.0f, 2.0f);
    second.setStatic();

    world.step(1/60.0f);

    REQUIRE(world.getContacts().size() == 1);
}

TEST_CASE("World doesnt detect separated boxes") {
    Enjoyer2D::World world;
    world.setGravity({0.0f, 0.0f});

    const Enjoyer2D::BodyHandle firstHandle = world.createBody();
    Enjoyer2D::Body& first = world.getBody(firstHandle);
    first.position = {0.0f, 0.0f};
    first.shape = Enjoyer2D::Shape::makeBox(2.0f, 2.0f);
    first.setStatic();

    const Enjoyer2D::BodyHandle secondHandle = world.createBody();
    Enjoyer2D::Body& second = world.getBody(secondHandle);
    second.position = {5.0f, 0.0f};
    second.shape = Enjoyer2D::Shape::makeBox(2.0f, 2.0f);
    second.setStatic();

    world.step(1/60.0f);

    REQUIRE(world.getContacts().empty());
}
