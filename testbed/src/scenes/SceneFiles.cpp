#include "scenes/SceneFiles.hpp"

#include <algorithm>
#include <cctype>
#include <string>

std::filesystem::path sceneDirectory() {
    return "saved_scenes";
}

// Adjusts user's file naming to simple and safe ones ("test file" -> "test_file")
std::string sanitizeSceneName(const char* rawName) {
    std::string clean;

    if (rawName != nullptr) {
        for (char ch : std::string(rawName)) {
            const unsigned char value = static_cast<unsigned char>(ch);

            if (std::isalnum(value) || ch == '_' || ch == '-') {
                clean += ch;
            } else if (std::isspace(value)) {
                clean += '_';
            }
        }
    }

    if (clean.empty()) {
        clean = "scene";
    }

    return clean;
}

std::filesystem::path scenePathFromName(const char* name) {
    std::filesystem::path path = sceneDirectory() / sanitizeSceneName(name);
    path.replace_extension(".json");
    return path;
}

void refreshSceneFiles(SceneFileState& sceneFiles) {
    sceneFiles.files.clear();

    std::filesystem::create_directories(sceneDirectory());

    for (const auto& entry : std::filesystem::directory_iterator(sceneDirectory())) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") {
            sceneFiles.files.push_back(entry.path());
        }
    }

    std::sort(sceneFiles.files.begin(), sceneFiles.files.end());

    if (sceneFiles.files.empty()) {
        sceneFiles.selectedIndex = -1;
    } else if (sceneFiles.selectedIndex < 0 || sceneFiles.selectedIndex >= static_cast<int>(sceneFiles.files.size())) {
        sceneFiles.selectedIndex = 0;
    }
}

const std::filesystem::path* selectedSceneFile(const SceneFileState& sceneFiles) {
    if (sceneFiles.selectedIndex < 0 ||sceneFiles.selectedIndex >= static_cast<int>(sceneFiles.files.size())) {
        return nullptr;
    }

    return &sceneFiles.files[sceneFiles.selectedIndex];
}

void selectSceneFile(SceneFileState& sceneFiles, const std::filesystem::path& path) {
    for (int i = 0; i < static_cast<int>(sceneFiles.files.size()); ++i) {
        if (sceneFiles.files[i] == path) {
            sceneFiles.selectedIndex = i;
            return;
        }
    }
}
