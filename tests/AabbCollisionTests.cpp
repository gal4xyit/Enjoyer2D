#include <Enjoyer2D/Collision.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("AABB collision normal points from first box to second on right") {
    const Enjoyer2D::AABB a {
        {0.0f, 0.0f},
        {1.0f, 1.0f}
    };

    const Enjoyer2D::AABB b {
            {0.5f, 0.0f},
            {1.5f, 1.0f}
    };

    const Enjoyer2D::Contact contact =  Enjoyer2D::collideAABB(a,b);

    REQUIRE(contact.collided);
    REQUIRE(contact.normal.x == 1.0f);
    REQUIRE(contact.normal.y == 0.0f);
    REQUIRE(contact.penetration == 0.5f);
}

TEST_CASE("AABB collision normal points from first box to second on left") {
    const Enjoyer2D::AABB a {
                {0.0f, 0.0f},
                {1.0f, 1.0f}
    };

    const Enjoyer2D::AABB b {
                    {-0.5f, 0.0f},
                    {0.5f, 1.0f}
    };

    const Enjoyer2D::Contact contact =  Enjoyer2D::collideAABB(a,b);

    REQUIRE(contact.collided);
    REQUIRE(contact.normal.x == -1.0f);
    REQUIRE(contact.normal.y == 0.0f);
    REQUIRE(contact.penetration == 0.5f);
}

TEST_CASE("AABB collision normal points from first box to second above") {
    const Enjoyer2D::AABB a {
                {0.0f, 0.0f},
                {1.0f, 1.0f}
    };

    const Enjoyer2D::AABB b {
                    {0.0f, 0.5f},
                    {1.0f, 1.5f}
    };

    const Enjoyer2D::Contact contact =  Enjoyer2D::collideAABB(a,b);

    REQUIRE(contact.collided);
    REQUIRE(contact.normal.x == 0.0f);
    REQUIRE(contact.normal.y == 1.0f);
    REQUIRE(contact.penetration == 0.5f);
}

TEST_CASE("AABB collision normal points from first box to second under") {
    const Enjoyer2D::AABB a {
                    {0.0f, 0.0f},
                    {1.0f, 1.0f}
    };

    const Enjoyer2D::AABB b {
                        {0.0f, -0.5f},
                        {1.0f, 0.5f}
    };

    const Enjoyer2D::Contact contact =  Enjoyer2D::collideAABB(a,b);

    REQUIRE(contact.collided);
    REQUIRE(contact.normal.x == 0.0f);
    REQUIRE(contact.normal.y == -1.0f);
    REQUIRE(contact.penetration == 0.5f);
}