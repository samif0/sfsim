#include "geometry/mesh.hpp"
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>

namespace SFSim {

MeshGeometry::MeshGeometry()
    : Geometry(GeometryType::Mesh)
    , _material(std::make_shared<Material>())
{
}

MeshGeometry::~MeshGeometry() = default;

void MeshGeometry::setVertices(const std::vector<Vertex>& vertices) {
    _vertices = vertices;
}

void MeshGeometry::setIndices(const std::vector<unsigned int>& indices) {
    _indices = indices;
}

void MeshGeometry::setMaterial(std::shared_ptr<Material> material) {
    _material = material ? material : std::make_shared<Material>();
}

void MeshGeometry::addVertex(const Vertex& vertex) {
    _vertices.push_back(vertex);
}

void MeshGeometry::addTriangle(unsigned int a, unsigned int b, unsigned int c) {
    _indices.push_back(a);
    _indices.push_back(b);
    _indices.push_back(c);
}

void MeshGeometry::addQuad(unsigned int a, unsigned int b, unsigned int c, unsigned int d) {
    addTriangle(a, b, c);
    addTriangle(a, c, d);
}

void MeshGeometry::calculateNormals() {
    for (auto& vertex : _vertices) {
        vertex.normal = Vector3f::zero();
    }
    
    for (size_t i = 0; i < _indices.size(); i += 3) {
        if (i + 2 >= _indices.size()) break;
        
        unsigned int i0 = _indices[i];
        unsigned int i1 = _indices[i + 1];
        unsigned int i2 = _indices[i + 2];
        
        if (i0 >= _vertices.size() || i1 >= _vertices.size() || i2 >= _vertices.size()) continue;
        
        Vector3f v0 = _vertices[i0].position;
        Vector3f v1 = _vertices[i1].position;
        Vector3f v2 = _vertices[i2].position;
        
        Vector3f normal = (v1 - v0).cross(v2 - v0).normalized();
        
        _vertices[i0].normal += normal;
        _vertices[i1].normal += normal;
        _vertices[i2].normal += normal;
    }
    
    for (auto& vertex : _vertices) {
        vertex.normal.normalize();
    }
}

void MeshGeometry::calculateTangents() {
    std::vector<Vector3f> tangents(_vertices.size(), Vector3f::zero());
    std::vector<Vector3f> bitangents(_vertices.size(), Vector3f::zero());
    
    for (size_t i = 0; i < _indices.size(); i += 3) {
        if (i + 2 >= _indices.size()) break;
        
        unsigned int i0 = _indices[i];
        unsigned int i1 = _indices[i + 1];
        unsigned int i2 = _indices[i + 2];
        
        if (i0 >= _vertices.size() || i1 >= _vertices.size() || i2 >= _vertices.size()) continue;
        
        const Vertex& v0 = _vertices[i0];
        const Vertex& v1 = _vertices[i1];
        const Vertex& v2 = _vertices[i2];
        
        Vector3f deltaPos1 = v1.position - v0.position;
        Vector3f deltaPos2 = v2.position - v0.position;
        
        Vector2f deltaUV1 = v1.texCoords - v0.texCoords;
        Vector2f deltaUV2 = v2.texCoords - v0.texCoords;
        
        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        
        Vector3f tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * f;
        Vector3f bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * f;
        
        tangents[i0] += tangent;
        tangents[i1] += tangent;
        tangents[i2] += tangent;
        
        bitangents[i0] += bitangent;
        bitangents[i1] += bitangent;
        bitangents[i2] += bitangent;
    }
    
    for (size_t i = 0; i < _vertices.size(); ++i) {
        tangents[i].normalize();
        bitangents[i].normalize();
    }
}

void MeshGeometry::clear() {
    _vertices.clear();
    _indices.clear();
}

void MeshGeometry::draw(sf::RenderWindow& window, const Matrix4x4& transform, const Matrix4x4& viewProjection) {
    if (_material && _material->isWireframe()) {
        drawWireframe(window, transform, viewProjection);
    } else {
        drawFilled(window, transform, viewProjection);
    }
}

std::vector<Vector3f> MeshGeometry::getVertices() const {
    std::vector<Vector3f> positions;
    positions.reserve(_vertices.size());
    
    for (const auto& vertex : _vertices) {
        positions.push_back(vertex.position);
    }
    
    return positions;
}

void MeshGeometry::setColor(const sf::Color& color) {
    if (_material) {
        _material->setDiffuseColor(color);
    }
}

void MeshGeometry::drawWireframe(sf::RenderWindow& window, const Matrix4x4& transform, const Matrix4x4& viewProjection) {
    Matrix4x4 mvp = viewProjection * transform;
    sf::Color color = _material ? _material->getDiffuseColor() : sf::Color::White;
    
    for (size_t i = 0; i < _indices.size(); i += 3) {
        if (i + 2 >= _indices.size()) break;
        
        unsigned int i0 = _indices[i];
        unsigned int i1 = _indices[i + 1];
        unsigned int i2 = _indices[i + 2];
        
        if (i0 >= _vertices.size() || i1 >= _vertices.size() || i2 >= _vertices.size()) continue;
        
        Vector3f worldPos0 = transform.transformPoint(_vertices[i0].position);
        Vector3f worldPos1 = transform.transformPoint(_vertices[i1].position);
        Vector3f worldPos2 = transform.transformPoint(_vertices[i2].position);
        
        Vector2f screen0 = projectPoint(worldPos0, mvp, window.getSize().x, window.getSize().y);
        Vector2f screen1 = projectPoint(worldPos1, mvp, window.getSize().x, window.getSize().y);
        Vector2f screen2 = projectPoint(worldPos2, mvp, window.getSize().x, window.getSize().y);
        
        drawLine(window, screen0, screen1, color);
        drawLine(window, screen1, screen2, color);
        drawLine(window, screen2, screen0, color);
    }
}

void MeshGeometry::drawFilled(sf::RenderWindow& window, const Matrix4x4& transform, const Matrix4x4& viewProjection) {
    for (size_t i = 0; i < _indices.size(); i += 3) {
        if (i + 2 >= _indices.size()) break;
        
        unsigned int i0 = _indices[i];
        unsigned int i1 = _indices[i + 1];
        unsigned int i2 = _indices[i + 2];
        
        if (i0 >= _vertices.size() || i1 >= _vertices.size() || i2 >= _vertices.size()) continue;
        
        drawTriangle(window, _vertices[i0], _vertices[i1], _vertices[i2], transform, viewProjection);
    }
}

void MeshGeometry::drawTriangle(sf::RenderWindow& window, const Vertex& a, const Vertex& b, const Vertex& c,
                               const Matrix4x4& transform, const Matrix4x4& viewProjection) {
    Matrix4x4 mvp = viewProjection * transform;
    
    Vector3f worldPosA = transform.transformPoint(a.position);
    Vector3f worldPosB = transform.transformPoint(b.position);
    Vector3f worldPosC = transform.transformPoint(c.position);
    
    Vector2f screenA = projectPoint(worldPosA, mvp, window.getSize().x, window.getSize().y);
    Vector2f screenB = projectPoint(worldPosB, mvp, window.getSize().x, window.getSize().y);
    Vector2f screenC = projectPoint(worldPosC, mvp, window.getSize().x, window.getSize().y);
    
    sf::Color color = _material ? _material->getDiffuseColor() : sf::Color::White;
    
    if (_material) {
        Vector3f lightDir = Vector3f(0.5f, 0.5f, 1.0f).normalized();
        Vector3f worldNormal = transform.transformDirection(a.normal).normalized();
        color = _material->calculateColor(worldPosA, worldNormal, lightDir);
    }
    
    sf::Vertex triangle[3];
    triangle[0].position = sf::Vector2f(screenA.x, screenA.y);
    triangle[0].color = color;
    triangle[1].position = sf::Vector2f(screenB.x, screenB.y);
    triangle[1].color = color;
    triangle[2].position = sf::Vector2f(screenC.x, screenC.y);
    triangle[2].color = color;
    window.draw(triangle, 3, sf::PrimitiveType::Triangles);
}

std::unique_ptr<MeshGeometry> MeshGeometry::createCube(float size) {
    auto mesh = std::make_unique<MeshGeometry>();
    float half = size * 0.5f;
    
    std::vector<Vertex> vertices = {
        // Front face
        Vertex(Vector3f(-half, -half,  half), Vector3f(0, 0, 1), Vector2f(0, 0)),
        Vertex(Vector3f( half, -half,  half), Vector3f(0, 0, 1), Vector2f(1, 0)),
        Vertex(Vector3f( half,  half,  half), Vector3f(0, 0, 1), Vector2f(1, 1)),
        Vertex(Vector3f(-half,  half,  half), Vector3f(0, 0, 1), Vector2f(0, 1)),
        
        // Back face
        Vertex(Vector3f(-half, -half, -half), Vector3f(0, 0, -1), Vector2f(1, 0)),
        Vertex(Vector3f(-half,  half, -half), Vector3f(0, 0, -1), Vector2f(1, 1)),
        Vertex(Vector3f( half,  half, -half), Vector3f(0, 0, -1), Vector2f(0, 1)),
        Vertex(Vector3f( half, -half, -half), Vector3f(0, 0, -1), Vector2f(0, 0)),
        
        // Top face
        Vertex(Vector3f(-half,  half, -half), Vector3f(0, 1, 0), Vector2f(0, 1)),
        Vertex(Vector3f(-half,  half,  half), Vector3f(0, 1, 0), Vector2f(0, 0)),
        Vertex(Vector3f( half,  half,  half), Vector3f(0, 1, 0), Vector2f(1, 0)),
        Vertex(Vector3f( half,  half, -half), Vector3f(0, 1, 0), Vector2f(1, 1)),
        
        // Bottom face
        Vertex(Vector3f(-half, -half, -half), Vector3f(0, -1, 0), Vector2f(1, 1)),
        Vertex(Vector3f( half, -half, -half), Vector3f(0, -1, 0), Vector2f(0, 1)),
        Vertex(Vector3f( half, -half,  half), Vector3f(0, -1, 0), Vector2f(0, 0)),
        Vertex(Vector3f(-half, -half,  half), Vector3f(0, -1, 0), Vector2f(1, 0)),
        
        // Right face
        Vertex(Vector3f( half, -half, -half), Vector3f(1, 0, 0), Vector2f(1, 0)),
        Vertex(Vector3f( half,  half, -half), Vector3f(1, 0, 0), Vector2f(1, 1)),
        Vertex(Vector3f( half,  half,  half), Vector3f(1, 0, 0), Vector2f(0, 1)),
        Vertex(Vector3f( half, -half,  half), Vector3f(1, 0, 0), Vector2f(0, 0)),
        
        // Left face
        Vertex(Vector3f(-half, -half, -half), Vector3f(-1, 0, 0), Vector2f(0, 0)),
        Vertex(Vector3f(-half, -half,  half), Vector3f(-1, 0, 0), Vector2f(1, 0)),
        Vertex(Vector3f(-half,  half,  half), Vector3f(-1, 0, 0), Vector2f(1, 1)),
        Vertex(Vector3f(-half,  half, -half), Vector3f(-1, 0, 0), Vector2f(0, 1))
    };
    
    std::vector<unsigned int> indices = {
        0,  1,  2,   0,  2,  3,   // front
        4,  5,  6,   4,  6,  7,   // back
        8,  9,  10,  8,  10, 11,  // top
        12, 13, 14,  12, 14, 15,  // bottom
        16, 17, 18,  16, 18, 19,  // right
        20, 21, 22,  20, 22, 23   // left
    };
    
    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    
    return mesh;
}

std::unique_ptr<MeshGeometry> MeshGeometry::createSphere(float radius, int segments, int rings) {
    auto mesh = std::make_unique<MeshGeometry>();
    
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    for (int ring = 0; ring <= rings; ++ring) {
        float phi = M_PI * ring / rings;
        float y = radius * std::cos(phi);
        float ringRadius = radius * std::sin(phi);
        
        for (int segment = 0; segment <= segments; ++segment) {
            float theta = 2.0f * M_PI * segment / segments;
            float x = ringRadius * std::cos(theta);
            float z = ringRadius * std::sin(theta);
            
            Vector3f position(x, y, z);
            Vector3f normal = position.normalized();
            Vector2f texCoords(static_cast<float>(segment) / segments, static_cast<float>(ring) / rings);
            
            vertices.emplace_back(position, normal, texCoords);
        }
    }
    
    for (int ring = 0; ring < rings; ++ring) {
        for (int segment = 0; segment < segments; ++segment) {
            int current = ring * (segments + 1) + segment;
            int next = current + segments + 1;
            
            indices.push_back(current);
            indices.push_back(next);
            indices.push_back(current + 1);
            
            indices.push_back(current + 1);
            indices.push_back(next);
            indices.push_back(next + 1);
        }
    }
    
    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    
    return mesh;
}

std::unique_ptr<MeshGeometry> MeshGeometry::createPlane(float width, float height, int widthSegments, int heightSegments) {
    auto mesh = std::make_unique<MeshGeometry>();
    
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    float halfWidth = width * 0.5f;
    float halfHeight = height * 0.5f;
    
    for (int y = 0; y <= heightSegments; ++y) {
        for (int x = 0; x <= widthSegments; ++x) {
            float u = static_cast<float>(x) / widthSegments;
            float v = static_cast<float>(y) / heightSegments;
            
            Vector3f position(
                -halfWidth + u * width,
                0,
                -halfHeight + v * height
            );
            
            Vector3f normal(0, 1, 0);
            Vector2f texCoords(u, v);
            
            vertices.emplace_back(position, normal, texCoords);
        }
    }
    
    for (int y = 0; y < heightSegments; ++y) {
        for (int x = 0; x < widthSegments; ++x) {
            int a = y * (widthSegments + 1) + x;
            int b = a + 1;
            int c = a + widthSegments + 1;
            int d = c + 1;
            
            indices.push_back(a);
            indices.push_back(c);
            indices.push_back(b);
            
            indices.push_back(b);
            indices.push_back(c);
            indices.push_back(d);
        }
    }
    
    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    
    return mesh;
}

std::unique_ptr<MeshGeometry> MeshGeometry::createGrid(float size, int divisions) {
    auto mesh = std::make_unique<MeshGeometry>();
    
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    float half = size * 0.5f;
    float step = size / divisions;
    
    unsigned int indexCount = 0;
    
    for (int i = 0; i <= divisions; ++i) {
        float offset = -half + i * step;
        
        vertices.emplace_back(Vector3f(-half, 0, offset), Vector3f::up());
        vertices.emplace_back(Vector3f(half, 0, offset), Vector3f::up());
        indices.push_back(indexCount++);
        indices.push_back(indexCount++);
        
        vertices.emplace_back(Vector3f(offset, 0, -half), Vector3f::up());
        vertices.emplace_back(Vector3f(offset, 0, half), Vector3f::up());
        indices.push_back(indexCount++);
        indices.push_back(indexCount++);
    }
    
    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    
    return mesh;
}

std::unique_ptr<MeshGeometry> MeshGeometry::loadFromOBJ(const std::string& filename) {
    auto mesh = std::make_unique<MeshGeometry>();
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        return mesh;
    }
    
