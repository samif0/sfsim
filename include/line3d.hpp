#ifndef LINE3D_HPP
#define LINE3D_HPP

#include "point3d.hpp"
#include <SFML/Graphics.hpp>

class line3d : public drawable {
private:
    // Use non-owning pointers - these are references to points owned elsewhere
    point3d* _p1;
    point3d* _p2;
    
public:
    line3d(point3d* p1, point3d* p2) : _p1(p1), _p2(p2) {}
    
    // Empty destructor - we don't own the points, so we don't delete them
    ~line3d() {}
    
    // Implement the required drawable methods
    void draw(sf::RenderWindow& window) const override {
        sf::Vertex v1 {};
        sf::Vertex v2 {};

        v1.position = sf::Vector2f(_p1->point_coords.x, _p1->point_coords.y);
        v1.color = sf::Color::Black;
        v2.position = sf::Vector2f(_p2->point_coords.x, _p2->point_coords.y);
        v2.color = sf::Color::Black; 
        sf::Vertex line[] = {
            v1, v2            
        };
       
        window.draw(line, 2, sf::PrimitiveType::Lines);
    }
    
    void scale(sf::Vector2u windowSize) override {
        // Properly scale both points
        _p1->scale(windowSize);
        _p2->scale(windowSize);
    }
    
    // Accessors
    point3d* get_p1() const { return _p1; }
    point3d* get_p2() const { return _p2; }
};

#endif