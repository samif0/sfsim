#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include "../drawable.hpp"
#include "../point3d.hpp"
#include "../triangle.hpp"

// TODO: Implement a proper entity-component system
// HINT: A good ECS architecture would have:
//       1. Entity class - just an ID and list of components
//       2. Component base class with derived types
//       3. Systems that process entities with specific components
//
// Example structure:
// class Component { public: virtual ~Component() = default; };
// 
// class TransformComponent : public Component {
//     sf::Vector3f position;
//     sf::Vector3f rotation;
//     sf::Vector3f scale = {1,1,1};
// };
//
// class RenderComponent : public Component {
//     Mesh* mesh;
//     Material* material;
// };
//
// class Entity {
//     std::vector<std::unique_ptr<Component>> components;
// public:
//     template<typename T> T* addComponent();
//     template<typename T> T* getComponent();
// };

class entity : public drawable {
public:
    // TODO: Store components instead of direct geometry
    // HINT: Instead of directly storing triangles, consider storing components
    //       that describe what the entity is and how it behaves
    entity(std::vector<triangle*>& tris) : tris(tris) {};
    ~entity() {};

    std::vector<triangle*>& tris;

    // TODO: Implement proper entity drawing based on components
    // HINT: The draw method should:
    //       1. Check if the entity has a render component
    //       2. Get the transform component for position/rotation
    //       3. Apply the transformation to the rendering
    //       4. Use the material/shader properties for appearance
    void draw(sf::RenderWindow& window) const override {
        for (triangle* t : tris) {
            t->draw(window);
        }
    };
    // TODO: Implement proper transformation system
    // HINT: Scale should be part of a transform component that:
    //       1. Stores position, rotation, and scale
    //       2. Can calculate a transformation matrix
    //       3. Handles parent-child relationships
    void scale(sf::Vector2u windowSize) override {
        for (triangle* t : tris) {
            t->scale(windowSize);
        }
    };

    // TODO: Replace with proper entity vertex data management
    // HINT: This method is converting triangles to points, but a better approach would be:
    //       1. Store vertex data in a proper vertex buffer
    //       2. Use indices to define triangles that reference those vertices
    //       3. Have a dedicated renderer that handles drawing efficiently
    //       4. Use a proper logging system instead of cout statements
    std::vector<point3d> to_free_points() {
        std::cout << "Converting entity to free points..." << std::endl;
        std::vector<point3d> free_points;
        for (triangle* t : tris) {
            free_points.push_back(t->vertices[0]);
            free_points.push_back(t->vertices[1]);
            free_points.push_back(t->vertices[2]);
        }
        std::cout << "Converted entity to free points" << std::endl;
        return free_points;
    }

private:
};

#endif
