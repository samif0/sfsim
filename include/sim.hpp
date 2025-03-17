#ifndef SIM_HPP
#define SIM_HPP

#include <SFML/Graphics.hpp>


struct sim_config {
    sf::String title;
    unsigned int win_width;
    unsigned int win_height;
};

class sim {
private:
    sim_config * scfg;
public:
    sim(sim_config * _scfg) : scfg(_scfg) {}
    ~sim(){
        delete(scfg);
    }; 

    void start(void (*func)());
    void stop();
    void pause();
    void resume();
};

#endif