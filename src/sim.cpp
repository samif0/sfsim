#include "sim.hpp"
#include "obj.hpp"
#define GL_SILENCE_DEPRECATION
#include <SFML/OpenGL.hpp>


void sim::start(void (*prerender)(sf::RenderWindow& window), void (*render)(sf::RenderWindow& window)){
    unsigned int window_width = this->scfg->win_width;
    unsigned int window_height = this->scfg->win_height;
    sf::String window_title = this->scfg->title;

    sf::RenderWindow window(sf::VideoMode({window_width, window_height}), window_title);
    window.setFramerateLimit(60);

    prerender(window);

   // run the main loop
   bool running = true;
   while (running)
   {
       // handle events
       while (const std::optional event = window.pollEvent())
       {
           if (event->is<sf::Event::Closed>())
           {
               // end the program
               running = false;
           }
           else if (const auto* resized = event->getIf<sf::Event::Resized>())
           {
               // adjust the viewport when the window is resized
               glViewport(0, 0, resized->size.x, resized->size.y);
           }
       }

       // clear the buffers
       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

       // draw...

       // end the current frame (internally swaps the front and back buffers)
       window.display();
   }
}
void sim::stop(){
    
}

void sim::pause(){
    
}

void sim::resume(){
    
}