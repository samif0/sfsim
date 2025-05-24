#ifndef SIM_HPP
#define SIM_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "ecs/entity.hpp"
#include "camera.hpp"
#include "math/vector.hpp"
#include "math/matrix.hpp"

namespace SFSim {

using namespace Math;
using namespace ECS;

struct sim_config {
    sf::String title;
    unsigned int win_width;
    unsigned int win_height;
};

// TODO: Implement an entity-component-system (ECS) architecture for better simulation organization
// HINT: Consider creating:
//       1. Entity class - just an ID and list of components
//       2. Component base class with derived types (Transform, Renderer, Physics, etc.)
//       3. System classes that operate on entities with specific components
//          (RenderSystem, PhysicsSystem, etc.)
//       4. World/Scene class to manage all entities
//
// IMPL: Create a proper scene graph to manage hierarchical relationships between objects
// HINT: For scene graph, make a Node class with:
//       - Transform (position, rotation, scale)
//       - Parent node pointer
//       - Children node vector
//       - Methods to add/remove children, set parent, etc.
class sim {
private:
    sim_config scfg;
    sf::RenderWindow* window;
    std::vector<Entity> entities;
    Camera camera;
    float fTheta = 0.0f;
    sf::Vector2i lastMousePosition;
    bool firstMouse = true;

    Vector3f projectPoint(const Vector3f& point, const Matrix4x4& viewProjection);

public:
    // TODO: Implement a resource manager to handle window creation and other resources
    // HINT: Create a ResourceManager singleton class that:
    //       1. Loads and caches resources (textures, meshes, shaders, etc.)
    //       2. Has methods like GetTexture(filename), GetMesh(filename), etc.
    //       3. Handles cleaning up resources when they're no longer needed
    //       4. Could manage the window creation too
    sim(const sim_config& _scfg) : scfg(_scfg) {
        unsigned int window_width = this->scfg.win_width;
        unsigned int window_height = this->scfg.win_height;
        sf::String window_title = this->scfg.title;

        this->window =
            new sf::RenderWindow(sf::VideoMode({window_width, window_height}), window_title);
        this->window->setFramerateLimit(60);
        
        // Initialize camera with proper aspect ratio
        float aspect = static_cast<float>(window_width) / static_cast<float>(window_height);
        camera.setPerspective(M_PI / 4.0f, aspect, 0.1f, 1000.0f);
        camera.setPosition(Vector3f(0, 0, 10));
        camera.setTarget(Vector3f(0, 0, 0));
    }
    ~sim() {
        if (window != nullptr) {
            delete window;
            window = nullptr;
        }
    }

    // TODO: Implement a scene manager for better object organization and initialization
    // HINT: Create a Scene class that:
    //       1. Stores all scene objects (entities, lights, camera, etc.)
    //       2. Has methods to add/remove objects, find objects by ID/name
    //       3. Manages scene loading/saving to files
    //       4. Handles initialization of all objects
    template<typename T>
    void init(const T& entities) {
        // Legacy compatibility - modern code should use addEntity directly
    }

    // TODO: Implement a proper render pipeline with configurable passes
    // HINT: Create a RenderPipeline class with:
    //       1. A series of RenderPass objects (shadow pass, geometry pass, post-processing, etc.)
    //       2. Each pass has its own render target, shader, and draw method
    //       3. Pipeline controls the order of passes and dependencies
    void prerender();
    void render();
    void postrender();

    // TODO: Create a simulation time manager to handle timing and callbacks
    // HINT: Create a TimeManager class that:
    //       1. Tracks game time, real time, delta time, etc.
    //       2. Handles fixed timestep updates separate from variable rendering
    //       3. Supports pausing, time scaling, etc.
    //       4. Provides a way to register callbacks for timed events
    void start();
    void stop();
    void pause();
    void resume();
    // TODO: Implement proper entity management with a scene graph
    // HINT: This method should:
    //       1. Add the entity to a scene/world container
    //       2. Set up parent-child relationships in the scene graph if needed
    //       3. Initialize any components the entity has
    //       4. Perhaps assign a unique ID to the entity
    // IMPL: Reduce debug prints in production code
    // HINT: Use a logging system with different levels (DEBUG, INFO, WARNING, ERROR)
    //       that can be configured at runtime or compile time
    // Add entity to the simulation
    void addEntity(Entity entity) {
        entities.push_back(std::move(entity));
    }
    
    void addCamera(const Vector3f& position, const Vector3f& target = Vector3f(0, 0, 0), const Vector3f& up = Vector3f::up()) {
        camera.lookAt(position, target, up);
    }
    
    Camera& getCamera() { return camera; }
    const Camera& getCamera() const { return camera; }
};

}; // namespace SFSim

#endif
