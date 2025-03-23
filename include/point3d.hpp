#ifndef POINT_3D_HPP
#define POINT_3D_HPP

#include <SFML/Graphics.hpp> 
#include "drawable.hpp"

class point3d : public drawable {
private:
    void draw_pixel(sf::RenderWindow& window, sf::Color color = sf::Color::Red) const {
        sf::RectangleShape pixel(sf::Vector2f(5, 5));
        pixel.setPosition({point_coords.x - 2, point_coords.y - 2});
        pixel.setFillColor(color);
        window.draw(pixel);
    }

public:
    sf::Vector3f point_coords;

    void draw(sf::RenderWindow& window) const override {
        draw_pixel(window);
    }    

    void scale(sf::Vector2u window_dims) override { 
        point_coords.x += 1.0f; point_coords.y += 1.0f;
    
        float halfWidth = 0.5f * window_dims.x;
        float halfHeight = 0.5f * window_dims.y;
    
        if (halfWidth > 0 && halfHeight > 0) {
            point_coords.x *= halfWidth; point_coords.y *= halfHeight;
        }

    }    

    point3d() : point_coords({0, 0, 0}) {}
    point3d(float x, float y, float z) : point_coords({x, y, z}) {}
    point3d(sf::Vector3f _point_coords) : point_coords(_point_coords) {}
    ~point3d(){}
};

#endif 