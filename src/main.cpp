#include <SFML/Graphics.hpp>
#include <sim.hpp>
#include <mesh.hpp>
#include <triangle.hpp>
#include <iostream>


int main()
{
    sim_config cfg {sf::String("sfsim"), 600, 800};
    sim s {&cfg};
    triangle st0(
        {sf::Vector3f(0.0f, 0.0f, 0.0f), sf::Vector3f(0.0f, 1.0f, 0.0f), sf::Vector3f(1.0f, 1.0f, 0.0f)}
    );
    triangle st1(
        {sf::Vector3f(0.0f, 0.0f, 0.0f), sf::Vector3f(1.0f, 1.0f, 0.0f), sf::Vector3f(1.0f, 0.0f, 0.0f)}
    );

    triangle et0(
        {sf::Vector3f(1.0f, 0.0f, 0.0f), sf::Vector3f(1.0f, 1.0f, 0.0f), sf::Vector3f(1.0f, 1.0f, 1.0f)}
    );
    triangle et1(
        {sf::Vector3f(1.0f, 0.0f, 0.0f), sf::Vector3f(1.0f, 1.0f, 1.0f), sf::Vector3f(1.0f, 0.0f, 1.0f)}
    );

    triangle nt0(
        {sf::Vector3f(1.0f, 0.0f, 1.0f), sf::Vector3f(1.0f, 1.0f, 1.0f), sf::Vector3f(0.0f, 1.0f, 1.0f)}
    );
    triangle nt1(
        {sf::Vector3f(1.0f, 0.0f, 1.0f), sf::Vector3f(0.0f, 1.0f, 1.0f), sf::Vector3f(0.0f, 0.0f, 1.0f)}
    );

    triangle wt0(
        {sf::Vector3f(0.0f, 0.0f, 1.0f), sf::Vector3f(0.0f, 1.0f, 1.0f), sf::Vector3f(0.0f, 1.0f, 0.0f)}
    );
    triangle wt1(
        {sf::Vector3f(0.0f, 0.0f, 1.0f), sf::Vector3f(0.0f, 1.0f, 0.0f), sf::Vector3f(0.0f, 0.0f, 0.0f)}
    );
    
    triangle tt0(
        {sf::Vector3f(0.0f, 1.0f, 0.0f), sf::Vector3f(0.0f, 1.0f, 1.0f), sf::Vector3f(1.0f, 1.0f, 1.0f)}
    );
    triangle tt1(
        {sf::Vector3f(0.0f, 1.0f, 0.0f), sf::Vector3f(1.0f, 1.0f, 1.0f), sf::Vector3f(1.0f, 1.0f, 0.0f)}
    );

    triangle bt0(
        {sf::Vector3f(1.0f, 0.0f, 1.0f), sf::Vector3f(0.0f, 0.0f, 1.0f), sf::Vector3f(0.0f, 0.0f, 0.0f)}
    );
    triangle bt1(
        {sf::Vector3f(1.0f, 0.0f, 1.0f), sf::Vector3f(0.0f, 0.0f, 0.0f), sf::Vector3f(1.0f, 0.0f, 0.0f)}
    );

    std::vector<triangle> cube {
        st0, st1, 
        et0, et1,
        nt0, nt1,
        wt0, wt1,
        tt0, tt1,
        bt0, bt1,
    };

    mesh cube_mesh(cube);
    std::vector<mesh> meshes {cube_mesh};

    s.init(meshes);
    s.start();
}