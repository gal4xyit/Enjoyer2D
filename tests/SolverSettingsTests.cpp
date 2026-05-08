#include <Enjoyer2D/World.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("World has default solver iteration counts") {
    Enjoyer2D::World world;

    REQUIRE(world.getVelocityIterations() == 8);
    REQUIRE(world.getPositionIterations() == 3);
}

TEST_CASE("World allows setting solver iteration counts") {
    Enjoyer2D::World world;

    world.setVelocityIterations(12);
    world.setPositionIterations(5);

    REQUIRE(world.getVelocityIterations() == 12);
    REQUIRE(world.getPositionIterations() == 5);
}

TEST_CASE("World clamps solver iteration counts to at least one") {
    Enjoyer2D::World world;

    world.setVelocityIterations(0);
    world.setPositionIterations(-10);

    REQUIRE(world.getVelocityIterations() == 1);
    REQUIRE(world.getPositionIterations() == 1);
}

