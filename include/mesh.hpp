#ifndef MESH_HPP
#define MESH_HPP

#include "triangle.hpp"
#include "line3d.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

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

        sf::Color c;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, 255);
        c = sf::Color(dist(gen), dist(gen), dist(gen));


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

                    point3d p(px, py, pz);
                    p.set_color(c);
                    _vertices.push_back(p);

                    vertex_indices[x][y][z] = vertex_index++;
                }
            }
        }
        
        // X-axis edges (all grid lines on faces)
        for (int x = 0; x < width; x++) {
            // Front and back faces (z = 0 or z = depth)
            for (int y = 0; y <= height; y++) {
                for (int z = 0; z <= depth; z += depth) {
                    int idx1 = vertex_indices[x][y][z];
                    int idx2 = vertex_indices[x+1][y][z];
                    _edges.push_back(line3d(&_vertices[idx1], &_vertices[idx2]));
                }
            }
            
            // Top and bottom faces (y = 0 or y = height)
            for (int z = 0; z <= depth; z++) {
                for (int y = 0; y <= height; y += height) {
                    int idx1 = vertex_indices[x][y][z];
                    int idx2 = vertex_indices[x+1][y][z];
                    _edges.push_back(line3d(&_vertices[idx1], &_vertices[idx2]));
                }
            }
        }
        
        // Y-axis edges (all grid lines on faces)
        for (int y = 0; y < height; y++) {
            // Left and right faces (x = 0 or x = width)
            for (int z = 0; z <= depth; z++) {
                for (int x = 0; x <= width; x += width) {
                    int idx1 = vertex_indices[x][y][z];
                    int idx2 = vertex_indices[x][y+1][z];
                    _edges.push_back(line3d(&_vertices[idx1], &_vertices[idx2]));
                }
            }
            
            // Front and back faces (z = 0 or z = depth)
            for (int x = 0; x <= width; x++) {
                for (int z = 0; z <= depth; z += depth) {
                    int idx1 = vertex_indices[x][y][z];
                    int idx2 = vertex_indices[x][y+1][z];
                    _edges.push_back(line3d(&_vertices[idx1], &_vertices[idx2]));
                }
            }
        }
        
        // Z-axis edges (all grid lines on faces)
        for (int z = 0; z < depth; z++) {
            // Left and right faces (x = 0 or x = width)
            for (int y = 0; y <= height; y++) {
                for (int x = 0; x <= width; x += width) {
                    int idx1 = vertex_indices[x][y][z];
                    int idx2 = vertex_indices[x][y][z+1];
                    _edges.push_back(line3d(&_vertices[idx1], &_vertices[idx2]));
                }
            }
            
            for (int x = 0; x <= width; x++) {
                for (int y = 0; y <= height; y += height) {
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