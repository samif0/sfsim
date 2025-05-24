#include "geometry/point.hpp"

namespace SFSim {

PointGeometry::PointGeometry()
    : Geometry(GeometryType::Point)
    , _position(Vector3f::zero())
    , _color(sf::Color::White)
    , _size(3.0f)
{
}

PointGeometry::PointGeometry(const Vector3f& position)
    : Geometry(GeometryType::Point)
    , _position(position)
    , _color(sf::Color::White)
    , _size(3.0f)
{
}

PointGeometry::PointGeometry(const Vector3f& position, const sf::Color& color)
    : Geometry(GeometryType::Point)
    , _position(position)
    , _color(color)
    , _size(3.0f)
{
}

void PointGeometry::setPosition(const Vector3f& position) {
    _position = position;
}

void PointGeometry::setSize(float size) {
    _size = size;
}

void PointGeometry::setColor(const sf::Color& color) {
    _color = color;
}

void PointGeometry::draw(sf::RenderWindow& window, const Matrix4x4& transform, const Matrix4x4& viewProjection) {
    Vector3f worldPos = transform.transformPoint(_position);
    Matrix4x4 mvp = viewProjection * transform;
    
    Vector2f screenPos = projectPoint(worldPos, mvp, window.getSize().x, window.getSize().y);
    
    if (screenPos.x >= 0 && screenPos.x < window.getSize().x &&
        screenPos.y >= 0 && screenPos.y < window.getSize().y) {
        drawPoint(window, screenPos, _color, _size);
    }
}

std::vector<Vector3f> PointGeometry::getVertices() const {
    return {_position};
}

} // namespace SFSim