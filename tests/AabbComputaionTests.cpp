#include <Enjoyer2D/Collision.hpp>
#include <Enjoyer2D/Shape.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Computes AABB for box body") {
    Enjoyer2D::Body body;
    body.position = {2.0f, 3.0f};
    body.shape = Enjoyer2D::Shape::makeBox(4.0f, 2.0f);

    const Enjoyer2D::AABB aabb = Enjoyer2D::computeAABB(body);

    REQUIRE(aabb.min.x == 0.0f);
    REQUIRE(aabb.min.y == 2.0f);
    REQUIRE(aabb.max.x == 4.0f);
    REQUIRE(aabb.max.y == 4.0f);
    REQUIRE(aabb.width() == 4.0f);
    REQUIRE(aabb.height() == 2.0f);
    REQUIRE(aabb.center().x == 2.0f);
    REQUIRE(aabb.center().y == 3.0f);
}

TEST_CASE("Computes AABB for circle body") {
    Enjoyer2D::Body body;
    body.position = {2.0f, 3.0f};
    body.shape = Enjoyer2D::Shape::makeCircle(1.5f);

    const Enjoyer2D::AABB aabb = Enjoyer2D::computeAABB(body);

    REQUIRE(aabb.min.x == 0.5f);
    REQUIRE(aabb.min.y == 1.5f);
    REQUIRE(aabb.max.x == 3.5f);
    REQUIRE(aabb.max.y == 4.5f);
    REQUIRE(aabb.width() == 3.0f);
    REQUIRE(aabb.height() == 3.0f);
    REQUIRE(aabb.center().x == 2.0f);
    REQUIRE(aabb.center().y == 3.0f);
}
