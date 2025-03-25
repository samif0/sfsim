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


    mesh g {};
    mesh g2 {};
    mesh g3 {};
    mesh g4 {};
    mesh g5 {};
    mesh g6 {};

    g.to_grid(1, 1, 1, 0.1);
    g2.to_grid(3, 3, 3, 0.3);
    g3.to_grid(5, 5, 5, 0.5);
    g4.to_grid(7, 7, 7, 0.7);
    g5.to_grid(11, 11, 11, 1.1);
    g6.to_grid(13, 13, 13, 1.3);

    std::vector<mesh> meshes {g, g2, g3, g4, g5, g6};
    std::vector<point3d> free_points {p0, p1, p2, p3, p4, p5, p6, p7};

    s.init(meshes, free_points);
    s.start();
}