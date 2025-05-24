#pragma once

#include "math/vector.hpp"
#include "math/matrix.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

namespace SFSim {

using namespace Math;

enum class GeometryType {
    Point,
    Line,
    Triangle,
    Mesh
};

class Geometry {
public:
    Geometry(GeometryType type) : _type(type) {}
    virtual ~Geometry() = default;
    
    GeometryType getType() const { return _type; }
    
    virtual void draw(sf::RenderWindow& window, const Matrix4x4& transform, const Matrix4x4& viewProjection) = 0;
    virtual std::vector<Vector3f> getVertices() const = 0;
    virtual void setColor(const sf::Color& color) = 0;
    
protected:
    GeometryType _type;
    
    Vector2f projectPoint(const Vector3f& point, const Matrix4x4& mvp, int screenWidth, int screenHeight) const {
        Vector4f clipSpace = mvp * Vector4f(point, 1.0f);
        
        if (clipSpace.w == 0) return Vector2f(0, 0);
        
        Vector3f ndc = Vector3f(clipSpace.x, clipSpace.y, clipSpace.z) / clipSpace.w;
        
        float x = (ndc.x + 1.0f) * 0.5f * screenWidth;
        float y = (1.0f - ndc.y) * 0.5f * screenHeight;
        
        return Vector2f(x, y);
    }
    
    void drawLine(sf::RenderWindow& window, const Vector2f& start, const Vector2f& end, const sf::Color& color) const {
        sf::Vertex line[2];
        line[0] = sf::Vertex(sf::Vector2f(start.x, start.y), color);
        line[1] = sf::Vertex(sf::Vector2f(end.x, end.y), color);
        window.draw(line, 2, sf::Lines);
    }
    
    void drawPoint(sf::RenderWindow& window, const Vector2f& position, const sf::Color& color, float size = 3.0f) const {
        sf::CircleShape point(size);
        point.setFillColor(color);
        point.setPosition(position.x - size, position.y - size);
        window.draw(point);
    }
};

} // namespace SFSim