#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>

namespace SimpleExamples {

class Camera {
private:
    sf::Vector3f position;
    float yaw;
    float pitch;
    float distance;
    sf::Vector2i lastMousePos;
    bool mousePressed;
    float moveSpeed;
    float mouseSensitivity;
    float zoomSpeed;
    
public:
    Camera(sf::Vector3f startPos = sf::Vector3f(0, 0, 0), float startDistance = 10.0f) 
        : position(startPos), yaw(0), pitch(0), distance(startDistance),
          mousePressed(false), moveSpeed(5.0f), mouseSensitivity(0.01f), zoomSpeed(0.5f) {}
    
    void handleEvent(const sf::Event& event, sf::RenderWindow& window) {
        if (const auto* mouse_pressed = event.getIf<sf::Event::MouseButtonPressed>()) {
            if (mouse_pressed->button == sf::Mouse::Button::Left) {
                mousePressed = true;
                lastMousePos = sf::Mouse::getPosition(window);
            }
        } else if (const auto* mouse_released = event.getIf<sf::Event::MouseButtonReleased>()) {
            if (mouse_released->button == sf::Mouse::Button::Left) {
                mousePressed = false;
            }
        } else if (const auto* mouse_moved = event.getIf<sf::Event::MouseMoved>()) {
            if (mousePressed) {
                sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
                sf::Vector2i mouseDelta = currentMousePos - lastMousePos;
                
                yaw += mouseDelta.x * mouseSensitivity;
                pitch += mouseDelta.y * mouseSensitivity;
                
                // Clamp pitch to prevent camera flipping
                pitch = std::max(-static_cast<float>(M_PI)/2 + 0.1f, 
                               std::min(static_cast<float>(M_PI)/2 - 0.1f, pitch));
                
                lastMousePos = currentMousePos;
            }
        } else if (const auto* mouse_wheel = event.getIf<sf::Event::MouseWheelScrolled>()) {
            distance -= mouse_wheel->delta * zoomSpeed;
            distance = std::max(2.0f, std::min(50.0f, distance));
        }
    }
    
    void update(float deltaTime) {
        // WASD camera movement
        const float currentMoveSpeed = moveSpeed * deltaTime;
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            position.x += sin(yaw) * currentMoveSpeed;
            position.z += cos(yaw) * currentMoveSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            position.x -= sin(yaw) * currentMoveSpeed;
            position.z -= cos(yaw) * currentMoveSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            position.x += cos(yaw) * currentMoveSpeed;
            position.z -= sin(yaw) * currentMoveSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            position.x -= cos(yaw) * currentMoveSpeed;
            position.z += sin(yaw) * currentMoveSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
            position.y += currentMoveSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {
            position.y -= currentMoveSpeed;
        }
    }
    
    // Getters for rendering
    const sf::Vector3f& getPosition() const { return position; }
    float getYaw() const { return yaw; }
    float getPitch() const { return pitch; }
    float getDistance() const { return distance; }
    
    // Setters for customization
    void setPosition(const sf::Vector3f& pos) { position = pos; }
    void setMoveSpeed(float speed) { moveSpeed = speed; }
    void setMouseSensitivity(float sensitivity) { mouseSensitivity = sensitivity; }
    void setZoomSpeed(float speed) { zoomSpeed = speed; }
    void setDistance(float dist) { distance = std::max(2.0f, std::min(50.0f, dist)); }
};

class Point3D {
public:
    sf::Vector3f pos;
    sf::Color color;
    
    Point3D(sf::Vector3f position, sf::Color c = sf::Color::White) 
        : pos(position), color(c) {}
    
    sf::Vector2f project(sf::Vector2u windowSize, const Camera& camera) const {
        // Transform point relative to camera
        sf::Vector3f transformed = pos - camera.getPosition();
        
        // Apply camera rotation
        float cosYaw = cos(camera.getYaw());
        float sinYaw = sin(camera.getYaw());
        float cosPitch = cos(camera.getPitch());
        float sinPitch = sin(camera.getPitch());
        
        // Rotate around Y axis (yaw)
        float x_temp = transformed.x * cosYaw - transformed.z * sinYaw;
        float z_temp = transformed.x * sinYaw + transformed.z * cosYaw;
        transformed.x = x_temp;
        transformed.z = z_temp;
        
        // Rotate around X axis (pitch)
        float y_temp = transformed.y * cosPitch - transformed.z * sinPitch;
        z_temp = transformed.y * sinPitch + transformed.z * cosPitch;
        transformed.y = y_temp;
        transformed.z = z_temp;
        
        // Add camera distance
        transformed.z += camera.getDistance();
        
        float fov = 90.0f * M_PI / 180.0f;
        float aspect = static_cast<float>(windowSize.y) / static_cast<float>(windowSize.x);
        float fovRad = 1.0f / tanf(fov * 0.5f);
        
        if (transformed.z <= 0.1f) transformed.z = 0.1f; // Prevent division by zero
        
        // Perspective projection
        float x = transformed.x * fovRad * aspect / transformed.z;
        float y = transformed.y * fovRad / transformed.z;
        
        // Convert to screen coordinates
        float screenX = (x + 1.0f) * windowSize.x * 0.5f;
        float screenY = (1.0f - y) * windowSize.y * 0.5f;
        
        return sf::Vector2f(screenX, screenY);
    }
    
