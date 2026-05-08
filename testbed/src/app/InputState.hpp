#pragma once

struct InputState {
    bool cameraUp = false;
    bool cameraDown = false;
    bool cameraLeft = false;
    bool cameraRight = false;

    bool zoomIn = false;
    bool zoomOut = false;

    bool quitRequested = false;

    int sceneChange = 0;
};
