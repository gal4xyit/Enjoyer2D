#pragma once

struct DebugRenderSettings {
    bool showCenters = false;
    bool showVelocities = false;
    bool showAabbs = false;
    bool showPath = false;
    bool showContactPoints = false;
    bool showContactNormals = false;

    float velocityScale = 0.25f;
    float normalScale = 0.5f;
};
