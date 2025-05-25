#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

#include "camera.hpp"
#include "math/matrix.hpp"
#include "math/vector.hpp"

namespace SimpleExamples {

using namespace SFSim;
using namespace SFSim::Math;

struct Point3D {
    Vector3f pos;
    sf::Color color;
    float size;

    Point3D(Vector3f position, sf::Color c = sf::Color::White, float s = 3.0f)
        : pos(position), color(c), size(s) {}

    void draw(sf::RenderWindow& window, const Camera& camera) const {
        Matrix4x4 viewProjection = camera.getViewProjectionMatrix();
        Vector4f clipSpace = viewProjection * Vector4f(pos, 1.0f);

        if (clipSpace.w <= 0)
            return;

        Vector3f ndc = Vector3f(clipSpace.x, clipSpace.y, clipSpace.z) / clipSpace.w;

        if (ndc.x < -1.0f || ndc.x > 1.0f || ndc.y < -1.0f || ndc.y > 1.0f)
            return;

        float x = (ndc.x + 1.0f) * 0.5f * window.getSize().x;
        float y = (1.0f - ndc.y) * 0.5f * window.getSize().y;

        sf::CircleShape circle(size);
        circle.setFillColor(color);
        circle.setPosition(sf::Vector2f(x - size, y - size));
        window.draw(circle);
    }
};

class Simulation {
protected:
    sf::RenderWindow window;
    std::vector<Point3D> points;
    float time;
    Camera camera;
    bool mouseControlEnabled;
    sf::Vector2i lastMousePos;

public:
    Simulation(const std::string& title, int width = 1200, int height = 800)
        : window(sf::VideoMode(sf::Vector2u(width, height)), title), time(0.0f), mouseControlEnabled(false) {
        window.setFramerateLimit(60);

        // Initialize camera with proper aspect ratio
        float aspect = static_cast<float>(width) / static_cast<float>(height);
        camera.setPerspective(M_PI / 4.0f, aspect, 0.1f, 1000.0f);
        camera.setPosition(Vector3f(0, 0, 10));
        camera.setTarget(Vector3f(0, 0, 0));
        
        lastMousePos = sf::Mouse::getPosition(window);
    }

    Camera& getCamera() { return camera; }

    void addCamera(const Vector3f& position, const Vector3f& target = Vector3f(0, 0, 0),
                   const Vector3f& up = Vector3f::up()) {
        camera.lookAt(position, target, up);
    }

    void addPoint(const Vector3f& position, const sf::Color& color = sf::Color::White,
                  float size = 3.0f) {
        points.emplace_back(position, color, size);
    }

    void clearPoints() { points.clear(); }

    virtual void update(float deltaTime) = 0;
    virtual void setup() = 0;

    void run() {
        setup();
        sf::Clock clock;

        while (window.isOpen()) {
            float deltaTime = clock.restart().asSeconds();
            time += deltaTime;

            while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                } else if (const auto* key_pressed = event->getIf<sf::Event::KeyPressed>()) {
                    if (key_pressed->scancode == sf::Keyboard::Scancode::Escape) {
                        window.close();
                    }
                } else if (const auto* mouse_pressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouse_pressed->button == sf::Mouse::Button::Right) {
                        mouseControlEnabled = true;
                        lastMousePos = sf::Mouse::getPosition(window);
                    }
                } else if (const auto* mouse_released = event->getIf<sf::Event::MouseButtonReleased>()) {
                    if (mouse_released->button == sf::Mouse::Button::Right) {
                        mouseControlEnabled = false;
                    }
                }
            }

            // Handle mouse rotation
            if (mouseControlEnabled) {
                sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
                sf::Vector2i mouseDelta = currentMousePos - lastMousePos;
                
                camera.rotateWithMouse(static_cast<float>(mouseDelta.x), static_cast<float>(mouseDelta.y));
                lastMousePos = currentMousePos;
            }