    void draw(sf::RenderWindow& window, const Camera& camera) const {
        sf::CircleShape circle(3);
        circle.setFillColor(color);
        sf::Vector2f screenPos = project(window.getSize(), camera);
        circle.setPosition(sf::Vector2f(screenPos.x - 1.5f, screenPos.y - 1.5f));
        window.draw(circle);
    }
};

class SimpleSimulation {
protected:
    sf::RenderWindow window;
    std::vector<Point3D> points;
    float time;
    Camera camera;
    
public:
    SimpleSimulation(const std::string& title, int width = 1200, int height = 800) 
        : window(sf::VideoMode(sf::Vector2u(width, height)), title), time(0.0f) {
        window.setFramerateLimit(60);
    }
    
    // Allow users to access and configure the camera
    Camera& getCamera() { return camera; }
    
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
                }
                
                // Let camera handle its own events
                camera.handleEvent(*event, window);
            }
            
            // Update camera
            camera.update(deltaTime);
            
            update(deltaTime);
            
            window.clear(sf::Color::Black);
            for (const auto& point : points) {
                point.draw(window, camera);
            }
            window.display();
        }
    }
};

class DNAHelix : public SimpleSimulation {
public:
    DNAHelix() : SimpleSimulation("DNA Double Helix Simulation") {
        // Customize camera for DNA viewing
        getCamera().setPosition(sf::Vector3f(5, 2, 0));
        getCamera().setDistance(15.0f);
        getCamera().setMoveSpeed(3.0f);
    }
    
    void setup() override {
        points.clear();
    }
    
    void update(float deltaTime) override {
        points.clear();
        
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
            sf::Vector3f pos1(
                radius * cos(angle1),
                y,
                radius * sin(angle1)
            );
            points.emplace_back(pos1, sf::Color::Red);
            
            // Second strand (blue)
            sf::Vector3f pos2(
                radius * cos(angle2),
                y,
                radius * sin(angle2)
            );
            points.emplace_back(pos2, sf::Color::Blue);
            
            // Base pairs (connecting rungs)
            if (i % 4 == 0) {
                for (int j = 1; j <= 3; ++j) {
                    float lerp = static_cast<float>(j) / 4.0f;
                    sf::Vector3f connection = pos1 + (pos2 - pos1) * lerp;
                    points.emplace_back(connection, sf::Color::Yellow);
                }
            }
        }
    }
};

class ParticleSystem : public SimpleSimulation {
private:
    struct Particle {
        sf::Vector3f position;
        sf::Vector3f velocity;
        sf::Color color;
        
        Particle(sf::Vector3f pos, sf::Vector3f vel, sf::Color col) 
            : position(pos), velocity(vel), color(col) {}
    };
    
    std::vector<Particle> particles;
    
public:
    ParticleSystem() : SimpleSimulation("Particle System Simulation") {
        // Fast camera for chasing particles
        getCamera().setMoveSpeed(8.0f);
        getCamera().setMouseSensitivity(0.015f);
        getCamera().setPosition(sf::Vector3f(0, 0, -5));
    }
    
    void setup() override {
        particles.clear();
        
        // Create initial particles
        for (int i = 0; i < 150; ++i) {
            float theta = static_cast<float>(rand()) / RAND_MAX * 2.0f * M_PI;
            float phi = static_cast<float>(rand()) / RAND_MAX * M_PI;
            float speed = 2.0f + static_cast<float>(rand()) / RAND_MAX * 3.0f;
            
            sf::Vector3f pos(
                (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 2.0f,
                (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 2.0f,
                (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 2.0f
            );
            
            sf::Vector3f vel(
                speed * sin(phi) * cos(theta),
                speed * sin(phi) * sin(theta) - 1.0f, // Gravity effect
                speed * cos(phi)
            );
            
            sf::Color color(
                100 + rand() % 156,
                100 + rand() % 156,
                100 + rand() % 156
            );
            
            particles.emplace_back(pos, vel, color);
        }
    }
    
    void update(float deltaTime) override {
        points.clear();
        
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
            
            points.emplace_back(particle.position, particle.color);
        }
    }
};

class Mandala : public SimpleSimulation {
public:
    Mandala() : SimpleSimulation("Geometric Mandala Simulation") {
        // Smooth slow camera for artistic viewing
        getCamera().setMoveSpeed(2.0f);
        getCamera().setMouseSensitivity(0.008f);
        getCamera().setZoomSpeed(0.3f);
        getCamera().setDistance(12.0f);
    }
    
    void setup() override {
        points.clear();
    }
    
    void update(float deltaTime) override {
        points.clear();
        
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
                
                sf::Vector3f pos(
                    radius * radiusVariation * cos(angle + spiralOffset),
                    heightOffset,
                    radius * radiusVariation * sin(angle + spiralOffset)
                );
                
                // Color based on ring and rotation
                float hue = static_cast<float>(ring) / rings + time * 0.1f;
                sf::Color color(
                    static_cast<std::uint8_t>(127 + 127 * sin(hue * 2.0f * M_PI)),
                    static_cast<std::uint8_t>(127 + 127 * sin(hue * 2.0f * M_PI + 2.0f * M_PI / 3.0f)),
                    static_cast<std::uint8_t>(127 + 127 * sin(hue * 2.0f * M_PI + 4.0f * M_PI / 3.0f))
                );
                
                points.emplace_back(pos, color);
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

} // namespace SimpleExamples