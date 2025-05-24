#pragma once

#include "math/vector.hpp"
#include "math/matrix.hpp"
#include "geometry/geometry.hpp"
#include "camera.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

namespace SFSim {

using namespace Math;

struct RenderCommand {
    Geometry* geometry;
    Matrix4x4 transform;
    int priority;
    float depth;
    
    bool operator<(const RenderCommand& other) const {
        if (priority != other.priority) {
            return priority > other.priority;
        }
        return depth > other.depth;
    }
};

class Renderer {
public:
    Renderer();
    ~Renderer();
    
    void initialize(sf::RenderWindow* window);
    void shutdown();
    
    void beginFrame();
    void endFrame();
    
    void setCamera(Camera* camera);
    Camera* getCamera() const { return _camera; }
    
    void submit(Geometry* geometry, const Matrix4x4& transform, int priority = 0);
    void submitImmediate(Geometry* geometry, const Matrix4x4& transform);
    
    void clear(const sf::Color& color = sf::Color::Black);
    void render();
    
    void setWireframeMode(bool wireframe) { _wireframeMode = wireframe; }
    bool isWireframeMode() const { return _wireframeMode; }
    
    void setBackfaceCulling(bool enabled) { _backfaceCulling = enabled; }
    bool isBackfaceCullingEnabled() const { return _backfaceCulling; }
    
    void setDepthTesting(bool enabled) { _depthTesting = enabled; }
    bool isDepthTestingEnabled() const { return _depthTesting; }
    
    sf::RenderWindow* getWindow() const { return _window; }
    
    const Matrix4x4& getViewMatrix() const;
    const Matrix4x4& getProjectionMatrix() const;
    Matrix4x4 getViewProjectionMatrix() const;
    
    void drawDebugLine(const Vector3f& start, const Vector3f& end, const sf::Color& color = sf::Color::Red);
    void drawDebugPoint(const Vector3f& position, const sf::Color& color = sf::Color::Red, float size = 3.0f);
    void drawDebugWireCube(const Vector3f& center, const Vector3f& size, const sf::Color& color = sf::Color::Green);
    void drawDebugGrid(const Vector3f& center, float spacing, int lines, const sf::Color& color = sf::Color::White);
    
    struct Statistics {
        int drawCalls;
        int triangles;
        int lines;
        int points;
        float frameTime;
        
        void reset() {
            drawCalls = triangles = lines = points = 0;
            frameTime = 0.0f;
        }
    };
    
    const Statistics& getStatistics() const { return _stats; }
    
private:
    sf::RenderWindow* _window;
    Camera* _camera;
    
    std::vector<RenderCommand> _renderQueue;
    std::vector<std::unique_ptr<Geometry>> _debugGeometry;
    
    bool _wireframeMode;
    bool _backfaceCulling;
    bool _depthTesting;
    
    Statistics _stats;
    sf::Clock _frameClock;
    
    void sortRenderQueue();
    void executeRenderQueue();
    void renderDebugGeometry();
    
    float calculateDepth(Geometry* geometry, const Matrix4x4& transform) const;
    bool shouldCullBackface(Geometry* geometry, const Matrix4x4& transform) const;
};

} // namespace SFSim