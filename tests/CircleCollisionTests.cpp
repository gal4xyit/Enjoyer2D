#include <Enjoyer2D/Collision.hpp>
#include <Enjoyer2D/Shape.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("Circle collision detects overlapping circles") {
    Enjoyer2D::Body a;
    a.position = {0.0f, 0.0f};
    a.shape = Enjoyer2D::Shape::makeCircle(1.0f);

    Enjoyer2D::Body b;
    b.position = {1.5f, 0.0f};
    b.shape = Enjoyer2D::Shape::makeCircle(1.0f);

    const Enjoyer2D::Contact contact = Enjoyer2D::collideBodies(a, b);

    REQUIRE(contact.collided);
    REQUIRE_THAT(contact.penetration, Catch::Matchers::WithinAbs(0.5f, 0.0001f));
    REQUIRE_THAT(contact.normal.x, Catch::Matchers::WithinAbs(1.0f, 0.0001f));
    REQUIRE_THAT(contact.normal.y, Catch::Matchers::WithinAbs(0.0f, 0.0001f));
}

TEST_CASE("Circle collision doesnt detect separated circles") {
    Enjoyer2D::Body a;
    a.position = {0.0f, 0.0f};
    a.shape = Enjoyer2D::Shape::makeCircle(1.0f);

    Enjoyer2D::Body b;
    b.position = {3.0f, 0.0f};
    b.shape = Enjoyer2D::Shape::makeCircle(1.0f);

    const Enjoyer2D::Contact contact = Enjoyer2D::collideBodies(a, b);

    REQUIRE_FALSE(contact.collided);
}

TEST_CASE("Circle collision normal can be diagonal") {
    Enjoyer2D::Body a;
    a.position = {0.0f, 0.0f};
    a.shape = Enjoyer2D::Shape::makeCircle(1.0f);

    Enjoyer2D::Body b;
    b.position = {1.0f, 1.0f};
    b.shape = Enjoyer2D::Shape::makeCircle(1.0f);

    const Enjoyer2D::Contact contact = Enjoyer2D::collideBodies(a, b);

    REQUIRE(contact.collided);
    REQUIRE(contact.normal.x > 0.0f);
    REQUIRE(contact.normal.y > 0.0f);
    REQUIRE_THAT(contact.normal.x, Catch::Matchers::WithinAbs(0.7071f, 0.001f));
    REQUIRE_THAT(contact.normal.y, Catch::Matchers::WithinAbs(0.7071f, 0.001f));
}

TEST_CASE("Circle collision handles identical centers") {
    Enjoyer2D::Body a;
    a.position = {0.0f, 0.0f};
    a.shape = Enjoyer2D::Shape::makeCircle(1.0f);

    Enjoyer2D::Body b;
    b.position = {0.0f, 0.0f};
    b.shape = Enjoyer2D::Shape::makeCircle(1.0f);

    const Enjoyer2D::Contact contact = Enjoyer2D::collideBodies(a, b);

    REQUIRE(contact.collided);
    REQUIRE_THAT(contact.penetration, Catch::Matchers::WithinAbs(2.0f, 0.0001f));
    REQUIRE_THAT(contact.normal.x, Catch::Matchers::WithinAbs(1.0f, 0.0001f));
    REQUIRE_THAT(contact.normal.y, Catch::Matchers::WithinAbs(0.0f, 0.0001f));
}