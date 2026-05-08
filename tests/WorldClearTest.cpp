#include <Enjoyer2D/Shape.hpp>
#include <Enjoyer2D/World.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("World clear removes bodies and contacts") {
    Enjoyer2D::World world;
    world.setGravity({0.0f, 0.0f});

    Enjoyer2D::BodyDef first;
    first.position = {0.0f, 0.0f};
    first.shape = Enjoyer2D::Shape::makeBox(2.0f, 2.0f);

    Enjoyer2D::BodyDef second;
    second.position = {1.0f, 0.0f};
    second.shape = Enjoyer2D::Shape::makeBox(2.0f, 2.0f);

    world.createBody(first);
    world.createBody(second);

    world.step(1.0f / 60.0f);

    REQUIRE(world.getBodyCount() == 2);
    REQUIRE(world.getContactCount() == 1);

    world.clear();

    REQUIRE(world.getBodyCount() == 0);
    REQUIRE(world.getContactCount() == 0);
}