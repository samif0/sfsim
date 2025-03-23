#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include <SFML/Graphics.hpp>

class drawable {
private:
public:
    virtual ~drawable() = default;

    virtual void draw(sf::RenderWindow& window) const = 0;
    virtual void scale(sf::Vector2u window_dims) = 0;
};

#endif