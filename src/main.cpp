#include <SFML/Graphics.hpp>
#include <sim.hpp>
#include <obj.hpp>
#include <iostream>

void prerender(sf::RenderWindow& window) {
       
}
void render(sf::RenderWindow& window) {    
}

int main()
{
    sim_objects * objs = new sim_objects;

    sf::VertexArray triangle(sf::PrimitiveType::Triangles, 3);
    triangle[0].position = sf::Vector2f(10.f, 10.f);
    triangle[1].position = sf::Vector2f(100.f, 10.f);
    triangle[2].position = sf::Vector2f(100.f, 100.f);
    triangle[0].color = sf::Color::Red;
    triangle[1].color = sf::Color::Blue;
    triangle[2].color = sf::Color::Green;

    obj o(0, triangle);
    objs->objects.insert({0, o});


    sim_config cfg {sf::String("sfsim"), 800, 600, *objs};
    sim s {&cfg};

    s.start(prerender, render);
    s.stop();

    delete(objs);
}