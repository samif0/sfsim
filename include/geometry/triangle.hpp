#pragma once

#include "geometry.hpp"

namespace SFSim {

class TriangleGeometry : public Geometry {
public:
    TriangleGeometry();
    TriangleGeometry(const Vector3f& a, const Vector3f& b, const Vector3f& c);
    TriangleGeometry(const Vector3f& a, const Vector3f& b, const Vector3f& c, const sf::Color& color);
    
    void setVertex(int index, const Vector3f& vertex);
    void setVertices(const Vector3f& a, const Vector3f& b, const Vector3f& c);
    
    const Vector3f& getVertex(int index) const;
    Vector3f getA() const { return _vertices[0]; }
    Vector3f getB() const { return _vertices[1]; }
    Vector3f getC() const { return _vertices[2]; }
    
    void setColor(const sf::Color& color) override;
    const sf::Color& getColor() const { return _color; }
    
    void setWireframe(bool wireframe);
    bool isWireframe() const { return _wireframe; }
    
    Vector3f getNormal() const;
    Vector3f getCenter() const;
    float getArea() const;
    
    void draw(sf::RenderWindow& window, const Matrix4x4& transform, const Matrix4x4& viewProjection) override;
    std::vector<Vector3f> getVertices() const override;
    
private:
    Vector3f _vertices[3];
    sf::Color _color;
    bool _wireframe;
    
    void drawWireframe(sf::RenderWindow& window, const Matrix4x4& transform, const Matrix4x4& viewProjection);
    void drawFilled(sf::RenderWindow& window, const Matrix4x4& transform, const Matrix4x4& viewProjection);
    void drawTriangleOutline(sf::RenderWindow& window, const Vector2f& a, const Vector2f& b, const Vector2f& c);
    void rasterizeTriangle(sf::RenderWindow& window, const Vector2f& a, const Vector2f& b, const Vector2f& c);
};

} // namespace SFSim