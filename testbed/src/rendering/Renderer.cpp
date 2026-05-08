#include "Renderer.hpp"
#include <cmath>
#include <cstddef>

SDL_FRect boxToScreenRect(const Enjoyer2D::Body& body, const View& view) {
    const float width = body.shape.box.width * view.pixelsPerUnit;
    const float height = body.shape.box.height * view.pixelsPerUnit;
    const SDL_FPoint center = worldToScreen(body.position, view);

    return {
        center.x - width * 0.5f,
        center.y - height * 0.5f,
        width,
        height
    };
}

void drawCircle(SDL_Renderer* renderer, const Enjoyer2D::Body& body, const View& view) {
    constexpr int segments = 32;
    constexpr float twoPi = 6.28318530718f;

    const float radius = body.shape.circle.radius;

    for (int i = 0; i < segments; ++i) {
        const float angleA = static_cast<float>(i) / segments * twoPi;
        const float angleB = static_cast<float>(i + 1) / segments * twoPi;

        const glm::vec2 pointA {
            body.position.x + std::cos(angleA) * radius,
            body.position.y + std::sin(angleA) * radius
        };

        const glm::vec2 pointB {
            body.position.x + std::cos(angleB) * radius,
            body.position.y + std::sin(angleB) * radius
        };

        const SDL_FPoint screenA = worldToScreen(pointA, view);
        const SDL_FPoint screenB = worldToScreen(pointB, view);

        SDL_RenderLine(renderer, screenA.x, screenA.y, screenB.x, screenB.y);
    }
}

void drawBody(SDL_Renderer* renderer, const Enjoyer2D::Body& body, const View& view) {
    if (body.shape.type == Enjoyer2D::ShapeType::Box) {
        const SDL_FRect rect = boxToScreenRect(body, view);
        SDL_RenderRect(renderer, &rect);
        return;
    }

    if (body.shape.type == Enjoyer2D::ShapeType::Circle) {
        drawCircle(renderer, body, view);
    }
}

void drawBodyCenter(SDL_Renderer* renderer, const Enjoyer2D::Body& body, const View& view) {
    const SDL_FPoint center = worldToScreen(body.position, view);

    constexpr float radius = 3.0f;

    SDL_RenderLine(renderer, center.x - radius, center.y, center.x + radius, center.y);
    SDL_RenderLine(renderer, center.x, center.y - radius, center.x, center.y + radius);
}

void drawBodyPath(SDL_Renderer* renderer, const std::vector<glm::vec2>& points, const View& view) {

    if (points.size() < 2) {
        return;
    }

    for (std::size_t i = 0; i + 1 < points.size(); ++i) {
        const SDL_FPoint start = worldToScreen(points[i], view);
        const SDL_FPoint end = worldToScreen(points[i + 1], view);

        SDL_RenderLine(renderer, start.x, start.y, end.x, end.y);
    }
}

void drawBodyVelocity(SDL_Renderer* renderer, const Enjoyer2D::Body& body, const View& view, float scale) {
    const glm::vec2 start = body.position;
    const glm::vec2 end = body.position + body.velocity * scale;

    const SDL_FPoint startScreen = worldToScreen(start, view);
    const SDL_FPoint endScreen = worldToScreen(end, view);

    SDL_RenderLine(renderer, startScreen.x, startScreen.y, endScreen.x, endScreen.y);
}

void drawAabb(SDL_Renderer* renderer, const Enjoyer2D::AABB& aabb, const View& view) {
    const glm::vec2 center = aabb.center();
    const glm::vec2 size {
        aabb.width(),
        aabb.height()
    };

    const SDL_FPoint screenCenter = worldToScreen(center, view);

    SDL_FRect rect {
        screenCenter.x - size.x * view.pixelsPerUnit * 0.5f,
        screenCenter.y - size.y * view.pixelsPerUnit * 0.5f,
        size.x * view.pixelsPerUnit,
        size.y * view.pixelsPerUnit
    };

    SDL_RenderRect(renderer, &rect);
}

