#include "scenes/SceneSerialization.hpp"

#include <Enjoyer2D/Body.hpp>
#include <Enjoyer2D/Shape.hpp>

#include <nlohmann/json.hpp>
#include <glm/vec2.hpp>

#include <fstream>
#include <string>
#include <filesystem>

using json = nlohmann::json;

json bodyToJson(const Enjoyer2D::Body& body) {

    json bodyJson;

    bodyJson["position"] = {body.position.x, body.position.y};
    bodyJson["velocity"] = {body.velocity.x, body.velocity.y};
    bodyJson["mass"] = body.getMass();
    bodyJson["static"] = body.isStatic();
    bodyJson["friction"] = body.friction;
    bodyJson["restitution"] = body.restitution;

    if (body.shape.type == Enjoyer2D::ShapeType::Box) {
        bodyJson["shape"] = {{"type", "box"}, {"width", body.shape.box.width}, {"height", body.shape.box.height}};
    }

    if (body.shape.type == Enjoyer2D::ShapeType::Circle) {
        bodyJson["shape"] = {{"type", "circle"}, {"radius", body.shape.circle.radius}};
    }

    return bodyJson;
}

glm::vec2 vec2FromJson(const json& value) {
    return {value.at(0).get<float>(), value.at(1).get<float>()};
}

Enjoyer2D::Shape shapeFromJson(const json& value) {
    const std::string type = value.at("type").get<std::string>();

    if (type == "circle") {
        return Enjoyer2D::Shape::makeCircle(value.at("radius").get<float>());
    }

    return Enjoyer2D::Shape::makeBox(value.at("width").get<float>(), value.at("height").get<float>());
}

Enjoyer2D::BodyDef bodyDefFromJson(const json& value) {
    Enjoyer2D::BodyDef def;

    def.position = vec2FromJson(value.at("position"));

    if (value.contains("velocity")) {
        def.velocity = vec2FromJson(value.at("velocity"));
    }

    def.mass = value.value("mass", 1.0f);
    def.isStatic = value.value("static", false);
    def.friction = value.value("friction", 0.5f);
    def.restitution = value.value("restitution", 0.0f);
    def.shape = shapeFromJson(value.at("shape"));

    return def;
}

// File stores world setting + definitions of bodies in it
bool saveWorldToFile(const Enjoyer2D::World &world, const std::string &path) {
    json sceneJson;

    const glm::vec2 gravity = world.getGravity();

    sceneJson["gravity"] = {gravity.x, gravity.y};
    sceneJson["velocityIterations"] = world.getVelocityIterations();
    sceneJson["positionIterations"] = world.getPositionIterations();
    sceneJson["bodies"] = json::array();

    for (const Enjoyer2D::Body &body : world.getBodies()) {
        sceneJson["bodies"].push_back(bodyToJson(body));
    }

    const std::filesystem::path outputPath(path);

    if (outputPath.has_parent_path()) {
        std::filesystem::create_directories(outputPath.parent_path());
    }

    std::ofstream file(outputPath);

    if (!file.is_open()) {
        return false;
    }

    file << sceneJson.dump(4);

    return true;
}

bool loadWorldFromFile(Enjoyer2D::World &world, const std::string &path) {
    std::ifstream file(path);

    if (!file.is_open()) {
        return false;
    }

    json sceneJson;

    file >> sceneJson;

    world.clear();

    world.setGravity(vec2FromJson(sceneJson["gravity"]));
    world.setVelocityIterations(sceneJson["velocityIterations"].get<int>());
    world.setPositionIterations(sceneJson["positionIterations"].get<int>());

    for (const json& bodyJson : sceneJson["bodies"]) {
        world.createBody(bodyDefFromJson(bodyJson));
    }

    return true;
}
