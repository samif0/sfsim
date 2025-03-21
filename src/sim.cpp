#include "sim.hpp"
#include "obj.hpp"


void sim::start(void (*prerender)(sf::RenderWindow& window), void (*render)(sf::RenderWindow& window)){
    unsigned int window_width = this->scfg->win_width;
    unsigned int window_height = this->scfg->win_height;
    sf::String window_title = this->scfg->title;

    sf::RenderWindow window(sf::VideoMode({window_width, window_height}), window_title);
    window.setFramerateLimit(60);

    prerender(window);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if(const auto* key_pressed = event->getIf<sf::Event::KeyPressed>()) {
                if(key_pressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window.close();
                }
            } 
        }

        window.clear(sf::Color::Black);

        render(window);

        window.display();
    }
}
void sim::stop(){
    
}

void sim::pause(){
    
}

void sim::resume(){
    
}