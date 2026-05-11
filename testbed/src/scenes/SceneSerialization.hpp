#pragma once

#include <Enjoyer2D/World.hpp>

#include <string>

bool saveWorldToFile(const Enjoyer2D::World& world, const std::string& path);

bool loadWorldFromFile(Enjoyer2D::World& world, const std::string& path);
