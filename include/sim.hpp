#ifndef SIM_HPP
#define SIM_HPP

#include <SFML/Graphics.hpp>
#include "mesh.hpp"
#include <iostream>


struct mat4x4 {
    float mat[4][4] {};
};

struct sim_config {
    sf::String title;
    unsigned int win_width;
    unsigned int win_height;
    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFovj = 90.0f;
    float fAspectRatio = float(win_height) / float(win_width);
    float fFovRad = 1.0f / tanf(fFovj * 0.5f / 180.0f * M_PI);
    float fElapsedTime = 0.1f;

    mat4x4 proj_mat = {
        {
            {fAspectRatio * fFovRad, 0, 0, 0},
            {0, fFovRad, 0, 0},
            {0, 0, fFar / (fFar - fNear), 1.0f},
            {0, 0, (-fFar * fNear) / (fFar - fNear), 0}
        }
    };
};


class sim {
private:
    sim_config * scfg;
    sf::RenderWindow * window;
    std::vector<mesh> meshes;

    void proj(sf::Vector3f &in, sf::Vector3f &out, mat4x4 &pm);
public:
    sim(sim_config * _scfg) : scfg(_scfg) {
        unsigned int window_width = this->scfg->win_width;
        unsigned int window_height = this->scfg->win_height;
        sf::String window_title = this->scfg->title;

        this->window = new sf::RenderWindow(sf::VideoMode({window_width, window_height}), window_title);
        this->window->setFramerateLimit(60);
    }
    ~sim() {
        if (window != nullptr) {
            delete window;
            window = nullptr;
        }
    }

    void init(std::vector<mesh> meshes){
        this->meshes = meshes;
    }
    void prerender();
    void render();
    void postrender();
    void start();
    void stop();
    void pause();
    void resume();
};

#endif