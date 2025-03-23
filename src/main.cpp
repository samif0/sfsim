#include <SFML/Graphics.hpp>
#include <sim.hpp>
#include <mesh.hpp>
#include <point3d.hpp>
#include <triangle.hpp>
#include <iostream>


int main()
{
    sim_config cfg {sf::String("sfsim"), 1920, 1080};
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

    point3d p0(sf::Vector3f{0, 0, 0});
    point3d p1(sf::Vector3f{0, 1, 0});
    point3d p2(sf::Vector3f{0, 0, 1});
    point3d p3(sf::Vector3f{0, 1, 1});
    point3d p4(sf::Vector3f{1, 0, 0});
    point3d p5(sf::Vector3f{1, 0, 1});
    point3d p6(sf::Vector3f{1, 1, 0});
    point3d p7(sf::Vector3f{1, 1, 1});

    std::vector<mesh> meshes {cube_mesh};
    std::vector<point3d> free_points {p0, p1, p2, p3, p4, p5, p6, p7};

    s.init(meshes, free_points);
    s.start();
}