#include <Enjoyer2D/World.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("World applies gravity to dynamic bodies") {
    Enjoyer2D::World world;
    world.setGravity({0.0f, -10.0f});

    const Enjoyer2D::BodyHandle bodyHandle = world.createBody();
    Enjoyer2D::Body& body = world.getBody(bodyHandle);
    body.position = {0.0f, 10.0f};
    body.velocity = {0.0f, 0.0f};
    body.setMass(1.0f);

    world.step(1.0f);

    const Enjoyer2D::Body& steppedBody = world.getBody(bodyHandle);
    REQUIRE(steppedBody.velocity.y == -10.0f);
    REQUIRE(steppedBody.position.y == 0.0f);
}

TEST_CASE("Static bodies dont move") {
    Enjoyer2D::World world;
    world.setGravity({0.0f, -10.0f});

    const Enjoyer2D::BodyHandle bodyHandle = world.createBody();
    Enjoyer2D::Body& body = world.getBody(bodyHandle);
    body.position = {0.0f, 10.0f};
    body.velocity = {5.0f, 0.0f};
    body.setMass(1.0f);
    body.setStatic();

    world.step(1.0f);

    const Enjoyer2D::Body& steppedBody = world.getBody(bodyHandle);
    REQUIRE(steppedBody.velocity.x == 0.0f);
    REQUIRE(steppedBody.velocity.y == 0.0f);
    REQUIRE(steppedBody.position.x == 0.0f);
    REQUIRE(steppedBody.position.y == 10.0f);
}

TEST_CASE("Force affect acceleration based on mass") {
    Enjoyer2D::World world;
    world.setGravity({0.0f, 0.0f});

    const Enjoyer2D::BodyHandle bodyHandle = world.createBody();
    Enjoyer2D::Body& body = world.getBody(bodyHandle);
    body.setMass(10.0f);
    body.addForce({20.0f, 0.0f});

    world.step(1.0f);

    const Enjoyer2D::Body& steppedBody = world.getBody(bodyHandle);
    REQUIRE(steppedBody.velocity.x == 2.0f);
    REQUIRE(steppedBody.position.x == 2.0f);
}
