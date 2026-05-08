#include <Enjoyer2D/World.hpp>
#include <Enjoyer2D/Shape.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("World detects overlapping circles") {
    Enjoyer2D::World world;
    world.setGravity({0.0f, 0.0f});

    const Enjoyer2D::BodyHandle aHandle = world.createBody();
    Enjoyer2D::Body& a = world.getBody(aHandle);
    a.position = {0.0f, 0.0f};
    a.shape = Enjoyer2D::Shape::makeCircle(1.0f);
    a.setMass(1.0f);

    const Enjoyer2D::BodyHandle bHandle = world.createBody();
    Enjoyer2D::Body& b = world.getBody(bHandle);
    b.position = {1.5f, 0.0f};
    b.shape = Enjoyer2D::Shape::makeCircle(1.0f);
    b.setMass(1.0f);

    world.step(1/60.0f);

    REQUIRE(world.getContacts().size() == 1);
}

TEST_CASE("World separates overlapping circles") {
    Enjoyer2D::World world;
    world.setGravity({0.0f, 0.0f});

    const Enjoyer2D::BodyHandle aHandle = world.createBody();
    Enjoyer2D::Body& a = world.getBody(aHandle);
    a.position = {0.0f, 0.0f};
    a.shape = Enjoyer2D::Shape::makeCircle(1.0f);
    a.setMass(1.0f);

    const Enjoyer2D::BodyHandle bHandle = world.createBody();
    Enjoyer2D::Body& b = world.getBody(bHandle);
    b.position = {1.5f, 0.0f};
    b.shape = Enjoyer2D::Shape::makeCircle(1.0f);
    b.setMass(1.0f);

    world.step(1/60.0f);

    REQUIRE(world.getBody(aHandle).position.x < 0.0f);
    REQUIRE(world.getBody(bHandle).position.x > 1.5f);
}

TEST_CASE("World bounces circles using collision response") {
    Enjoyer2D::World world;
    world.setGravity({0.0f, 0.0f});

    const Enjoyer2D::BodyHandle aHandle = world.createBody();
    Enjoyer2D::Body& a = world.getBody(aHandle);
    a.position = {0.0f, 0.0f};
    a.velocity = {1.0f, 0.0f};
    a.shape = Enjoyer2D::Shape::makeCircle(1.0f);
    a.setMass(1.0f);
    a.restitution = 1.0f;

    const Enjoyer2D::BodyHandle bHandle = world.createBody();
    Enjoyer2D::Body& b = world.getBody(bHandle);
    b.position = {1.9f, 0.0f};
    b.velocity = {-1.0f, 0.0f};
    b.shape = Enjoyer2D::Shape::makeCircle(1.0f);
    b.setMass(1.0f);
    a.restitution = 1.0f;

    world.step(1/60.0f);

    REQUIRE(world.getBody(aHandle).velocity.x < 0.0f);
    REQUIRE(world.getBody(bHandle).velocity.x > 0.0f);
}