void drawContactPoint(SDL_Renderer* renderer, const Enjoyer2D::Contact& contact, const View& view) {
    const SDL_FPoint point = worldToScreen(contact.point, view);

    constexpr float radius = 4.0f;

    SDL_RenderLine(renderer, point.x - radius, point.y, point.x + radius, point.y);
    SDL_RenderLine(renderer, point.x, point.y - radius, point.x, point.y + radius);
}

void drawContactNormal(SDL_Renderer* renderer, const Enjoyer2D::Contact& contact, const View& view, float scale) {
    const glm::vec2 start = contact.point;
    const glm::vec2 end = contact.point + contact.normal * scale;

    const SDL_FPoint screenStart = worldToScreen(start, view);
    const SDL_FPoint screenEnd = worldToScreen(end, view);

    SDL_RenderLine(renderer, screenStart.x, screenStart.y, screenEnd.x, screenEnd.y);

    const glm::vec2 n = contact.normal;
    const glm::vec2 tangent {-n.y, n.x};

    constexpr float arrowLength = 0.15f;
    constexpr float arrowWidth = 0.08f;

    const glm::vec2 arrowA = end - n * arrowLength + tangent * arrowWidth;
    const glm::vec2 arrowB = end - n * arrowLength - tangent * arrowWidth;

    const SDL_FPoint screenArrowA = worldToScreen(arrowA, view);
    const SDL_FPoint screenArrowB = worldToScreen(arrowB, view);

    SDL_RenderLine(renderer, screenEnd.x, screenEnd.y, screenArrowA.x, screenArrowA.y);
    SDL_RenderLine(renderer, screenEnd.x, screenEnd.y, screenArrowB.x, screenArrowB.y);
}

void drawWorld(SDL_Renderer* renderer, const Enjoyer2D::World& world, const RenderContext& context) {
    for (std::size_t i = 0; i < world.getBodies().size(); ++i) {
        const Enjoyer2D::Body& body = world.getBodies()[i];

        if (context.selectedBodyIndex == static_cast<int>(i)) {
            SDL_SetRenderDrawColor(renderer, 255, 220, 90, 255);
        } else if (body.isStatic()) {
            SDL_SetRenderDrawColor(renderer, 150, 150, 160, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 90, 190, 255, 255);
        }

        drawBody(renderer, body, context.view);
    }

    if (context.debugSettings.showCenters) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 120, 255);

        for (const Enjoyer2D::Body& body : world.getBodies()) {
            if (!body.isStatic()) {
                drawBodyCenter(renderer, body, context.view);
            }
        }
    }

    if (context.debugSettings.showPath) {
        SDL_SetRenderDrawColor(renderer, 80, 180, 220, 255);

        const std::vector<Enjoyer2D::Body>& bodies = world.getBodies();

        for (std::size_t i = 0; i < bodies.size() && i < context.debugState.bodyTrails.size(); ++i) {
            if (!bodies[i].isStatic()) {
                drawBodyPath(renderer, context.debugState.bodyTrails[i].points, context.view);
            }
        }
    }

    if (context.debugSettings.showVelocities) {
        SDL_SetRenderDrawColor(renderer, 120, 255, 160, 255);

        for (const Enjoyer2D::Body& body : world.getBodies()) {
            if (!body.isStatic()) {
                drawBodyVelocity(renderer, body, context.view, context.debugSettings.velocityScale);
            }
        }
    }

    if (context.debugSettings.showAabbs) {
        SDL_SetRenderDrawColor(renderer, 255, 80, 220, 255);

        for (const Enjoyer2D::Body& body : world.getBodies()) {
            drawAabb(renderer, Enjoyer2D::computeAABB(body), context.view);
        }
    }

    if (context.debugSettings.showContactPoints) {
        SDL_SetRenderDrawColor(renderer, 255, 80, 80, 255);

        for (const Enjoyer2D::Contact& contact : world.getContacts()) {
            drawContactPoint(renderer, contact, context.view);
        }
    }

    if (context.debugSettings.showContactNormals) {
        SDL_SetRenderDrawColor(renderer, 80, 180, 255, 255);

        for (const Enjoyer2D::Contact& contact : world.getContacts()) {
            drawContactNormal(renderer, contact, context.view, context.debugSettings.normalScale);
        }
    }


}

