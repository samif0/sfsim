#ifndef MESH_HPP
#define MESH_HPP

#include "triangle.hpp"
#include "line3d.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

// TODO: Implement proper mesh class with vertex/index buffers and material properties
// HINT: Modern meshes typically use:
//       - Vertex buffer: Array of vertex data (position, normal, texcoord, etc.)
//       - Index buffer: Array of indices to define triangles/faces
//       - Material properties: Diffuse color, specular, roughness, etc.
//       - Consider creating a structure like this:
//         struct Vertex {
//             sf::Vector3f position;
//             sf::Vector3f normal;
//             sf::Vector2f texCoord;
//         };
// IMPL: Consider supporting standard mesh file formats (OBJ, FBX, glTF)
// HINT: Add mesh loading functions like:
//       - static Mesh LoadFromOBJ(const std::string& filename);
//       - static Mesh LoadFromFBX(const std::string& filename);
class mesh : public drawable {
private:
    // TODO: Use more efficient data structures for large meshes
    // HINT: For large meshes, consider:
    //       - Using contiguous arrays instead of vectors of objects
    //       - Spatial data structures like octrees or BVHs for culling
    //       - Custom memory allocators to reduce fragmentation
    std::vector<point3d> _vertices;
    std::vector<line3d> _edges;
    std::vector<triangle> _triangles;

    std::vector<triangle> _mesh; // TODO: Consolidate with _triangles

public:

    // TODO: Move grid creation to a dedicated factory class or namespace
    // IMPL: Support different grid types (uniform, non-uniform, adaptive)
    void to_grid(int width, int height, int depth, float cell_size, bool show_points) {
        _vertices.clear();
        _edges.clear();

        // TODO: Allow for configurable grid colors or color schemes
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
                    p.set_visible(show_points);
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

    // TODO: Implement proper mesh drawing method
    // HINT: A proper draw method would:
    //       1. Bind the mesh's vertex/index buffers to the GPU
    //       2. Set up material properties and textures
    //       3. Set transformation matrices (model, view, projection)
    //       4. Call the appropriate draw call (e.g., glDrawElements)
    // IMPL: Consider using GPU acceleration for mesh rendering
    // HINT: Even a simple OpenGL implementation with:
    //       - Vertex Buffer Objects (VBOs) for vertices
    //       - Vertex Array Objects (VAOs) for state
    //       - Index Buffer Objects (IBOs) for triangles
    //       - Basic shaders for lighting
    //       would be much faster than software rendering
    void draw(sf::RenderWindow& window) const override {
    }
    
    // TODO: Implement proper mesh scaling that preserves proportions
    // HINT: Instead of directly scaling vertices, consider:
    //       1. Keeping original vertices untouched
    //       2. Using a transformation matrix for scaling
    //       3. Applying the matrix during rendering
    //       4. Supporting non-uniform scaling (different x/y/z) when needed
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