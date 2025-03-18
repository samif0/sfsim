#ifndef SIM_HPP
#define SIM_HPP

#include <SFML/Graphics.hpp>
#include "obj.hpp"


struct sim_config {
    sf::String title;
    unsigned int win_width;
    unsigned int win_height;
    sim_objects& objs;
};

class sim {
private:
    sim_config * scfg;
public:
    sim(sim_config * _scfg) : scfg(_scfg) {}
    ~sim(){}; 

    void start(void(*prerender)(sf::RenderWindow& window), void (*render)(sf::RenderWindow&window));
    void stop();
    void pause();
    void resume();
};

#endif