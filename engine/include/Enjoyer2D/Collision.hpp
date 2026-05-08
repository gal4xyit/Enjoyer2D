#pragma once

#include <algorithm>
#include <Enjoyer2D/Body.hpp>
#include <Enjoyer2D/Shape.hpp>
#include <Enjoyer2D/AABB.hpp>
#include <Enjoyer2D/Contact.hpp>
#include <glm/geometric.hpp>

namespace Enjoyer2D {

    [[nodiscard]] inline AABB computeAABB(const Body& body) {
        const glm::vec2 center = body.position;

        if (body.shape.type == ShapeType::Circle) {
            const float radius = body.shape.circle.radius;
            return {center - glm::vec2 {radius, radius},
            center + glm::vec2 {radius, radius},};
        }

        const float halfWidth = body.shape.box.width * 0.5f;
        const float halfHeight = body.shape.box.height * 0.5f;

        return {center - glm::vec2 {halfWidth, halfHeight},
        center + glm::vec2 {halfWidth, halfHeight},};
    }

    [[nodiscard]] inline bool overlaps(const AABB& a, const AABB& b) {
        if (a.max.x < b.min.x || a.min.x > b.max.x) {
            return false;
        }
        if (a.max.y < b.min.y || a.min.y > b.max.y) {
            return false;
        }
        return true;
    }

    [[nodiscard]] inline Contact collideAABB(const AABB& a, const AABB& b) {
        Contact contact;

        if (!overlaps(a, b)) {
            return contact;
        }

        const float overlapRight = a.max.x - b.min.x;
        const float overlapLeft = b.max.x - a.min.x;
        const float overlapUp = a.max.y - b.min.y;
        const float overlapDown = b.max.y - a.min.y;

        contact.collided = true;

        contact.penetration = overlapLeft;
        contact.normal = {-1.0f, 0.0f};

        const float contactMinX = std::max(a.min.x, b.min.x);
        const float contactMaxX = std::min(a.max.x, b.max.x);
        const float contactMinY = std::max(a.min.y, b.min.y);
        const float contactMaxY = std::min(a.max.y, b.max.y);

        contact.point = {
            (contactMinX + contactMaxX) * 0.5f,
            (contactMinY + contactMaxY) * 0.5f
        };

        if (overlapRight < contact.penetration) {
            contact.penetration = overlapRight;
            contact.normal = {1.0f, 0.0f};
        }

        if (overlapDown < contact.penetration) {
            contact.penetration = overlapDown;
            contact.normal = {0.0f, -1.0f};
        }

        if (overlapUp < contact.penetration) {
            contact.penetration = overlapUp;
            contact.normal = {0.0f, 1.0f};
        }

        return contact;
    }

    [[nodiscard]] inline Contact collideCircles(const Body& a, const Body& b) {
        Contact contact;

        const glm::vec2 offset = b.position - a.position;
        const float distance = glm::length(offset);

        float totalRadius = a.shape.circle.radius + b.shape.circle.radius;

        if (distance >= totalRadius) {
            return contact;
        }

        contact.collided = true;
        contact.penetration = totalRadius - distance;

        if (distance > 0.0f) {
            contact.normal = offset/distance;
        }else {
            contact.normal = {1.0f, 0.0f};
        }

        contact.point = a.position + contact.normal * a.shape.circle.radius;

        return contact;
    }

    [[nodiscard]] inline Contact collideBoxCircle(const Body& boxBody, const Body& circleBody) {
        Contact contact;

        const AABB box = computeAABB(boxBody);

        const glm::vec2 closestPoint {std::clamp(circleBody.position.x, box.min.x, box.max.x),
        std::clamp(circleBody.position.y, box.min.y, box.max.y)};

        const glm::vec2 offset = circleBody.position - closestPoint;
        const float distance = glm::length(offset);
        const float radius = circleBody.shape.circle.radius;

        if (distance >= radius) {
            return contact;
        }

        contact.collided = true;
        contact.penetration = radius - distance;

        if (distance > 0.0f) {
            contact.normal = offset/distance;
        }else {
            contact.normal = {1.0f, 0.0f};
        }

        contact.point = closestPoint;

        return contact;
    }

    [[nodiscard]] inline Contact collideBodies(const Body& a, const Body& b) {
        if (a.shape.type == ShapeType::Box && b.shape.type == ShapeType::Box) {
            return collideAABB(computeAABB(a), computeAABB(b));
        }

        if (a.shape.type == ShapeType::Circle && b.shape.type == ShapeType::Circle) {
            return collideCircles(a, b);
        }

        if (a.shape.type == ShapeType::Box && b.shape.type == ShapeType::Circle) {
            return collideBoxCircle(a, b);
        }

        if (a.shape.type == ShapeType::Circle && b.shape.type == ShapeType::Box) {
            Contact contact = collideBoxCircle(b, a);
            contact.normal = -contact.normal;
            return contact;
        }

        return {};
    }
}
