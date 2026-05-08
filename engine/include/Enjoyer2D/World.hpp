#pragma once

#include <Enjoyer2D/Body.hpp>
#include <Enjoyer2D/Collision.hpp>
#include <Enjoyer2D/Contact.hpp>
#include <Enjoyer2D/WorldStats.hpp>

#include <glm/vec2.hpp>
#include <glm/geometric.hpp>

#include <cmath>
#include <vector>
#include <algorithm>
#include <cstddef>

namespace Enjoyer2D {
    using BodyHandle = std::size_t;

    class World {
        public:

        BodyHandle createBody() {
            bodies.emplace_back();
            return bodies.size() - 1;
        }

        BodyHandle createBody(const BodyDef& def) {
            const BodyHandle handle = createBody();
            Body& body = getBody(handle);

            body.position = def.position;
            body.velocity = def.velocity;
            body.shape = def.shape;
            body.restitution = def.restitution;
            body.friction = def.friction;

            if (def.isStatic) {
                body.setStatic();
            }else {
                body.setMass(def.mass);
            }

            return handle;
        }


        void setGravity(const glm::vec2& newGravity) {
            gravity = newGravity;
        }

        void setVelocityIterations(int iterations) {
            velocityIterations = std::max(iterations, 1);
        }

        void setPositionIterations(int iterations) {
            positionIterations = std::max(iterations, 1);
        }

        [[nodiscard]] Body& getBody(BodyHandle handle) {
            return bodies.at(handle);
        }

        [[nodiscard]] const Body& getBody(BodyHandle handle) const {
            return bodies.at(handle);
        }

        [[nodiscard]] const glm::vec2& getGravity() const {
            return gravity;
        }

        [[nodiscard]] std::vector<Body>& getBodies() {
            return bodies;
        }

        [[nodiscard]] const std::vector<Body>& getBodies() const {
            return bodies;
        }

        [[nodiscard]] const std::vector<Contact>& getContacts() const {
            return contacts;
        }

        [[nodiscard]] std::size_t getBodyCount() const {
            return bodies.size();
        }

        [[nodiscard]] std::size_t getContactCount() const {
            return contacts.size();
        }

        [[nodiscard]] int getVelocityIterations() const {
            return velocityIterations;
        }

        [[nodiscard]] int getPositionIterations() const {
            return positionIterations;
        }

        [[nodiscard]] WorldStats getStats() const {
            return {
                bodies.size(),
                contacts.size(),
                velocityIterations,
                positionIterations
            };
        }

        void clear() {
            bodies.clear();
            contacts.clear();
        }

        void destroyBody(BodyHandle handle) {
            if (handle >= bodies.size()) {
                return;
            }

            bodies.erase(bodies.begin() + static_cast<std::ptrdiff_t>(handle));
            contacts.clear();
        }

        void step(float dt) {
            if (dt <= 0) {
                return;
            }

            contacts.clear();

            for (Body& body : bodies) {
                integrateBody(body, dt);
            }

            detectCollision();

            for (int i = 0; i < positionIterations; ++i) {
                correctPosition();
            }

            for (int i = 0; i < velocityIterations; ++i) {
                resolveCollision();
            }
        }

        private:
        std::vector<Body> bodies;
        std::vector<Contact> contacts;
        glm::vec2 gravity {0.0f, -9.8f};
        int velocityIterations {8};
        int positionIterations {3};

        void integrateBody(Body& body, float dt) const{
            if (body.isStatic() || body.getInverseMass() <= 0.0f) {
                body.clearForces();
                return;
            }

            glm::vec2 acceleration = gravity + body.force * body.getInverseMass();

            body.velocity += acceleration * dt;
            body.position += body.velocity * dt;

            body.clearForces();
        }

        void detectCollision() {
            for (std::size_t i = 0; i < bodies.size(); ++i) {
                for (std::size_t j = i + 1; j < bodies.size(); ++j) {
                    Contact contact = collideBodies(bodies[i], bodies[j]);

                    if (contact.collided) {
                        contact.bodyA = i;
                        contact.bodyB = j;
                        contacts.push_back(contact);
                    }
                }
            }
        }

        void correctPosition() {
            constexpr float percent = 0.2f;
            constexpr float slop = 0.04f;

            for (const Contact& contact : contacts) {
                Body& a = bodies[contact.bodyA];
                Body& b = bodies[contact.bodyB];

                const float totalInverseMass = a.getInverseMass() + b.getInverseMass();

                if (totalInverseMass <= 0.0f) {
                    continue;
                }

                const float correctMagnitude = std::max(contact.penetration - slop, 0.0f) / totalInverseMass * percent;

                const glm::vec2 correction = correctMagnitude * contact.normal;

                a.position -= correction * a.getInverseMass();
                b.position += correction * b.getInverseMass();
            }
        }

        void resolveCollision() {
            for (const Contact& contact : contacts) {
                Body& a = bodies[contact.bodyA];
                Body& b = bodies[contact.bodyB];

                const float totalInverseMass = a.getInverseMass() + b.getInverseMass();

                if (totalInverseMass <= 0.0f) {
                    continue;
                }

                const glm::vec2 relativeVelocity = b.velocity - a.velocity;
                const float velocityAlongNormal = glm::dot(relativeVelocity, contact.normal);

                if (velocityAlongNormal > 0.0f) {
                    continue;
                }

                const float restitution = std::min(a.restitution, b.restitution);

                const float impulseMagnitude = -(1.0f + restitution) * velocityAlongNormal/totalInverseMass;

                const glm::vec2 impulse = impulseMagnitude * contact.normal;

                a.velocity -= impulse * a.getInverseMass();
                b.velocity += impulse * b.getInverseMass();

                const glm::vec2 relativeVelocityAfterNormalImpulse = b.velocity - a.velocity;

                glm::vec2 tangent = relativeVelocityAfterNormalImpulse - glm::dot(relativeVelocityAfterNormalImpulse, contact.normal) * contact.normal;

                if (glm::length(tangent) > 0.0f) {
                    tangent = glm::normalize(tangent);

                    const float velocityAlongTangent = glm::dot(relativeVelocityAfterNormalImpulse, tangent);

                    const float frictionImpulseMagnitude = -velocityAlongTangent / totalInverseMass;

                    const float friction = std::sqrt(a.friction * b.friction);

                    const float maxFrictionImpulse = impulseMagnitude * friction;

                    const float clampedFrictionImpulseMagnitude = std::clamp(frictionImpulseMagnitude, -maxFrictionImpulse, maxFrictionImpulse);

                    const glm::vec2 frictionImpulse = clampedFrictionImpulseMagnitude * tangent;

                    a.velocity -= frictionImpulse * a.getInverseMass();
                    b.velocity += frictionImpulse * b.getInverseMass();
                }
            }
        }
    };
}
