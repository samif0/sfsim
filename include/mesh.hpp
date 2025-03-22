#ifndef MESH_HPP
#define MESH_HPP

#include "triangle.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class mesh {
private:
public:
    std::vector<triangle> _mesh;
    mesh(std::vector<triangle> _m) : _mesh(_m) {}
    ~mesh(){}
};


#endif