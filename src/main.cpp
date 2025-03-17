#include <SFML/Graphics.hpp>
#include <sim.hpp>
#include <iostream>


void render() {
    std::cout << "from render" << std::endl;
}

int main()
{
    sim_config cfg {sf::String("sfsim"), 800, 600};
    sim s {&cfg};

    s.start(&render);
    s.stop();
}