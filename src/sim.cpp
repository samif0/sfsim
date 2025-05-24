#include "sim.hpp"

#include "ecs/entity.hpp"
#include "ecs/transform_component.hpp"
#include "ecs/render_component.hpp"
#include "geometry/mesh.hpp"
#include "geometry/point.hpp"
#include "math/matrix.hpp"
#include "math/vector.hpp"
#include "camera.hpp"
#include "core/time.hpp"


using namespace SFSim;
using namespace SFSim::Math;
using namespace SFSim::ECS;
using namespace SFSim::Core;

// Modern projection using Matrix4x4 and Vector3f
Vector3f sim::projectPoint(const Vector3f& point, const Matrix4x4& viewProjection) {
    return viewProjection.transformPoint(point);
}

void sim::prerender() {
    // Update entities and systems
    Time::getInstance().update();
    float deltaTime = Time::getInstance().getDeltaTime();
    
    for (auto& entity : entities) {
        entity.update(deltaTime);
    }
}

void sim::render() {
    // Update rotation for animation
    fTheta += 1.0f * Time::getInstance().getDeltaTime();
    
    // Create view and projection matrices using modern math library
    Matrix4x4 viewMatrix = camera.getViewMatrix();
    Matrix4x4 projMatrix = camera.getProjectionMatrix();
    Matrix4x4 viewProjection = projMatrix * viewMatrix;
    
    // Render all entities using ECS system
    for (auto& entity : entities) {
        auto* transform = entity.getComponent<TransformComponent>();
        auto* render = entity.getComponent<RenderComponent>();
        
        if (transform && render && render->isVisible() && render->getGeometry()) {
            // Get world transformation matrix
            Matrix4x4 worldMatrix = transform->getWorldMatrix();
            
            // Render the geometry
            render->getGeometry()->draw(*window, worldMatrix, viewProjection);
        }
    }
}

void sim::postrender() {
    // Post-render cleanup if needed
}

void sim::start() {
    // Initialize time system
    Time::getInstance().initialize();
    
    while (window->isOpen()) {
        while (const std::optional event = window->pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window->close();
            } else if (const auto* key_pressed = event->getIf<sf::Event::KeyPressed>()) {
                if (key_pressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window->close();
                }
                // Camera movement controls
                float moveSpeed = 2.0f * Time::getInstance().getDeltaTime();
                if (key_pressed->scancode == sf::Keyboard::Scancode::W) {
                    camera.moveForward(moveSpeed);
                } else if (key_pressed->scancode == sf::Keyboard::Scancode::S) {
                    camera.moveForward(-moveSpeed);
                } else if (key_pressed->scancode == sf::Keyboard::Scancode::A) {
                    camera.moveRight(-moveSpeed);
                } else if (key_pressed->scancode == sf::Keyboard::Scancode::D) {
                    camera.moveRight(moveSpeed);
                } else if (key_pressed->scancode == sf::Keyboard::Scancode::Q) {
                    camera.moveUp(-moveSpeed);
                } else if (key_pressed->scancode == sf::Keyboard::Scancode::E) {
                    camera.moveUp(moveSpeed);
                }
            }
        }
        
        // Handle mouse look
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
        if (firstMouse) {
            lastMousePosition = mousePos;
            firstMouse = false;
        } else {
            float deltaX = static_cast<float>(mousePos.x - lastMousePosition.x);
            float deltaY = static_cast<float>(mousePos.y - lastMousePosition.y);
            
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                camera.rotateWithMouse(deltaX, deltaY, 0.005f);
            }
            
            lastMousePosition = mousePos;
        }

        prerender();
        window->clear(sf::Color::White);
        render();
        postrender();
        window->display();
    }
}
void sim::stop() {}

void sim::pause() {}

void sim::resume() {}
