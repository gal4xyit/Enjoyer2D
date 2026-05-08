#pragma once

namespace Enjoyer2D {
    enum class ShapeType {
        Circle,
        Box
    };

    struct CircleShape {
        float radius {0.5f};
    };

    struct BoxShape {
        float width {1.0f};
        float height {1.0f};
    };

    struct Shape {
        ShapeType type {ShapeType::Box};

        CircleShape circle {};
        BoxShape box {};

        static Shape makeCircle(float radius) {
            Shape shape;
            shape.type = ShapeType::Circle;
            shape.circle.radius = radius;
            return shape;
        }

        static Shape makeBox(float width, float height) {
            Shape shape;
            shape.type = ShapeType::Box;
            shape.box.width = width;
            shape.box.height = height;
            return shape;
        }
    };
}