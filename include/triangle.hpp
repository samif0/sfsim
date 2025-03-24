#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <SFML/Graphics.hpp>
#include "drawable.hpp"

class triangle : public drawable { 
private:
    void draw_pixel(sf::RenderWindow& window, float x, float y, sf::Color color = sf::Color::Black) const {
        sf::RectangleShape pixel(sf::Vector2f(1, 1));
        pixel.setPosition({x, y});
        pixel.setFillColor(color);
        window.draw(pixel);
    }

    // Bresenham line algorithm
    void draw_line(sf::RenderWindow& window, float x1, float y1, float x2, float y2, sf::Color color = sf::Color::Black) const {
        float x, y, dx, dy, dx1, dy1, px, py, xe, ye;
        [[maybe_unused]] int i;
        dx = x2 - x1; 
        dy = y2 - y1;
        dx1 = std::abs(dx); 
        dy1 = std::abs(dy);
        px = 2 * dy1 - dx1;
        py = 2 * dx1 - dy1;
        
        if (dy1 <= dx1) {
            if (dx >= 0) {
                x = x1; y = y1; xe = x2;
            } else {
                x = x2; y = y2; xe = x1;
            }
            
            draw_pixel(window, x, y, color);
            
            for (i = 0; x < xe; i++) {
                x = x + 1;
                if (px < 0) {
                    px = px + 2 * dy1;
                } else {
                    if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) 
                        y = y + 1; 
                    else 
                        y = y - 1;
                    
                    px = px + 2 * (dy1 - dx1);
                }
                draw_pixel(window, x, y, color);
            }
        } else {
            if (dy >= 0) {
                x = x1; y = y1; ye = y2;
            } else {
                x = x2; y = y2; ye = y1;
            }
            
            draw_pixel(window, x, y, color);
            
            for (i = 0; y < ye; i++) {
                y = y + 1;
                if (py <= 0) {
                    py = py + 2 * dx1;
                } else {
                    if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) 
                        x = x + 1; 
                    else 
                        x = x - 1;
                        
                    py = py + 2 * (dx1 - dy1);
                }
                draw_pixel(window, x, y, color);
            }
        }
    }

    void draw_triangle(sf::RenderWindow& window, float x1, float y1, float x2, float y2, float x3, float y3, 
                    sf::Color color = sf::Color::Black) const {
        draw_line(window, x1, y1, x2, y2, color);
        draw_line(window, x2, y2, x3, y3, color);
        draw_line(window, x3, y3, x1, y1, color);
    }
public:  
    std::vector<sf::Vector3f> vertices;

    void draw(sf::RenderWindow& window) const override {
        draw_triangle(window, vertices[0].x, vertices[0].y, vertices[1].x, vertices[1].y, vertices[2].x, vertices[2].y);
    }

    void scale(sf::Vector2u window_dims) override {
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