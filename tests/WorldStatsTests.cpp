#include <Enjoyer2D/World.hpp>
#include <Enjoyer2D/Shape.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("World reports body count") {
    Enjoyer2D::World world;

    REQUIRE(world.getBodyCount() == 0);

    world.createBody();
    world.createBody();

    REQUIRE(world.getBodyCount() == 2);
}

TEST_CASE("World reports contact count") {
    Enjoyer2D::World world;
    world.setGravity({0.0f, 0.0f});

    const Enjoyer2D::BodyHandle firstHandle = world.createBody();
    Enjoyer2D::Body& first = world.getBody(firstHandle);
    first.position = {0.0f, 0.0f};
    first.shape = Enjoyer2D::Shape::makeBox(2.0f, 2.0f);
    first.setMass(1.0f);

    const Enjoyer2D::BodyHandle secondHandle = world.createBody();
    Enjoyer2D::Body& second = world.getBody(secondHandle);
    second.position = {1.0f, 0.0f};
    second.shape = Enjoyer2D::Shape::makeBox(2.0f, 2.0f);
    second.setMass(1.0f);

    world.step(1.0f / 60.0f);

    REQUIRE(world.getContactCount() == 1);
}
