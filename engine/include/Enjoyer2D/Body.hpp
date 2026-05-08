#pragma once

#include <Enjoyer2D/Shape.hpp>
#include <glm/vec2.hpp>

namespace Enjoyer2D {

    struct BodyDef {
        glm::vec2 position {0.0f, 0.0f};
        glm::vec2 velocity {0.0f, 0.0f};

        Shape shape {};

        float mass {1.0f};
        float restitution {0.2f};
        float friction {0.5f};

        bool isStatic {false};
    };

    class Body {
    public:
        glm::vec2 position {0.0f, 0.0f};
        glm::vec2 velocity {0.0f, 0.0f};
        glm::vec2 force {0.0f, 0.0f};

        Shape shape {};

        float restitution {0.2f};
        float friction {0.5f};

        void setMass(float newMass) {
            if (newMass <= 0.0f) {
                setStatic();
                return;
            }

            mass = newMass;
            inverseMass = 1.0f / mass;
            staticBody = false;
        }

        void setStatic() {
            mass = 0.0f;
            inverseMass = 0.0f;
            staticBody = true;
            velocity = glm::vec2 {0.0f, 0.0f};
            force = glm::vec2 {0.0f, 0.0f};
        }

        void addForce(const glm::vec2& addedForce) {
            if (staticBody) {
                return;
            }
            force += addedForce;
        }

        void clearForces() {
            force = glm::vec2 {0.0f, 0.0f};
        }

        [[nodiscard]] float getMass() const {
            return mass;
        }

        [[nodiscard]] float getInverseMass() const {
            return inverseMass;
        }

        [[nodiscard]] bool isStatic() const {
            return staticBody;
        }

    private:
        float mass {1.0f};
        float inverseMass {1.0f};
        bool staticBody {false};
    };
}
