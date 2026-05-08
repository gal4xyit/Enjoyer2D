#include <Enjoyer2D/Shape.hpp>
#include <Enjoyer2D/World.hpp>
#include <Enjoyer2D/WorldStats.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("World stats snapshot reports body contact and solver counts") {
    Enjoyer2D::World world;
    world.setGravity({0.0f, 0.0f});
    world.setVelocityIterations(12);
    world.setPositionIterations(5);

    Enjoyer2D::BodyDef first;
    first.position = {0.0f, 0.0f};
    first.shape = Enjoyer2D::Shape::makeBox(2.0f, 2.0f);

    Enjoyer2D::BodyDef second;
    second.position = {1.0f, 0.0f};
    second.shape = Enjoyer2D::Shape::makeBox(2.0f, 2.0f);

    world.createBody(first);
    world.createBody(second);

    world.step(1.0f / 60.0f);

    const Enjoyer2D::WorldStats stats = world.getStats();

    REQUIRE(stats.bodyCount == 2);
    REQUIRE(stats.contactCount == 1);
    REQUIRE(stats.velocityIterations == 12);
    REQUIRE(stats.positionIterations == 5);
}

