#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <mesh.hpp>
#include <point3d.hpp>
#include <sim.hpp>
#include <simobj/entity.hpp>
#include <triangle.hpp>

namespace Examples {

// Example 1: Rotating Double Helix DNA Structure
void create_dna_helix(sim& s) {
    std::vector<point3d> helix_points;
    std::vector<triangle*> triangles;
    
    const int segments = 50;
    const float height = 8.0f;
    const float radius = 2.0f;
    const float turns = 4.0f;
    
    // Create DNA double helix points
    for (int i = 0; i < segments; ++i) {
        float t = static_cast<float>(i) / segments;
        float angle1 = t * turns * 2.0f * M_PI;
        float angle2 = angle1 + M_PI; // Second strand offset by 180 degrees
        float y = t * height - height / 2.0f;
        
        // First strand
        sf::Vector3f pos1(
            radius * cos(angle1),
            y,
            radius * sin(angle1)
        );
        
        // Second strand
        sf::Vector3f pos2(
            radius * cos(angle2),
            y,
            radius * sin(angle2)
        );
        
        helix_points.emplace_back(pos1);
        helix_points.emplace_back(pos2);
        
        // Create connecting rungs between strands
        if (i % 5 == 0 && i < segments - 1) {
            sf::Vector3f next_pos1(
                radius * cos(angle1 + 0.1f),
                y + 0.1f,
                radius * sin(angle1 + 0.1f)
            );
            
            sf::Vector3f next_pos2(
                radius * cos(angle2 + 0.1f),
                y + 0.1f,
                radius * sin(angle2 + 0.1f)
            );
            
            // Create small triangles for the rungs
            triangle* rung = new triangle({pos1, pos2, next_pos1});
            triangles.push_back(rung);
        }
    }
    
    // Add helix points to simulation
    std::vector<mesh> meshes;
    s.init(meshes, helix_points);
    
    // Create entity from triangles
    if (!triangles.empty()) {
        entity dna_entity(triangles);
        s.add_entity(dna_entity);
    }
}

// Example 2: Bouncing Particle System
void create_particle_system(sim& s) {
    std::vector<point3d> particles;
    std::vector<triangle*> particle_triangles;
    
    const int num_particles = 100;
    const float spread = 5.0f;
    
    // Create random particles in a sphere
    for (int i = 0; i < num_particles; ++i) {
        float theta = static_cast<float>(rand()) / RAND_MAX * 2.0f * M_PI;
        float phi = static_cast<float>(rand()) / RAND_MAX * M_PI;
        float r = static_cast<float>(rand()) / RAND_MAX * spread;
        
        sf::Vector3f pos(
            r * sin(phi) * cos(theta),
            r * sin(phi) * sin(theta) + 2.0f, // Offset upward
            r * cos(phi)
        );
        
        particles.emplace_back(pos);
        
        // Create small triangular particles
        if (i % 10 == 0) {
            sf::Vector3f offset1(0.1f, 0.0f, 0.0f);
            sf::Vector3f offset2(0.0f, 0.1f, 0.0f);
            sf::Vector3f offset3(0.0f, 0.0f, 0.1f);
            
            triangle* particle_tri = new triangle({
                pos + offset1,
                pos + offset2,
                pos + offset3
            });
            particle_triangles.push_back(particle_tri);
        }
    }
    
    std::vector<mesh> meshes;
    s.init(meshes, particles);
    
    if (!particle_triangles.empty()) {
        entity particle_entity(particle_triangles);
        s.add_entity(particle_entity);
    }
}

// Example 3: Geometric Mandala
void create_mandala(sim& s) {
    std::vector<point3d> mandala_points;
    std::vector<triangle*> mandala_triangles;
    
    const int rings = 8;
    const int points_per_ring = 12;
    const float max_radius = 4.0f;
    
    // Create concentric rings of points
    for (int ring = 1; ring <= rings; ++ring) {
        float radius = (static_cast<float>(ring) / rings) * max_radius;
        float height_offset = sin(static_cast<float>(ring) * 0.5f) * 0.5f;
        
        for (int point = 0; point < points_per_ring; ++point) {
            float angle = (static_cast<float>(point) / points_per_ring) * 2.0f * M_PI;
            float spiral_offset = static_cast<float>(ring) * 0.1f;
            
            sf::Vector3f pos(
                radius * cos(angle + spiral_offset),
                height_offset,
                radius * sin(angle + spiral_offset)
            );
            
            mandala_points.emplace_back(pos);
            
            // Create triangular petals
            if (ring > 1 && point % 3 == 0) {
                float inner_radius = ((static_cast<float>(ring - 1)) / rings) * max_radius;
                sf::Vector3f inner_pos(
                    inner_radius * cos(angle + spiral_offset),
                    sin(static_cast<float>(ring - 1) * 0.5f) * 0.5f,
                    inner_radius * sin(angle + spiral_offset)
                );
                
                float next_angle = ((static_cast<float>(point + 1)) / points_per_ring) * 2.0f * M_PI;
                sf::Vector3f next_pos(
                    radius * cos(next_angle + spiral_offset),
                    height_offset,
                    radius * sin(next_angle + spiral_offset)
                );
                
                triangle* petal = new triangle({pos, inner_pos, next_pos});
                mandala_triangles.push_back(petal);
            }
        }
    }
    
    std::vector<mesh> meshes;
    s.init(meshes, mandala_points);
    
    if (!mandala_triangles.empty()) {
        entity mandala_entity(mandala_triangles);
        s.add_entity(mandala_entity);
    }
}

// Main example runner
void run_example(int example_number) {
    sim_config cfg{sf::String("SFSim Examples"), 1200, 800};
    sim s{&cfg};
    
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