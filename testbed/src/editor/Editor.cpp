#include "Editor.hpp"

#include <Enjoyer2D/Collision.hpp>

#include <glm/geometric.hpp>

#include <vector>
#include <cstddef>

int pickBodyAtPoint(const Enjoyer2D::World& world, const glm::vec2& point) {
    const std::vector<Enjoyer2D::Body>& bodies = world.getBodies();

    for (std::size_t i = bodies.size(); i-- > 0;) {
        const Enjoyer2D::Body& body = bodies[i];

        if (body.shape.type == Enjoyer2D::ShapeType::Box) {
            const float halfWidth = body.shape.box.width * 0.5f;
            const float halfHeight = body.shape.box.height * 0.5f;

            const bool insideX =
                point.x >= body.position.x - halfWidth &&
                point.x <= body.position.x + halfWidth;

            const bool insideY =
                point.y >= body.position.y - halfHeight &&
                point.y <= body.position.y + halfHeight;

            if (insideX && insideY) {
                return static_cast<int>(i);
            }
        }

        if (body.shape.type == Enjoyer2D::ShapeType::Circle) {
            const glm::vec2 offset = point - body.position;
            const float distanceSquared = glm::dot(offset, offset);
            const float radiusSquared = body.shape.circle.radius * body.shape.circle.radius;

            if (distanceSquared <= radiusSquared) {
                return static_cast<int>(i);
            }
        }
    }

    return -1;
}

bool overlapsWorld(const Enjoyer2D::World& world, const Enjoyer2D::Body& candidate)
{
    for (const Enjoyer2D::Body& body : world.getBodies()) {
        if (Enjoyer2D::collideBodies(body, candidate).collided) {
            return true;
        }
    }

    return false;
}

Enjoyer2D::Shape makeSpawnShape(const EditorState& editor) {
    if (editor.activeTool == EditorTool::SpawnCircle) {
        return Enjoyer2D::Shape::makeCircle(editor.spawn.circleRadius);
    }

    return Enjoyer2D::Shape::makeBox(
        editor.spawn.boxWidth,
        editor.spawn.boxHeight
    );
}

Enjoyer2D::BodyDef makeSpawnBodyDef(const EditorState& editor) {
    Enjoyer2D::BodyDef def;
    def.position = editor.mouseWorld;
    def.mass = editor.spawn.mass;
    def.velocity = editor.spawn.velocity;
    def.friction = editor.spawn.friction;
    def.restitution = editor.spawn.restitution;
    def.isStatic = editor.spawn.isStatic;
    def.shape = makeSpawnShape(editor);

    return def;
}

Enjoyer2D::Body makePreviewBody(const EditorState& editor) {
    Enjoyer2D::Body body;
    body.position = editor.mouseWorld;
    body.velocity = editor.spawn.velocity;
    body.friction = editor.spawn.friction;
    body.restitution = editor.spawn.restitution;
    body.shape = makeSpawnShape(editor);

    if (editor.spawn.isStatic) {
        body.setStatic();
    } else {
        body.setMass(editor.spawn.mass);
    }

    return body;
}