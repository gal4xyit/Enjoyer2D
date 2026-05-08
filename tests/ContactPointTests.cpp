#include <Enjoyer2D/Collision.hpp>
#include <Enjoyer2D/Shape.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("AABB collision stores approximate contact point") {
    const Enjoyer2D::AABB a {
            {0.0f, 0.0f},
            {1.0f, 1.0f}
    };

    const Enjoyer2D::AABB b {
            {0.5f, 0.0f},
            {1.5f, 1.0f}
    };

    const Enjoyer2D::Contact contact = Enjoyer2D::collideAABB(a, b);

    REQUIRE(contact.collided);
    REQUIRE_THAT(contact.point.x, Catch::Matchers::WithinAbs(0.75f, 0.0001f));
    REQUIRE_THAT(contact.point.y, Catch::Matchers::WithinAbs(0.5f, 0.0001f));
}

TEST_CASE("Circle collision stores point on first circle") {
    Enjoyer2D::Body a;
    a.position = {0.0f, 0.0f};
    a.shape = Enjoyer2D::Shape::makeCircle(1.0f);

    Enjoyer2D::Body b;
    b.position = {1.5f, 0.0f};
    b.shape = Enjoyer2D::Shape::makeCircle(1.0f);

    const Enjoyer2D::Contact contact = Enjoyer2D::collideBodies(a, b);

    REQUIRE(contact.collided);
    REQUIRE_THAT(contact.point.x, Catch::Matchers::WithinAbs(1.0f, 0.0001f));
    REQUIRE_THAT(contact.point.y, Catch::Matchers::WithinAbs(0.0f, 0.0001f));
}

TEST_CASE("Box circle collision stores closest point on box") {
    Enjoyer2D::Body box;
    box.position = {0.0f, 0.0f};
    box.shape = Enjoyer2D::Shape::makeBox(2.0f, 2.0f);

    Enjoyer2D::Body circle;
    circle.position = {1.5f, 0.0f};
    circle.shape = Enjoyer2D::Shape::makeCircle(1.0f);

    const Enjoyer2D::Contact contact = Enjoyer2D::collideBodies(box, circle);

    REQUIRE(contact.collided);
    REQUIRE_THAT(contact.point.x, Catch::Matchers::WithinAbs(1.0f, 0.0001f));
    REQUIRE_THAT(contact.point.y, Catch::Matchers::WithinAbs(0.0f, 0.0001f));
}
