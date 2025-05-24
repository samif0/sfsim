#include "renderer/renderer.hpp"
#include "geometry/line.hpp"
#include "geometry/point.hpp"
#include "geometry/triangle.hpp"
#include <algorithm>

namespace SFSim {

Renderer::Renderer()
    : _window(nullptr)
    , _camera(nullptr)
    , _wireframeMode(false)
    , _backfaceCulling(true)
    , _depthTesting(true)
{
    _stats.reset();
}

Renderer::~Renderer() {
    shutdown();
}

void Renderer::initialize(sf::RenderWindow* window) {
    _window = window;
    _stats.reset();
}

void Renderer::shutdown() {
    _renderQueue.clear();
    _debugGeometry.clear();
    _window = nullptr;
    _camera = nullptr;
}

void Renderer::beginFrame() {
    _frameClock.restart();
    _stats.reset();
    _renderQueue.clear();
    _debugGeometry.clear();
}

void Renderer::endFrame() {
    render();
    _stats.frameTime = _frameClock.getElapsedTime().asSeconds();
}

void Renderer::setCamera(Camera* camera) {
    _camera = camera;
}

void Renderer::submit(Geometry* geometry, const Matrix4x4& transform, int priority) {
    if (!geometry) return;
    
    if (_backfaceCulling && shouldCullBackface(geometry, transform)) {
        return;
    }
    
    RenderCommand command;
    command.geometry = geometry;
    command.transform = transform;
    command.priority = priority;
    command.depth = calculateDepth(geometry, transform);
    
    _renderQueue.push_back(command);
}

void Renderer::submitImmediate(Geometry* geometry, const Matrix4x4& transform) {
    if (!geometry || !_window || !_camera) return;
    
    Matrix4x4 viewProjection = getViewProjectionMatrix();
    geometry->draw(*_window, transform, viewProjection);
    
    _stats.drawCalls++;
    
    switch (geometry->getType()) {
        case GeometryType::Triangle:
            _stats.triangles++;
            break;
        case GeometryType::Line:
            _stats.lines++;
            break;
        case GeometryType::Point:
            _stats.points++;
            break;
        default:
            break;
    }
}

void Renderer::clear(const sf::Color& color) {
    if (_window) {
        _window->clear(color);
    }
}

void Renderer::render() {
    if (!_window || !_camera) return;
    
    sortRenderQueue();
    executeRenderQueue();
    renderDebugGeometry();
}

const Matrix4x4& Renderer::getViewMatrix() const {
    if (_camera) {
        return _camera->getViewMatrix();
    }
    static Matrix4x4 identity;
    return identity;
}

const Matrix4x4& Renderer::getProjectionMatrix() const {
    if (_camera) {
        return _camera->getProjectionMatrix();
    }
    static Matrix4x4 identity;
    return identity;
}

Matrix4x4 Renderer::getViewProjectionMatrix() const {
    if (_camera) {
        return _camera->getViewProjectionMatrix();
    }
    return Matrix4x4::identity();
}

void Renderer::drawDebugLine(const Vector3f& start, const Vector3f& end, const sf::Color& color) {
    auto line = std::make_unique<LineGeometry>(start, end, color);
    submitImmediate(line.get(), Matrix4x4::identity());
    _debugGeometry.push_back(std::move(line));
}

void Renderer::drawDebugPoint(const Vector3f& position, const sf::Color& color, float size) {
    auto point = std::make_unique<PointGeometry>(position, color);
    point->setSize(size);
    submitImmediate(point.get(), Matrix4x4::identity());
    _debugGeometry.push_back(std::move(point));
}

void Renderer::drawDebugWireCube(const Vector3f& center, const Vector3f& size, const sf::Color& color) {
    Vector3f halfSize = size * 0.5f;
    
    Vector3f vertices[8] = {
        center + Vector3f(-halfSize.x, -halfSize.y, -halfSize.z),
        center + Vector3f( halfSize.x, -halfSize.y, -halfSize.z),
        center + Vector3f( halfSize.x,  halfSize.y, -halfSize.z),
        center + Vector3f(-halfSize.x,  halfSize.y, -halfSize.z),
        center + Vector3f(-halfSize.x, -halfSize.y,  halfSize.z),
        center + Vector3f( halfSize.x, -halfSize.y,  halfSize.z),
        center + Vector3f( halfSize.x,  halfSize.y,  halfSize.z),
        center + Vector3f(-halfSize.x,  halfSize.y,  halfSize.z),
    };
    
    int edges[12][2] = {
        {0,1}, {1,2}, {2,3}, {3,0},
        {4,5}, {5,6}, {6,7}, {7,4},
        {0,4}, {1,5}, {2,6}, {3,7}
    };
    
    for (int i = 0; i < 12; ++i) {
        drawDebugLine(vertices[edges[i][0]], vertices[edges[i][1]], color);
    }
}

void Renderer::drawDebugGrid(const Vector3f& center, float spacing, int lines, const sf::Color& color) {
    float extent = spacing * lines * 0.5f;
    
    for (int i = -lines/2; i <= lines/2; ++i) {
        float offset = i * spacing;
        
        drawDebugLine(
            center + Vector3f(-extent, 0, offset),
            center + Vector3f(extent, 0, offset),
            color
        );
        
        drawDebugLine(
            center + Vector3f(offset, 0, -extent),
            center + Vector3f(offset, 0, extent),
            color
        );
    }
}

void Renderer::sortRenderQueue() {
    std::sort(_renderQueue.begin(), _renderQueue.end());
}

void Renderer::executeRenderQueue() {
    Matrix4x4 viewProjection = getViewProjectionMatrix();
    
    for (const auto& command : _renderQueue) {
        command.geometry->draw(*_window, command.transform, viewProjection);
        
        _stats.drawCalls++;
        
        switch (command.geometry->getType()) {
            case GeometryType::Triangle:
                _stats.triangles++;
                break;
            case GeometryType::Line:
                _stats.lines++;
                break;
            case GeometryType::Point:
                _stats.points++;
                break;
            default:
                break;
        }
    }
}

void Renderer::renderDebugGeometry() {
}

float Renderer::calculateDepth(Geometry* geometry, const Matrix4x4& transform) const {
    if (!_camera) return 0.0f;
    
    auto vertices = geometry->getVertices();
    if (vertices.empty()) return 0.0f;
    
    Vector3f center = Vector3f::zero();
    for (const auto& vertex : vertices) {
        center += transform.transformPoint(vertex);
    }
    center = center / static_cast<float>(vertices.size());
    
    Vector3f viewSpacePos = getViewMatrix().transformPoint(center);
    return viewSpacePos.z;
}

bool Renderer::shouldCullBackface(Geometry* geometry, const Matrix4x4& transform) const {
    if (!_camera || geometry->getType() != GeometryType::Triangle) {
        return false;
    }
    
    auto vertices = geometry->getVertices();
    if (vertices.size() != 3) return false;
    
    Vector3f worldA = transform.transformPoint(vertices[0]);
    Vector3f worldB = transform.transformPoint(vertices[1]);
    Vector3f worldC = transform.transformPoint(vertices[2]);
    
    Vector3f normal = (worldB - worldA).cross(worldC - worldA).normalized();
    Vector3f toCamera = (_camera->getPosition() - worldA).normalized();
    
    return normal.dot(toCamera) < 0;
}

} // namespace SFSim