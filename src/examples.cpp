#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <sim.hpp>
#include <ecs/entity.hpp>
#include <ecs/transform_component.hpp>
#include <ecs/render_component.hpp>
#include <geometry/mesh.hpp>
#include <geometry/point.hpp>
#include <math/vector.hpp>
#include <examples.hpp>

using namespace SFSim;
using namespace SFSim::Math;
using namespace SFSim::ECS;

namespace Examples {

// Example 1: Rotating Double Helix DNA Structure
void create_dna_helix(sim& s) {
    const int segments = 50;
    const float height = 8.0f;
    const float radius = 2.0f;
    const float turns = 4.0f;
    
    // Create DNA helix mesh
    auto helixMesh = std::make_unique<MeshGeometry>();
    
    // Create DNA double helix vertices
    for (int i = 0; i < segments; ++i) {
        float t = static_cast<float>(i) / segments;
        float angle1 = t * turns * 2.0f * M_PI;
        float angle2 = angle1 + M_PI; // Second strand offset by 180 degrees
        float y = t * height - height / 2.0f;
        
        // First strand
        Vector3f pos1(
            radius * cos(angle1),
            y,
            radius * sin(angle1)
        );
        
        // Second strand
        Vector3f pos2(
            radius * cos(angle2),
            y,
            radius * sin(angle2)
        );
        
        helixMesh->addVertex(Vertex(pos1));
        helixMesh->addVertex(Vertex(pos2));
        
        // Create connecting rungs between strands
        if (i % 5 == 0 && i < segments - 1) {
            float nextT = static_cast<float>(i + 1) / segments;
            float nextAngle1 = nextT * turns * 2.0f * M_PI;
            float nextAngle2 = nextAngle1 + M_PI;
            float nextY = nextT * height - height / 2.0f;
            
            Vector3f nextPos1(
                radius * cos(nextAngle1),
                nextY,
                radius * sin(nextAngle1)
            );
            
            Vector3f nextPos2(
                radius * cos(nextAngle2),
                nextY,
                radius * sin(nextAngle2)
            );
            
            // Create triangular rungs
            unsigned int base = helixMesh->getVertices().size();
            helixMesh->addVertex(Vertex(pos1));
            helixMesh->addVertex(Vertex(pos2));
            helixMesh->addVertex(Vertex(nextPos1));
            helixMesh->addTriangle(base, base + 1, base + 2);
        }
    }
    
    helixMesh->calculateNormals();
    helixMesh->setColor(sf::Color::Blue);
    
    // Create entity with transform and render components
    static EntityID nextId = 1;
    Entity helixEntity(nextId++);
    helixEntity.addComponent<TransformComponent>(Vector3f(0, 0, 0));
    helixEntity.addComponent<RenderComponent>(std::move(helixMesh));
    
    s.addEntity(std::move(helixEntity));
}

// Example 2: Bouncing Particle System
void create_particle_system(sim& s) {
    const int num_particles = 20; // Reduced for better performance
    const float spread = 5.0f;
    
    static EntityID nextId = 100;
    
    // Create random particles in a sphere
    for (int i = 0; i < num_particles; ++i) {
        float theta = static_cast<float>(rand()) / RAND_MAX * 2.0f * M_PI;
        float phi = static_cast<float>(rand()) / RAND_MAX * M_PI;
        float r = static_cast<float>(rand()) / RAND_MAX * spread;
        
        Vector3f pos(
            r * sin(phi) * cos(theta),
            r * sin(phi) * sin(theta) + 2.0f, // Offset upward
            r * cos(phi)
        );
        
        // Create small cube for each particle
        auto particleMesh = MeshGeometry::createCube(0.2f);
        particleMesh->setColor(sf::Color::Red);
        
        Entity particleEntity(nextId++);
        particleEntity.addComponent<TransformComponent>(pos);
        particleEntity.addComponent<RenderComponent>(std::move(particleMesh));
        
        s.addEntity(std::move(particleEntity));
    }
}

// Example 3: Geometric Mandala
void create_mandala(sim& s) {
    const int rings = 8;
    const int points_per_ring = 12;
    const float max_radius = 4.0f;
    
    auto mandalaMesh = std::make_unique<MeshGeometry>();
    
    // Create concentric rings of vertices
    for (int ring = 1; ring <= rings; ++ring) {
        float radius = (static_cast<float>(ring) / rings) * max_radius;
        float height_offset = sin(static_cast<float>(ring) * 0.5f) * 0.5f;
        
        for (int point = 0; point < points_per_ring; ++point) {
            float angle = (static_cast<float>(point) / points_per_ring) * 2.0f * M_PI;
            float spiral_offset = static_cast<float>(ring) * 0.1f;
            
            Vector3f pos(
                radius * cos(angle + spiral_offset),
                height_offset,
                radius * sin(angle + spiral_offset)
            );
            
            mandalaMesh->addVertex(Vertex(pos));
            
            // Create triangular petals connecting rings
            if (ring > 1 && point % 3 == 0) {
                float inner_radius = ((static_cast<float>(ring - 1)) / rings) * max_radius;
                Vector3f inner_pos(
                    inner_radius * cos(angle + spiral_offset),
                    sin(static_cast<float>(ring - 1) * 0.5f) * 0.5f,
                    inner_radius * sin(angle + spiral_offset)
                );
                
                float next_angle = ((static_cast<float>(point + 1)) / points_per_ring) * 2.0f * M_PI;
                Vector3f next_pos(
                    radius * cos(next_angle + spiral_offset),
                    height_offset,
                    radius * sin(next_angle + spiral_offset)
                );
                
                unsigned int base = mandalaMesh->getVertices().size();
                mandalaMesh->addVertex(Vertex(pos));
                mandalaMesh->addVertex(Vertex(inner_pos));
                mandalaMesh->addVertex(Vertex(next_pos));
                mandalaMesh->addTriangle(base, base + 1, base + 2);
            }
        }
    }
    
    mandalaMesh->calculateNormals();
    mandalaMesh->setColor(sf::Color::Green);
    
    static EntityID nextId = 200;
    Entity mandalaEntity(nextId++);
    mandalaEntity.addComponent<TransformComponent>(Vector3f(0, 0, 0));
    mandalaEntity.addComponent<RenderComponent>(std::move(mandalaMesh));
    
    s.addEntity(std::move(mandalaEntity));
}


} // namespace Examples

namespace Examples {

// Main example runner  
void run_example(int example_number) {
    
    SFSim::sim_config cfg{sf::String("SFSim Examples"), 1200, 800};
    SFSim::sim s{cfg};
    
    // Set up camera
    s.addCamera(Vector3f(0, 5, 15), Vector3f(0, 0, 0));
    
    switch (example_number) {
        case 1:
            std::cout << "Running DNA Helix Example..." << std::endl;
            create_dna_helix(s);
            break;
        case 2:
            std::cout << "Running Particle System Example..." << std::endl;
            create_particle_system(s);
            break;
        case 3:
            std::cout << "Running Mandala Example..." << std::endl;
            create_mandala(s);
            break;
        default:
            std::cout << "Running DNA Helix Example (default)..." << std::endl;
            create_dna_helix(s);
            break;
    }
    
    s.start();
}

} // namespace Examples