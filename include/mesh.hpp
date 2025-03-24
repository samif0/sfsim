#ifndef MESH_HPP
#define MESH_HPP

#include "triangle.hpp"
#include "line3d.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class mesh : public drawable {
private:
    std::vector<point3d> _vertices;
    std::vector<line3d> _edges;
    std::vector<triangle> _triangles;

    std::vector<triangle> _mesh;

public:

    void to_grid(int width, int height, int depth, float cell_size) {
        _vertices.clear();
        _edges.clear();

        std::vector<std::vector<std::vector<int>>> vertex_indices(width + 1,
            std::vector<std::vector<int>>(height + 1,
                std::vector<int>(depth + 1, -1)));

        int vertex_index = 0;
        for (int x = 0; x <= width; x++) {
            for (int y = 0; y <= height; y++) {
                for (int z = 0; z <= depth; z++) {
                    float px = (x - width/2.0f) * cell_size;
                    float py = (y - height/2.0f) * cell_size;
                    float pz = (z - depth/2.0f) * cell_size;
                    
                    _vertices.push_back(point3d(px, py, pz));
                    vertex_indices[x][y][z] = vertex_index++;
                }
            }
        }

        // X-axis edges
        for (int x = 0; x < width; x++) {
            for (int y = 0; y <= height; y++) {
                for (int z = 0; z <= depth; z++) {
                    int idx1 = vertex_indices[x][y][z];
                    int idx2 = vertex_indices[x+1][y][z];
                    _edges.push_back(line3d(&_vertices[idx1], &_vertices[idx2]));
                }
            }
        }
        
        // Y-axis edges
        for (int x = 0; x <= width; x++) {
            for (int y = 0; y < height; y++) {
                for (int z = 0; z <= depth; z++) {
                    int idx1 = vertex_indices[x][y][z];
                    int idx2 = vertex_indices[x][y+1][z];
                    _edges.push_back(line3d(&_vertices[idx1], &_vertices[idx2]));
                }
            }
        }
        
        // Z-axis edges
        for (int x = 0; x <= width; x++) {
            for (int y = 0; y <= height; y++) {
                for (int z = 0; z < depth; z++) {
                    int idx1 = vertex_indices[x][y][z];
                    int idx2 = vertex_indices[x][y][z+1];
                    _edges.push_back(line3d(&_vertices[idx1], &_vertices[idx2]));
                }
            }
        }
    }

    void draw(sf::RenderWindow& window) const override {
    }
    
    void scale(sf::Vector2u windowSize) override {
    }

    std::vector<triangle>& get_mesh() {
        return this->_mesh;
    }
    std::vector<point3d>& get_vertices() {
        return this->_vertices;
    }
    std::vector<line3d>& get_edges() {
        return this->_edges;
    }
    std::vector<triangle>& get_triangles() {
        return this->_triangles;
    }

    mesh() {};
    mesh(std::vector<triangle> &_m) : _mesh(_m) {}
    ~mesh(){}
};


#endif