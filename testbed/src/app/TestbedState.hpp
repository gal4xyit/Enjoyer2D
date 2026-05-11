#pragma once

#include <Enjoyer2D/World.hpp>
#include "rendering/DebugState.hpp"
#include "scenes/SceneState.hpp"

#include <glm/vec2.hpp>

#include <filesystem>
#include <vector>
#include <string>

struct SimulationState {
    bool paused = false;
    bool singleStepRequest = false;
    bool resetRequest = false;

    float accumulator = 0.0f;
    float fixedDt = 1.0f/60.0f;
};

struct ControlSettings {
    float cameraSpeed = 8.0f;
    float zoomSpeed = 60.0f;

    float minZoom = 20.0f;
    float maxZoom = 100.0f;
};

struct PerformanceStats {
    float frameDt = 0.0f;
    float physicsStepMs = 0.0f;
    int physicsStepsThisFrame = 0;
};

enum class EditorTool {
    Select,
    SpawnBox,
    SpawnCircle
};

struct SpawnSettings {
    float boxWidth = 1.0f;
    float boxHeight = 1.0f;
    float circleRadius = 0.5f;

    float mass = 1.0f;
    float friction = 0.5f;
    float restitution = 0.0f;

    glm::vec2 velocity {0.0f, 0.0f};
    bool isStatic = false;

    void reset() {
        *this = SpawnSettings {};
    }
};

struct SceneFileState {
    std::vector<std::filesystem::path> files;
    int selectedIndex = -1;
    char saveName[128] = "new_scene";
};

struct EditorState {
    int selectedBodyIndex = -1;
    EditorTool activeTool = EditorTool::Select;
    SpawnSettings spawn;

    bool showSpawnPreview = true;
    bool hasMouseWorld = false;
    glm::vec2 mouseWorld {0.0f, 0.0f};

    std::string statusMessage;

    SceneFileState sceneFiles;
};

struct TestbedState {
    Enjoyer2D::World world;
    SceneState scene;
    SimulationState sim;
    ControlSettings controls;
    PerformanceStats stats;
    DebugState debug;
    EditorState editor;
};