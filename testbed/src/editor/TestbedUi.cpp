#include "TestbedUi.hpp"

#include "app/TestbedActions.hpp"

#include <Enjoyer2D/Body.hpp>
#include <Enjoyer2D/Shape.hpp>

#include <imgui.h>
#include <glm/vec2.hpp>
#include <cstddef>
#include <cstdio>

void drawTestbedUi(TestbedState& testbed, DebugRenderSettings& debugRender) {
    ImGui::Begin("Enjoyer2D Testbed");

        ImGui::Text("Scene: %s", testbed.scene.name());

        if (ImGui::BeginCombo("Scene", testbed.scene.name())) {
            for (int i = 0; i < SceneState::count(); ++i) {
                SceneId scene = SceneState::sceneAt(i);
                const bool isSelected = scene == testbed.scene.current;

                if (ImGui::Selectable(sceneName(scene), isSelected)) {
                    loadTestbedScene(testbed, scene);
                }

                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndCombo();
        }

        if (ImGui::Checkbox("Paused", &testbed.sim.paused)) {
        }

        ImGui::SameLine();

        if (ImGui::Button("Reset")) {
            testbed.sim.resetRequest = true;
        }

        ImGui::SameLine();

        ImGui::BeginDisabled(!testbed.sim.paused);
        if (ImGui::Button("Step")) {
            testbed.sim.singleStepRequest = true;
        }
        ImGui::EndDisabled();

        ImGui::Separator();

        ImGui::Text("Bodies: %zu", testbed.world.getBodyCount());
        ImGui::Text("Contacts: %zu", testbed.world.getContactCount());

        ImGui::Separator();

        if (ImGui::CollapsingHeader("Bodies")) {
            auto& bodies = testbed.world.getBodies();

            for (std::size_t i = 0; i < bodies.size(); ++i) {
                const Enjoyer2D::Body& body = bodies[i];

                const char* shapeName = body.shape.type == Enjoyer2D::ShapeType::Box ? "Box" : "Circle";

                char label[64];
                std::snprintf(label, sizeof(label), "%s %zu", shapeName, i);

                const bool selected = testbed.editor.selectedBodyIndex == static_cast<int>(i);

                if (ImGui::Selectable(label, selected)) {
                    testbed.editor.selectedBodyIndex = static_cast<int>(i);
                }
            }
        }

        const float fps = testbed.stats.frameDt > 0.0f ? 1.0f / testbed.stats.frameDt : 0.0f;
        ImGui::Text("FPS: %.1f", fps);
        ImGui::Text("Physics: %.3f ms", testbed.stats.physicsStepMs);
        ImGui::Text("Physics steps: %d", testbed.stats.physicsStepsThisFrame);

        ImGui::Separator();

        ImGui::Checkbox("Centers", &debugRender.showCenters);
        ImGui::Checkbox("Velocity vectors", &debugRender.showVelocities);
        ImGui::Checkbox("Paths", &debugRender.showPath);
        ImGui::Checkbox("AABBs", &debugRender.showAabbs);
        ImGui::Checkbox("Contact points", &debugRender.showContactPoints);
        ImGui::Checkbox("Contact normals", &debugRender.showContactNormals);

        ImGui::SliderFloat("Velocity scale", &debugRender.velocityScale, 0.0f, 2.0f);
        ImGui::SliderFloat("Normal scale", &debugRender.normalScale, 0.0f, 2.0f);

        ImGui::Separator();

        if (ImGui::CollapsingHeader("Physics settings", ImGuiTreeNodeFlags_DefaultOpen)) {
            glm::vec2 gravity = testbed.world.getGravity();

            if (ImGui::DragFloat2("Gravity", &gravity.x, 0.1f)) {
                testbed.world.setGravity(gravity);
            }

            int velocityIterations = testbed.world.getVelocityIterations();
            if (ImGui::SliderInt("Velocity iterations", &velocityIterations, 1, 30)) {
                testbed.world.setVelocityIterations(velocityIterations);
            }

            int positionIterations = testbed.world.getPositionIterations();
            if (ImGui::SliderInt("Position iterations", &positionIterations, 1, 20)) {
                testbed.world.setPositionIterations(positionIterations);
            }
        }

        ImGui::Separator();

        if (ImGui::CollapsingHeader("Tools", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::RadioButton("Select", testbed.editor.activeTool == EditorTool::Select)) {
                testbed.editor.activeTool = EditorTool::Select;
            }

            if (ImGui::RadioButton("Spawn box", testbed.editor.activeTool == EditorTool::SpawnBox)) {
                testbed.editor.activeTool = EditorTool::SpawnBox;
            }

            if (ImGui::RadioButton("Spawn circle", testbed.editor.activeTool == EditorTool::SpawnCircle)) {
                testbed.editor.activeTool = EditorTool::SpawnCircle;
            }
        }
        if (testbed.editor.statusMessage[0] != '\0') {
            ImGui::TextUnformatted(testbed.editor.statusMessage);
        }

        if (testbed.editor.activeTool == EditorTool::SpawnBox || testbed.editor.activeTool == EditorTool::SpawnCircle) {

            ImGui::Separator();
            ImGui::Text("Spawn settings");

            if (testbed.editor.activeTool == EditorTool::SpawnBox) {
                ImGui::DragFloat("Box width", &testbed.editor.spawn.boxWidth, 0.05f, 0.1f, 10.0f);
                ImGui::DragFloat("Box height", &testbed.editor.spawn.boxHeight, 0.05f, 0.1f, 10.0f);
            }

            if (testbed.editor.activeTool == EditorTool::SpawnCircle) {
                ImGui::DragFloat("Circle radius", &testbed.editor.spawn.circleRadius, 0.05f, 0.1f, 10.0f);
            }

            ImGui::DragFloat("Mass##Spawn", &testbed.editor.spawn.mass, 0.1f, 0.1f, 100.0f);
            ImGui::DragFloat2("Velocity##Spawn", &testbed.editor.spawn.velocity.x, 0.05f, -20.0f, 20.0f);
            ImGui::DragFloat("Friction##Spawn", &testbed.editor.spawn.friction, 0.01f, 0.0f, 2.0f);
            ImGui::DragFloat("Restitution##Spawn", &testbed.editor.spawn.restitution, 0.01f, 0.0f, 1.0f);
            ImGui::Checkbox("Static##Spawn", &testbed.editor.spawn.isStatic);

            ImGui::SameLine();

            ImGui::Checkbox("Show spawn preview", &testbed.editor.showSpawnPreview);

            ImGui::SameLine();

            if (ImGui::Button("Reset##SpawnSettings")) {
                testbed.editor.spawn.reset();
            }
    }

        ImGui::End();

        auto& bodies = testbed.world.getBodies();

        if (testbed.editor.selectedBodyIndex >= static_cast<int>(bodies.size())) {
            testbed.editor.selectedBodyIndex = -1;
        }

        if (testbed.editor.selectedBodyIndex != -1) {
            Enjoyer2D::Body& body = bodies[testbed.editor.selectedBodyIndex];

            ImGui::Begin("Body Inspector");
            bool bodyStorageChanged = false;

            const char* shapeName = body.shape.type == Enjoyer2D::ShapeType::Box ? "Box" : "Circle";

            ImGui::Text("Selected body: %s %d", shapeName, testbed.editor.selectedBodyIndex);

            ImGui::DragFloat2("Position", &body.position.x, 0.05f);
            ImGui::DragFloat2("Velocity", &body.velocity.x, 0.05f);
            ImGui::DragFloat2("Force", &body.force.x, 0.05f);

            bool isStatic = body.isStatic();
            if (ImGui::Checkbox("Static", &isStatic)) {
                if (isStatic) {
                    body.setStatic();
                } else {
                    body.setMass(1.0f);
                }
            }

            if (!body.isStatic()) {
                float mass = body.getMass();
                if (ImGui::DragFloat("Mass", &mass, 0.1f, 0.1f, 100.0f)) {
                    body.setMass(mass);
                }
            }

            ImGui::DragFloat("Restitution", &body.restitution, 0.01f, 0.0f, 1.0f);
            ImGui::DragFloat("Friction", &body.friction, 0.01f, 0.0f, 2.0f);

            if (body.shape.type == Enjoyer2D::ShapeType::Box) {
                ImGui::DragFloat("Width", &body.shape.box.width, 0.05f, 0.1f, 20.0f);
                ImGui::DragFloat("Height", &body.shape.box.height, 0.05f, 0.1f, 20.0f);
            }

            if (body.shape.type == Enjoyer2D::ShapeType::Circle) {
                ImGui::DragFloat("Radius", &body.shape.circle.radius, 0.05f, 0.1f, 20.0f);
            }

            if (ImGui::Button("Clear selection")) {
                testbed.editor.selectedBodyIndex = -1;
            }

            ImGui::SameLine();

            if (ImGui::Button("Delete body")) {
                testbed.world.destroyBody(static_cast<Enjoyer2D::BodyHandle>(testbed.editor.selectedBodyIndex));

                testbed.editor.selectedBodyIndex = -1;
                resetDebugTrails(testbed.debug, testbed.world);
                bodyStorageChanged = true;
            }

            ImGui::SameLine();

            if (ImGui::Button("Duplicate")) {
                Enjoyer2D::BodyDef def;

                def.position = body.position + glm::vec2 {0.5f, 0.5f};
                def.velocity = body.velocity;
                def.shape = body.shape;
                def.mass = body.getMass();
                def.friction = body.friction;
                def.restitution = body.restitution;
                def.isStatic = body.isStatic();

                testbed.editor.selectedBodyIndex = static_cast<int>(testbed.world.createBody(def));
                resetDebugTrails(testbed.debug, testbed.world);
                bodyStorageChanged = true;
            }

            if (!bodyStorageChanged) {
                ImGui::Separator();

                if (ImGui::CollapsingHeader("Actions", ImGuiTreeNodeFlags_DefaultOpen)) {
                    if (!body.isStatic()) {
                        if (ImGui::Button("Push right")) {
                            body.velocity.x += 5.0f;
                        }

                        ImGui::SameLine();

                        if (ImGui::Button("Push left")) {
                            body.velocity.x -= 5.0f;
                        }

                        if (ImGui::Button("Jump")) {
                            body.velocity.y += 5.0f;
                        }

                        if (ImGui::Button("Apply force right")) {
                            body.addForce({100.0f, 0.0f});
                        }

                        ImGui::SameLine();

                        if (ImGui::Button("Apply force up")) {
                            body.addForce({0.0f, 100.0f});
                        }

                        if (ImGui::Button("Stop")) {
                            body.velocity = {0.0f, 0.0f};
                            body.clearForces();
                        }
                    } else {
                        ImGui::Text("Static bodies cannot be pushed.");
                    }
                }
            }
            ImGui::End();
        }
}