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

        window.draw(shape);
    }

    void scale(sf::Vector2u window_dims){
        vertices[0].x += 1.0f; vertices[0].y += 1.0f;
        vertices[1].x += 1.0f; vertices[1].y += 1.0f;
        vertices[2].x += 1.0f; vertices[2].y += 1.0f;

        vertices[0].x *= 0.5f * window_dims.x; vertices[0].y *= 0.5 * window_dims.y;
        vertices[1].x *= 0.5f * window_dims.x; vertices[1].y *= 0.5 * window_dims.y;
        vertices[2].x *= 0.5f * window_dims.x; vertices[2].y *= 0.5 * window_dims.y;
    }

    triangle(): vertices({ {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}} ) {}
    triangle(std::vector<sf::Vector3f> _vertices) : vertices(_vertices) {};
    ~triangle(){};
};

#endif