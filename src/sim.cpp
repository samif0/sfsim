#include "sim.hpp"



void sim::start(void (*func)()){
    unsigned int window_width = this->scfg->win_width;
    unsigned int window_height = this->scfg->win_height;
    sf::String window_title = this->scfg->title;

    sf::RenderWindow window(sf::VideoMode({window_width, window_height}), window_title);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::Black);

        func();

        window.display();
    }
}
void sim::stop(){
    
}

void sim::pause(){
    
}

void sim::resume(){
    
}