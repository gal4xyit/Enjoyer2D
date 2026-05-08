#include "app/TestbedApp.hpp"

int main() {
    TestbedApp app;

    if (!app.init()) {
        return 1;
    }

    app.run();

    return 0;
}