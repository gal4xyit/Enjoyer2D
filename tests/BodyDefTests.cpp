#include <Enjoyer2D/World.hpp>
#include <Enjoyer2D/Shape.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("World creates dynamic body from definition") {
    Enjoyer2D::World world;

    Enjoyer2D::BodyDef def;
    def.position = {2.0f, 3.0f};
    def.velocity = {4.0f, 5.0f};
    def.shape = Enjoyer2D::Shape::makeBox(2.0f, 1.0f);
    def.mass = 10.0f;
    def.restitution = 0.7f;
    def.friction = 0.9f;

    const Enjoyer2D::BodyHandle handle = world.createBody(def);
    const Enjoyer2D::Body& body = world.getBody(handle);

    REQUIRE(body.position.x == 2.0f);
    REQUIRE(body.position.y == 3.0f);
    REQUIRE(body.velocity.x == 4.0f);
    REQUIRE(body.velocity.y == 5.0f);
    REQUIRE(body.getMass() == 10.0f);
    REQUIRE(body.getInverseMass() == 0.1f);
    REQUIRE_FALSE(body.isStatic());
    REQUIRE(body.restitution == 0.7f);
    REQUIRE(body.friction == 0.9f);
    REQUIRE(body.shape.type == Enjoyer2D::ShapeType::Box);
    REQUIRE(body.shape.box.width == 2.0f);
    REQUIRE(body.shape.box.height == 1.0f);
}

TEST_CASE("World creates static body from definition") {
    Enjoyer2D::World world;

    Enjoyer2D::BodyDef def;
    def.position = {1.0f, 2.0f};
    def.velocity = {10.0f, 20.0f};
    def.shape = Enjoyer2D::Shape::makeCircle(3.0f);
    def.mass = 50.0f;
    def.isStatic = true;

    const Enjoyer2D::BodyHandle handle = world.createBody(def);
    const Enjoyer2D::Body& body = world.getBody(handle);

    REQUIRE(body.position.x == 1.0f);
    REQUIRE(body.position.y == 2.0f);
    REQUIRE(body.velocity.x == 0.0f);
    REQUIRE(body.velocity.y == 0.0f);
    REQUIRE(body.getMass() == 0.0f);
    REQUIRE(body.getInverseMass() == 0.0f);
    REQUIRE(body.isStatic());
    REQUIRE(body.shape.type == Enjoyer2D::ShapeType::Circle);
    REQUIRE(body.shape.circle.radius == 3.0f);
}