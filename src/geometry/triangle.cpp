#include "geometry/triangle.hpp"
#include <algorithm>

namespace SFSim {

TriangleGeometry::TriangleGeometry()
    : Geometry(GeometryType::Triangle)
    , _color(sf::Color::White)
    , _wireframe(false)
{
    _vertices[0] = Vector3f(0, 1, 0);
    _vertices[1] = Vector3f(-1, -1, 0);
    _vertices[2] = Vector3f(1, -1, 0);
}

TriangleGeometry::TriangleGeometry(const Vector3f& a, const Vector3f& b, const Vector3f& c)
    : Geometry(GeometryType::Triangle)
    , _color(sf::Color::White)
    , _wireframe(false)
{
    _vertices[0] = a;
    _vertices[1] = b;
    _vertices[2] = c;
}

TriangleGeometry::TriangleGeometry(const Vector3f& a, const Vector3f& b, const Vector3f& c, const sf::Color& color)
    : Geometry(GeometryType::Triangle)
    , _color(color)
    , _wireframe(false)
{
    _vertices[0] = a;
    _vertices[1] = b;
    _vertices[2] = c;
}

void TriangleGeometry::setVertex(int index, const Vector3f& vertex) {
    if (index >= 0 && index < 3) {
        _vertices[index] = vertex;
    }
}

void TriangleGeometry::setVertices(const Vector3f& a, const Vector3f& b, const Vector3f& c) {
    _vertices[0] = a;
    _vertices[1] = b;
    _vertices[2] = c;
}

const Vector3f& TriangleGeometry::getVertex(int index) const {
    static Vector3f zero = Vector3f::zero();
    if (index >= 0 && index < 3) {
        return _vertices[index];
    }
    return zero;
}

void TriangleGeometry::setColor(const sf::Color& color) {
    _color = color;
}

void TriangleGeometry::setWireframe(bool wireframe) {
    _wireframe = wireframe;
}

Vector3f TriangleGeometry::getNormal() const {
    Vector3f edge1 = _vertices[1] - _vertices[0];
    Vector3f edge2 = _vertices[2] - _vertices[0];
    return edge1.cross(edge2).normalized();
}

Vector3f TriangleGeometry::getCenter() const {
    return (_vertices[0] + _vertices[1] + _vertices[2]) / 3.0f;
}

float TriangleGeometry::getArea() const {
    Vector3f edge1 = _vertices[1] - _vertices[0];
    Vector3f edge2 = _vertices[2] - _vertices[0];
    return edge1.cross(edge2).length() * 0.5f;
}

void TriangleGeometry::draw(sf::RenderWindow& window, const Matrix4x4& transform, const Matrix4x4& viewProjection) {
    if (_wireframe) {
        drawWireframe(window, transform, viewProjection);
    } else {
        drawFilled(window, transform, viewProjection);
    }
}

std::vector<Vector3f> TriangleGeometry::getVertices() const {
    return {_vertices[0], _vertices[1], _vertices[2]};
}

void TriangleGeometry::drawWireframe(sf::RenderWindow& window, const Matrix4x4& transform, const Matrix4x4& viewProjection) {
    Matrix4x4 mvp = viewProjection * transform;
    
    Vector2f screenA = projectPoint(transform.transformPoint(_vertices[0]), mvp, window.getSize().x, window.getSize().y);
    Vector2f screenB = projectPoint(transform.transformPoint(_vertices[1]), mvp, window.getSize().x, window.getSize().y);
    Vector2f screenC = projectPoint(transform.transformPoint(_vertices[2]), mvp, window.getSize().x, window.getSize().y);
    
    drawTriangleOutline(window, screenA, screenB, screenC);
}

void TriangleGeometry::drawFilled(sf::RenderWindow& window, const Matrix4x4& transform, const Matrix4x4& viewProjection) {
    Matrix4x4 mvp = viewProjection * transform;
    
    Vector2f screenA = projectPoint(transform.transformPoint(_vertices[0]), mvp, window.getSize().x, window.getSize().y);
    Vector2f screenB = projectPoint(transform.transformPoint(_vertices[1]), mvp, window.getSize().x, window.getSize().y);
    Vector2f screenC = projectPoint(transform.transformPoint(_vertices[2]), mvp, window.getSize().x, window.getSize().y);
    
    rasterizeTriangle(window, screenA, screenB, screenC);
}

void TriangleGeometry::drawTriangleOutline(sf::RenderWindow& window, const Vector2f& a, const Vector2f& b, const Vector2f& c) {
    drawLine(window, a, b, _color);
    drawLine(window, b, c, _color);
    drawLine(window, c, a, _color);
}

void TriangleGeometry::rasterizeTriangle(sf::RenderWindow& window, const Vector2f& a, const Vector2f& b, const Vector2f& c) {
    sf::Vertex triangle[3];
    triangle[0] = sf::Vertex(sf::Vector2f(a.x, a.y), _color);
    triangle[1] = sf::Vertex(sf::Vector2f(b.x, b.y), _color);
    triangle[2] = sf::Vertex(sf::Vector2f(c.x, c.y), _color);
    window.draw(triangle, 3, sf::Triangles);
}

} // namespace SFSim