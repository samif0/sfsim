#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <SFML/Graphics.hpp>

class triangle {
private:
public:  
    std::vector<sf::Vector3f> vertices;

    void draw(sf::RenderWindow& window){
        sf::ConvexShape shape(3);
        shape.setPoint(0, { vertices[0].x, vertices[0].y } );
        shape.setPoint(1, { vertices[1].x, vertices[1].y } );
        shape.setPoint(2, { vertices[2].x, vertices[2].y } );

        shape.setFillColor(sf::Color(50, 50, 50));  

        float area = std::abs((vertices[1].x - vertices[0].x) * (vertices[2].y - vertices[0].y) - 
                         (vertices[2].x - vertices[0].x) * (vertices[1].y - vertices[0].y)) * 0.5f;
    
        if (area > 10.0f) {
            shape.setOutlineColor(sf::Color::Black);
            shape.setOutlineThickness(1.0f);
        }
        window.draw(shape);
    }

    void scale(sf::Vector2u window_dims){
        // First normalize to [-1,1] range
        vertices[0].x += 1.0f; vertices[0].y += 1.0f;
        vertices[1].x += 1.0f; vertices[1].y += 1.0f;
        vertices[2].x += 1.0f; vertices[2].y += 1.0f;
    
        float halfWidth = 0.5f * window_dims.x;
        float halfHeight = 0.5f * window_dims.y;
    
        if (halfWidth > 0 && halfHeight > 0) {
            vertices[0].x *= halfWidth; vertices[0].y *= halfHeight;
            vertices[1].x *= halfWidth; vertices[1].y *= halfHeight;
            vertices[2].x *= halfWidth; vertices[2].y *= halfHeight;
        }
    }

    triangle(): vertices({ {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}} ) {}
    triangle(std::vector<sf::Vector3f> _vertices) : vertices(_vertices) {};
    ~triangle(){};
};

#endif