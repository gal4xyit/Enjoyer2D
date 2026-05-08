#include <Enjoyer2D/Shape.hpp>
#include <Enjoyer2D/Collision.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("Box circle collision detects circle to the right of box") {
    Enjoyer2D::Body box;
    box.position = {0.0f, 0.0f};
    box.shape = Enjoyer2D::Shape::makeBox(2.0f, 2.0f);

    Enjoyer2D::Body circle;
    circle.position = {1.5f, 0.0f};
    circle.shape = Enjoyer2D::Shape::makeCircle(1.0f);

    const Enjoyer2D::Contact contact = Enjoyer2D::collideBodies(box, circle);

    REQUIRE(contact.collided);
    REQUIRE_THAT(contact.penetration, Catch::Matchers::WithinAbs(0.5f, 0.0001f));
    REQUIRE_THAT(contact.normal.x, Catch::Matchers::WithinAbs(1.0f, 0.0001f));
    REQUIRE_THAT(contact.normal.y, Catch::Matchers::WithinAbs(0.0f, 0.0001f));
}

TEST_CASE("Box circle collision normal flips when order is circle -> box") {
    Enjoyer2D::Body circle;
    circle.position = {1.5f, 0.0f};
    circle.shape = Enjoyer2D::Shape::makeCircle(1.0f);

    Enjoyer2D::Body box;
    box.position = {0.0f, 0.0f};
    box.shape = Enjoyer2D::Shape::makeBox(2.0f, 2.0f);

    Enjoyer2D::Contact contact = Enjoyer2D::collideBodies(circle, box);

    REQUIRE(contact.collided);
    REQUIRE_THAT(contact.penetration, Catch::Matchers::WithinAbs(0.5f, 0.0001f));
    REQUIRE_THAT(contact.normal.x, Catch::Matchers::WithinAbs(-1.0f, 0.0f));
    REQUIRE_THAT(contact.normal.y, Catch::Matchers::WithinAbs(0.0f, 0.0f));
}

TEST_CASE("Box circle collision detects diagonal corner contact") {
    Enjoyer2D::Body box;
    box.position = {0.0f, 0.0f};
    box.shape = Enjoyer2D::Shape::makeBox(2.0f, 2.0f);

    Enjoyer2D::Body circle;
    circle.position = {1.5f, 1.5f};
    circle.shape = Enjoyer2D::Shape::makeCircle(1.0f);

    const Enjoyer2D::Contact contact = Enjoyer2D::collideBodies(box, circle);

    REQUIRE(contact.collided);
    REQUIRE(contact.normal.x > 0.0f);
    REQUIRE(contact.normal.y > 0.0f);
    REQUIRE_THAT(contact.normal.x, Catch::Matchers::WithinAbs(0.7071f, 0.001f));
    REQUIRE_THAT(contact.normal.y, Catch::Matchers::WithinAbs(0.7071f, 0.001f));
}

TEST_CASE("Box circle collision doesnt detect separated circle") {
    Enjoyer2D::Body box;
    box.position = {0.0f, 0.0f};
    box.shape = Enjoyer2D::Shape::makeBox(2.0f, 2.0f);

    Enjoyer2D::Body circle;
    circle.position = {3.0f, 0.0f};
    circle.shape = Enjoyer2D::Shape::makeCircle(1.0f);

    const Enjoyer2D::Contact contact = Enjoyer2D::collideBodies(box, circle);

    REQUIRE_FALSE(contact.collided);
}