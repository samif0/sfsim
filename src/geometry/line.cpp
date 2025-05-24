#include "geometry/line.hpp"

namespace SFSim {

LineGeometry::LineGeometry()
    : Geometry(GeometryType::Line)
    , _start(Vector3f::zero())
    , _end(Vector3f::unitX())
    , _color(sf::Color::White)
{
}

LineGeometry::LineGeometry(const Vector3f& start, const Vector3f& end)
    : Geometry(GeometryType::Line)
    , _start(start)
    , _end(end)
    , _color(sf::Color::White)
{
}

LineGeometry::LineGeometry(const Vector3f& start, const Vector3f& end, const sf::Color& color)
    : Geometry(GeometryType::Line)
    , _start(start)
    , _end(end)
    , _color(color)
{
}

void LineGeometry::setStart(const Vector3f& start) {
    _start = start;
}

void LineGeometry::setEnd(const Vector3f& end) {
    _end = end;
}

void LineGeometry::setPoints(const Vector3f& start, const Vector3f& end) {
    _start = start;
    _end = end;
}

void LineGeometry::setColor(const sf::Color& color) {
    _color = color;
}

Vector3f LineGeometry::getDirection() const {
    return (_end - _start).normalized();
}

float LineGeometry::getLength() const {
    return (_end - _start).length();
}

void LineGeometry::draw(sf::RenderWindow& window, const Matrix4x4& transform, const Matrix4x4& viewProjection) {
    Vector3f worldStart = transform.transformPoint(_start);
    Vector3f worldEnd = transform.transformPoint(_end);
    Matrix4x4 mvp = viewProjection * transform;
    
    Vector2f screenStart = projectPoint(worldStart, mvp, window.getSize().x, window.getSize().y);
    Vector2f screenEnd = projectPoint(worldEnd, mvp, window.getSize().x, window.getSize().y);
    
    drawLine(window, screenStart, screenEnd, _color);
}

std::vector<Vector3f> LineGeometry::getVertices() const {
    return {_start, _end};
}

} // namespace SFSim