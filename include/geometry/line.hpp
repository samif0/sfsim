#pragma once

#include "geometry.hpp"

namespace SFSim {

class LineGeometry : public Geometry {
public:
    LineGeometry();
    LineGeometry(const Vector3f& start, const Vector3f& end);
    LineGeometry(const Vector3f& start, const Vector3f& end, const sf::Color& color);
    
    void setStart(const Vector3f& start);
    void setEnd(const Vector3f& end);
    void setPoints(const Vector3f& start, const Vector3f& end);
    
    const Vector3f& getStart() const { return _start; }
    const Vector3f& getEnd() const { return _end; }
    
    void setColor(const sf::Color& color) override;
    const sf::Color& getColor() const { return _color; }
    
    Vector3f getDirection() const;
    float getLength() const;
    
    void draw(sf::RenderWindow& window, const Matrix4x4& transform, const Matrix4x4& viewProjection) override;
    std::vector<Vector3f> getVertices() const override;
    
private:
    Vector3f _start;
    Vector3f _end;
    sf::Color _color;
};

} // namespace SFSim