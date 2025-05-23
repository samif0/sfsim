#ifndef SIM_HPP
#define SIM_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

#include "mesh.hpp"
#include "point3d.hpp"
#include "simobj/entity.hpp"

// TODO: Create a dedicated math utility class for matrix operations
// HINT: Consider creating a 'math.hpp' with Matrix4x4 class that has methods like:
//       - Matrix multiplication (operator*)
//       - CreateRotationX/Y/Z(angle)
//       - CreateTranslation(x,y,z)
//       - CreateProjection(fov, aspect, near, far)
struct mat4x4 {
    float mat[4][4]{};
};

// TODO: Create a camera class that handles view transformations and projection matrices
// HINT: Create a 'camera.hpp' with Camera class that contains:
//       - Position (Vector3f) and orientation (maybe quaternions or Euler angles)
//       - Methods like SetPosition(), LookAt(), SetFOV(), etc.
//       - GetViewMatrix() and GetProjectionMatrix() to generate required matrices
//       - Movement methods like MoveForward(), Rotate(), etc.
struct sim_config {
    sf::String title;
    unsigned int win_width;
    unsigned int win_height;
    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFovj = 90.0f;
    float fAspectRatio = float(win_height) / float(win_width);
    float fFovRad = 1.0f / tanf(fFovj * 0.5f / 180.0f * M_PI);
    float fElapsedTime = 0.1f;

    mat4x4 proj_mat = {{{fAspectRatio * fFovRad, 0, 0, 0},
                        {0, fFovRad, 0, 0},
                        {0, 0, fFar / (fFar - fNear), 1.0f},
                        {0, 0, (-fFar * fNear) / (fFar - fNear), 0}}};
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
    sim_config* scfg;
    sf::RenderWindow* window;
    std::vector<mesh> meshes;
    std::vector<point3d> free_points;
    float fTheta = 0.0f;  // TODO: Move rotation logic to a proper transform class

    // TODO: Move matrix operations to math utility class
    void proj(sf::Vector3f& in, sf::Vector3f& out, mat4x4& pm);

public:
    // TODO: Implement a resource manager to handle window creation and other resources
    // HINT: Create a ResourceManager singleton class that:
    //       1. Loads and caches resources (textures, meshes, shaders, etc.)
    //       2. Has methods like GetTexture(filename), GetMesh(filename), etc.
    //       3. Handles cleaning up resources when they're no longer needed
    //       4. Could manage the window creation too
    sim(sim_config* _scfg) : scfg(_scfg) {
        unsigned int window_width = this->scfg->win_width;
        unsigned int window_height = this->scfg->win_height;
        sf::String window_title = this->scfg->title;

        this->window =
            new sf::RenderWindow(sf::VideoMode({window_width, window_height}), window_title);
        this->window->setFramerateLimit(60);
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
    void init(std::vector<mesh>& meshes, std::vector<point3d>& free_points) {
        this->meshes = meshes;
        this->free_points = free_points;
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
    void add_entity(entity& e) {
        std::cout << "Adding entity to free points..." << std::endl;
        this->free_points.insert(this->free_points.end(), e.to_free_points().begin(),
                                 e.to_free_points().end());
        std::cout << "Added entity to free points" << std::endl;
    }
};

#endif
