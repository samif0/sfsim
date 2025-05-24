#pragma once

#include "geometry.hpp"
#include "renderer/material.hpp"
#include <vector>
#include <memory>
#include <string>

namespace SFSim {

struct Vertex {
    Vector3f position;
    Vector3f normal;
    Vector2f texCoords;
    
    Vertex() : position(Vector3f::zero()), normal(Vector3f::up()), texCoords(Vector2f(0, 0)) {}
    Vertex(const Vector3f& pos) : position(pos), normal(Vector3f::up()), texCoords(Vector2f(0, 0)) {}
    Vertex(const Vector3f& pos, const Vector3f& norm) : position(pos), normal(norm), texCoords(Vector2f(0, 0)) {}
    Vertex(const Vector3f& pos, const Vector3f& norm, const Vector2f& tex) : position(pos), normal(norm), texCoords(tex) {}
};

class MeshGeometry : public Geometry {
public:
    MeshGeometry();
    ~MeshGeometry();
    
    void setVertices(const std::vector<Vertex>& vertices);
    void setIndices(const std::vector<unsigned int>& indices);
    void setMaterial(std::shared_ptr<Material> material);
    
    const std::vector<Vertex>& getVertices() const { return _vertices; }
    const std::vector<unsigned int>& getIndices() const { return _indices; }
    std::shared_ptr<Material> getMaterial() const { return _material; }
    
    void addVertex(const Vertex& vertex);
    void addTriangle(unsigned int a, unsigned int b, unsigned int c);
    void addQuad(unsigned int a, unsigned int b, unsigned int c, unsigned int d);
    
    void calculateNormals();
    void calculateTangents();
    
    void clear();
    
    void draw(sf::RenderWindow& window, const Matrix4x4& transform, const Matrix4x4& viewProjection) override;
    std::vector<Vector3f> getVertices() const override;
    void setColor(const sf::Color& color) override;
    
    static std::unique_ptr<MeshGeometry> createCube(float size = 1.0f);
    static std::unique_ptr<MeshGeometry> createSphere(float radius = 1.0f, int segments = 16, int rings = 16);
    static std::unique_ptr<MeshGeometry> createPlane(float width = 1.0f, float height = 1.0f, int widthSegments = 1, int heightSegments = 1);
    static std::unique_ptr<MeshGeometry> createGrid(float size = 10.0f, int divisions = 10);
    
    static std::unique_ptr<MeshGeometry> loadFromOBJ(const std::string& filename);
    bool saveToOBJ(const std::string& filename) const;
    
private:
    std::vector<Vertex> _vertices;
    std::vector<unsigned int> _indices;
    std::shared_ptr<Material> _material;
    
    void drawWireframe(sf::RenderWindow& window, const Matrix4x4& transform, const Matrix4x4& viewProjection);
    void drawFilled(sf::RenderWindow& window, const Matrix4x4& transform, const Matrix4x4& viewProjection);
    void drawTriangle(sf::RenderWindow& window, const Vertex& a, const Vertex& b, const Vertex& c, 
                     const Matrix4x4& transform, const Matrix4x4& viewProjection);
};

} // namespace SFSim