#pragma once

#include "app/TestbedState.hpp"

#include <filesystem>

std::filesystem::path sceneDirectory();

std::filesystem::path scenePathFromName(const char* name);

void refreshSceneFiles(SceneFileState& sceneFiles);

const std::filesystem::path* selectedSceneFile(const SceneFileState& sceneFiles);

void selectSceneFile(SceneFileState& sceneFiles, const std::filesystem::path& path);