    std::vector<Vector3f> positions;
    std::vector<Vector3f> normals;
    std::vector<Vector2f> texCoords;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        
        if (prefix == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            positions.emplace_back(x, y, z);
        }
        else if (prefix == "vn") {
            float x, y, z;
            iss >> x >> y >> z;
            normals.emplace_back(x, y, z);
        }
        else if (prefix == "vt") {
            float u, v;
            iss >> u >> v;
            texCoords.emplace_back(u, v);
        }
        else if (prefix == "f") {
            std::string vertex1, vertex2, vertex3;
            iss >> vertex1 >> vertex2 >> vertex3;
            
            auto parseVertex = [&](const std::string& vertexStr) -> unsigned int {
                std::istringstream viss(vertexStr);
                std::string posIndex, texIndex, normIndex;
                
                std::getline(viss, posIndex, '/');
                std::getline(viss, texIndex, '/');
                std::getline(viss, normIndex);
                
                int pi = std::stoi(posIndex) - 1;
                int ti = texIndex.empty() ? -1 : std::stoi(texIndex) - 1;
                int ni = normIndex.empty() ? -1 : std::stoi(normIndex) - 1;
                
                Vector3f pos = (pi >= 0 && pi < positions.size()) ? positions[pi] : Vector3f::zero();
                Vector2f tex = (ti >= 0 && ti < texCoords.size()) ? texCoords[ti] : Vector2f(0, 0);
                Vector3f norm = (ni >= 0 && ni < normals.size()) ? normals[ni] : Vector3f::up();
                
                vertices.emplace_back(pos, norm, tex);
                return static_cast<unsigned int>(vertices.size() - 1);
            };
            
            indices.push_back(parseVertex(vertex1));
            indices.push_back(parseVertex(vertex2));
            indices.push_back(parseVertex(vertex3));
        }
    }
    
    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    
    if (normals.empty()) {
        mesh->calculateNormals();
    }
    
    return mesh;
}

bool MeshGeometry::saveToOBJ(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << "# OBJ file generated by SFSim\n";
    
    for (const auto& vertex : _vertices) {
        file << "v " << vertex.position.x << " " << vertex.position.y << " " << vertex.position.z << "\n";
    }
    
    for (const auto& vertex : _vertices) {
        file << "vn " << vertex.normal.x << " " << vertex.normal.y << " " << vertex.normal.z << "\n";
    }
    
    for (const auto& vertex : _vertices) {
        file << "vt " << vertex.texCoords.x << " " << vertex.texCoords.y << "\n";
    }
    
    for (size_t i = 0; i < _indices.size(); i += 3) {
        if (i + 2 < _indices.size()) {
            file << "f " << (_indices[i] + 1) << "/" << (_indices[i] + 1) << "/" << (_indices[i] + 1) << " "
                 << (_indices[i + 1] + 1) << "/" << (_indices[i + 1] + 1) << "/" << (_indices[i + 1] + 1) << " "
                 << (_indices[i + 2] + 1) << "/" << (_indices[i + 2] + 1) << "/" << (_indices[i + 2] + 1) << "\n";
        }
    }
    
    return true;
}

} // namespace SFSim