            // Handle camera movement with WASD
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
                camera.moveForward(5.0f * deltaTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
                camera.moveForward(-5.0f * deltaTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
                camera.moveRight(-5.0f * deltaTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
                camera.moveRight(5.0f * deltaTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
                camera.moveUp(5.0f * deltaTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
                camera.moveUp(-5.0f * deltaTime);
            }

            update(deltaTime);

            window.clear(sf::Color::Black);
            for (const auto& point : points) {
                point.draw(window, camera);
            }
            window.display();
        }
    }
};

class DNAHelix : public Simulation {
public:
    DNAHelix() : Simulation("DNA Double Helix Simulation") {
        addCamera(Vector3f(5, 2, 15), Vector3f(0, 0, 0));
    }

    void setup() override {}

    void update(float deltaTime) override {
        clearPoints();

        const int segments = 80;
        const float height = 8.0f;
        const float radius = 2.0f;
        const float turns = 3.0f;

        for (int i = 0; i < segments; ++i) {
            float t = static_cast<float>(i) / segments;
            float angle1 = t * turns * 2.0f * M_PI + time * 0.5f;
            float angle2 = angle1 + M_PI;
            float y = t * height - height / 2.0f;

            // First strand (red)
            Vector3f pos1(radius * cos(angle1), y, radius * sin(angle1));
            addPoint(pos1, sf::Color::Red, 2.0f);

            // Second strand (blue)
            Vector3f pos2(radius * cos(angle2), y, radius * sin(angle2));
            addPoint(pos2, sf::Color::Blue, 2.0f);

            // Base pairs (connecting rungs)
            if (i % 4 == 0) {
                for (int j = 1; j <= 3; ++j) {
                    float lerp = static_cast<float>(j) / 4.0f;
                    Vector3f connection = pos1 + (pos2 - pos1) * lerp;
                    addPoint(connection, sf::Color::Yellow, 1.5f);
                }
            }
        }
    }
};

class ParticleSystem : public Simulation {
private:
    struct Particle {
        Vector3f position;
        Vector3f velocity;
        sf::Color color;

        Particle(Vector3f pos, Vector3f vel, sf::Color col)
            : position(pos), velocity(vel), color(col) {}
    };

    std::vector<Particle> particles;

public:
    ParticleSystem() : Simulation("Particle System Simulation") {
        addCamera(Vector3f(0, 0, 15), Vector3f(0, 0, 0));
    }

    void setup() override {
        particles.clear();

        // Create initial particles
        for (int i = 0; i < 150; ++i) {
            float theta = static_cast<float>(rand()) / RAND_MAX * 2.0f * M_PI;
            float phi = static_cast<float>(rand()) / RAND_MAX * M_PI;
            float speed = 2.0f + static_cast<float>(rand()) / RAND_MAX * 3.0f;

            Vector3f pos((static_cast<float>(rand()) / RAND_MAX - 0.5f) * 2.0f,
                         (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 2.0f,
                         (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 2.0f);

            Vector3f vel(speed * sin(phi) * cos(theta),
                         speed * sin(phi) * sin(theta) - 1.0f,  // Gravity effect
                         speed * cos(phi));

            sf::Color color(100 + rand() % 156, 100 + rand() % 156, 100 + rand() % 156);

            particles.emplace_back(pos, vel, color);
        }
    }

    void update(float deltaTime) override {
        clearPoints();

        const float bounce = 0.8f;
        const float gravity = 9.8f;
        const float boundary = 5.0f;

        for (auto& particle : particles) {
            // Apply gravity
            particle.velocity.y -= gravity * deltaTime;

            // Update position
            particle.position += particle.velocity * deltaTime;

            // Boundary collisions
            if (particle.position.x > boundary || particle.position.x < -boundary) {
                particle.velocity.x *= -bounce;
                particle.position.x = std::clamp(particle.position.x, -boundary, boundary);
            }
            if (particle.position.y > boundary || particle.position.y < -boundary) {
                particle.velocity.y *= -bounce;
                particle.position.y = std::clamp(particle.position.y, -boundary, boundary);
            }
            if (particle.position.z > boundary || particle.position.z < -boundary) {
                particle.velocity.z *= -bounce;
                particle.position.z = std::clamp(particle.position.z, -boundary, boundary);
            }

            addPoint(particle.position, particle.color, 2.5f);
        }
    }
};

class Mandala : public Simulation {
public:
    Mandala() : Simulation("Geometric Mandala Simulation") {
        addCamera(Vector3f(0, 5, 12), Vector3f(0, 0, 0));
    }

    void setup() override {}

    void update(float deltaTime) override {
        clearPoints();

        const int rings = 12;
        const int pointsPerRing = 16;
        const float maxRadius = 4.0f;

        for (int ring = 1; ring <= rings; ++ring) {
            float radius = (static_cast<float>(ring) / rings) * maxRadius;
            float heightOffset = sin(static_cast<float>(ring) * 0.3f + time) * 0.8f;

            for (int point = 0; point < pointsPerRing; ++point) {
                float angle = (static_cast<float>(point) / pointsPerRing) * 2.0f * M_PI;
                float spiralOffset = time * 0.5f + static_cast<float>(ring) * 0.2f;
                float radiusVariation = 1.0f + 0.3f * sin(time * 2.0f + angle * 3.0f);

                Vector3f pos(radius * radiusVariation * cos(angle + spiralOffset), heightOffset,
                             radius * radiusVariation * sin(angle + spiralOffset));

                // Color based on ring and rotation
                float hue = static_cast<float>(ring) / rings + time * 0.1f;
                sf::Color color(static_cast<std::uint8_t>(127 + 127 * sin(hue * 2.0f * M_PI)),
                                static_cast<std::uint8_t>(
                                    127 + 127 * sin(hue * 2.0f * M_PI + 2.0f * M_PI / 3.0f)),
                                static_cast<std::uint8_t>(
                                    127 + 127 * sin(hue * 2.0f * M_PI + 4.0f * M_PI / 3.0f)));

                addPoint(pos, color, 2.0f);
            }
        }
    }
};

void runExample(int choice) {
    switch (choice) {
        case 1: {
            DNAHelix dna;
            dna.run();
            break;
        }
        case 2: {
            ParticleSystem particles;
            particles.run();
            break;
        }
        case 3: {
            Mandala mandala;
            mandala.run();
            break;
        }
        default: {
            DNAHelix dna;
            dna.run();
            break;
        }
    }
}

}  // namespace SimpleExamples
