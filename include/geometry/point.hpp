#pragma once

#include "geometry.hpp"

namespace SFSim {

class PointGeometry : public Geometry {
public:
    PointGeometry();
    PointGeometry(const Vector3f& position);
    PointGeometry(const Vector3f& position, const sf::Color& color);
    
    void setPosition(const Vector3f& position);
    const Vector3f& getPosition() const { return _position; }
    
    void setSize(float size);
    float getSize() const { return _size; }
    
    void setColor(const sf::Color& color) override;
    const sf::Color& getColor() const { return _color; }
    
    void draw(sf::RenderWindow& window, const Matrix4x4& transform, const Matrix4x4& viewProjection) override;
    std::vector<Vector3f> getVertices() const override;
    
private:
    Vector3f _position;
    sf::Color _color;
    float _size;
};

} // namespace SFSim