#ifndef POINT_3D_HPP
#define POINT_3D_HPP

#include <SFML/Graphics.hpp> 
#include "drawable.hpp"

class point3d : public drawable {
private:
    sf::Color clr; 
    bool clr_set;

    void draw_pixel(sf::RenderWindow& window, sf::Color color = sf::Color::Red) const {
        sf::RectangleShape pixel(sf::Vector2f(2, 2));
        pixel.setPosition({point_coords.x - 1, point_coords.y - 1});
        if(clr_set){
            pixel.setFillColor(clr);
        } else {
            pixel.setFillColor(color);
        }
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

    void set_color(sf::Color clr){ this->clr = clr; clr_set = true; }
    point3d() :  clr_set(false), point_coords({0, 0, 0}) {}
    point3d(float x, float y, float z) : clr_set(false), point_coords({x, y, z}) {}
    point3d(sf::Vector3f _point_coords) : clr_set(false), point_coords(_point_coords) {}
    ~point3d(){}
    
    point3d& operator=(const point3d& other) {
        if (this != &other) {
            point_coords = other.point_coords;
            clr = other.clr;
            clr_set = other.clr_set;
        }
        return *this;
    }
};

#endif 