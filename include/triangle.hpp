#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <SFML/Graphics.hpp>
#include "drawable.hpp"

// TODO: Consider using a dedicated geometry class hierarchy
// HINT: Create a base Geometry class with derived classes like:
//       - Triangle
//       - Quad
//       - Sphere
//       - etc.
//       Each with their own draw and intersection methods
// IMPL: Support different primitive types with shared functionality
// HINT: Common functionality could include:
//       - Transformation methods (rotate, scale, translate)
//       - Bounding box/sphere calculations
//       - Intersection tests for collision detection
class triangle : public drawable { 
private:
    float depth; // TODO: Add proper Z-buffer handling for depth sorting
    // HINT: A proper Z-buffer would be a 2D array matching the screen resolution
    //       that stores the closest depth value for each pixel. Before drawing a pixel,
    //       check if its depth is less than the stored value, and only draw if it is.

    // TODO: Replace with hardware-accelerated pixel operations
    // HINT: Modern graphics APIs (OpenGL, Vulkan, DirectX) handle pixel drawing 
    //       on the GPU via fragment shaders, which is much faster than 
    //       manually setting pixels on the CPU
    void draw_pixel(sf::RenderWindow& window, float x, float y, sf::Color color = sf::Color::Black) const {
        sf::RectangleShape pixel(sf::Vector2f(1, 1));
        pixel.setPosition({x, y});
        pixel.setFillColor(color);
        window.draw(pixel);
    }

    // TODO: Move to a dedicated line rendering utility
    // Bresenham line algorithm
    // IMPL: Consider hardware-accelerated line rendering when available
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

    // TODO: Support filled triangles and proper shading
    // HINT: For filled triangles:
    //       1. Split the triangle into horizontal scanlines
    //       2. For each scanline, calculate start and end x-coordinates
    //       3. Draw a horizontal line between those points
    //       4. Interpolate color, depth, and texture coordinates along scanlines
    // IMPL: Implement proper triangle rasterization with interpolation
    // HINT: Barycentric coordinates are great for interpolation:
    //       - Calculate weights (w1,w2,w3) for any point inside triangle
    //       - Interpolate any attribute using: attr = w1*attr1 + w2*attr2 + w3*attr3
    //       - Works for colors, texture coordinates, normals, etc.
    void draw_triangle(sf::RenderWindow& window, float x1, float y1, float x2, float y2, float x3, float y3, 
                    sf::Color color = sf::Color::Black) const {
        draw_line(window, x1, y1, x2, y2, color);
        draw_line(window, x2, y2, x3, y3, color);
        draw_line(window, x3, y3, x1, y1, color);
    }

public:  
    // TODO: Add material properties, texture coordinates, and normals
    // HINT: Enhance vertices with additional attributes:
    //       struct Vertex {
    //           sf::Vector3f position;
    //           sf::Vector3f normal;     // For lighting calculations
    //           sf::Vector2f texCoord;   // For texture mapping
    //           sf::Color color;         // For vertex coloring
    //       };
    //       Then use an array of these instead of just position vectors
    std::vector<sf::Vector3f> vertices;

    // TODO: Support different rendering modes (wireframe, filled, textured)
    // HINT: Add an enum for render mode:
    //       enum RenderMode { Wireframe, Filled, Textured };
    //       Then check the current mode in the draw method and use
    //       the appropriate drawing technique (draw lines vs. fill triangle)
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

    float get_depth() const { return this->depth; }
    void set_depth(float n_depth) { this->depth = n_depth; }
    void set_depth() { 
        float sum_z = 0;
        for(auto& vert : this->vertices){
            sum_z += vert.z;
        }

        this->depth = sum_z / this->vertices.size(); 
    }
    triangle(): vertices({ {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}} ) {}
    triangle(std::vector<sf::Vector3f> _vertices) : vertices(_vertices) {};
    ~triangle(){};

    bool operator<(const triangle& other) const {
        return this->get_depth() < other.get_depth();
    }
};

#endif