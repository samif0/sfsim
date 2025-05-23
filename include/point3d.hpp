#ifndef POINT_3D_HPP
#define POINT_3D_HPP

#include <SFML/Graphics.hpp> 
#include "drawable.hpp"

// TODO: Create a comprehensive vector math library
// HINT: A good vector math library would include:
//       - Vector2, Vector3, Vector4 classes with operations:
//         * Basic arithmetic (add, subtract, multiply, divide)
//         * Dot and cross products
//         * Normalization, length calculations
//         * Projections, reflections
//       - Matrix classes (3x3, 4x4) with operations:
//         * Matrix multiplication
//         * Inverse, transpose
//         * Creation of special matrices (rotation, scale, etc.)
// IMPL: Consider using existing math libraries like GLM
// HINT: Instead of building your own, GLM is a well-tested C++ math 
//       library designed for graphics programming that provides all
//       the vector/matrix operations you'll need
class point3d : public drawable {
private:
    sf::Color clr; 
    bool clr_set;
    bool visible;

    // TODO: Replace with hardware-accelerated point rendering
    // HINT: In modern OpenGL, points can be rendered as:
    //       1. GL_POINTS with adjustable point size
    //       2. Point sprites (textured quads that always face the camera)
    //       3. For more complex effects, can use geometry shaders to expand
    //          points into custom geometry
    // IMPL: Support different point rendering modes and sizes
    // HINT: Consider adding:
    //       - Point size parameter
    //       - Point shape option (square, circle, custom sprite)
    //       - Optional lighting effects or attenuation with distance
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
    // TODO: Use a custom vector class with proper math operations
    // HINT: Create a Vector3 class with operations like:
    //       - Vector3 operator+(const Vector3& other) const
    //       - Vector3 operator-(const Vector3& other) const
    //       - float dot(const Vector3& other) const
    //       - Vector3 cross(const Vector3& other) const
    //       - float length() const
    //       - Vector3 normalized() const
    sf::Vector3f point_coords;

    // TODO: Support different point visualization modes
    // HINT: Consider adding an enum for visualization mode:
    //       enum PointMode { Simple, Sphere, CustomShape };
    //       Then in draw(), check the mode and render accordingly
    void draw(sf::RenderWindow& window) const override {
        draw_pixel(window);
    }    

    // TODO: Improve scaling to handle proper transformations
    // HINT: Instead of manually scaling, use a full transformation pipeline:
    //       1. Model matrix (object's local transformations)
    //       2. View matrix (camera position and orientation)
    //       3. Projection matrix (perspective or orthographic)
    //       Then transform points using: screenPos = projection * view * model * worldPos
    // IMPL: Use a proper projection matrix instead of manual scaling
    // HINT: Create a method that takes projection and view matrices:
    //       sf::Vector2f project(const Matrix4x4& projection, const Matrix4x4& view)
    void scale(sf::Vector2u window_dims) override { 
        point_coords.x += 1.0f; point_coords.y += 1.0f;
    
        float halfWidth = 0.5f * window_dims.x;
        float halfHeight = 0.5f * window_dims.y;
    
        if (halfWidth > 0 && halfHeight > 0) {
            point_coords.x *= halfWidth; point_coords.y *= halfHeight;
        }

    }    

    void set_color(sf::Color clr){ this->clr = clr; clr_set = true; }
    void set_visible(bool vis){ this->visible = vis;}
    bool is_visible(){ return this->visible; }
    point3d() :  clr_set(false), visible(true), point_coords({0, 0, 0}) {}
    point3d(float x, float y, float z) : clr_set(false), visible(true), point_coords({x, y, z}) {}
    point3d(sf::Vector3f _point_coords) : clr_set(false), visible(true), point_coords(_point_coords) {}
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