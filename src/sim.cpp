#include "sim.hpp"
#include "mesh.hpp"
#include "triangle.hpp"
#include "point3d.hpp"
#include <vector>
#include <iostream>

void sim::proj(sf::Vector3f &in, sf::Vector3f &out, mat4x4 &pm){
    //TODO: use gpu if gpu is available
    out.x = in.x * pm.mat[0][0] + in.y * pm.mat[1][0] + in.z * pm.mat[2][0] + pm.mat[3][0];
    out.y = in.x * pm.mat[0][1] + in.y * pm.mat[1][1] + in.z * pm.mat[2][1] + pm.mat[3][1];
    out.z = in.x * pm.mat[0][2] + in.y * pm.mat[1][2] + in.z * pm.mat[2][2] + pm.mat[3][2];
    float w = in.x * pm.mat[0][3] + in.y * pm.mat[1][3] + in.z * pm.mat[2][3] + pm.mat[3][3];
    if(w != 0.0f){
        out.x /= w;
        out.y /= w;
        out.z /= w;
    }
}

void sim::prerender(){

}

void sim::render(){

    mat4x4 matRotZ, matRotX;

    fTheta += 1.0f * 0.01;
    
    matRotZ.mat[0][0] = cosf(fTheta);
    matRotZ.mat[0][1] = sinf(fTheta);
    matRotZ.mat[1][0] = -sinf(fTheta);
    matRotZ.mat[1][1] = cosf(fTheta);
    matRotZ.mat[2][2] = 1;
    matRotZ.mat[3][3] = 1;

    matRotX.mat[0][0] = 1;
    matRotX.mat[1][1] = cosf(fTheta * 0.5f);
    matRotX.mat[1][2] = sinf(fTheta * 0.5f);
    matRotX.mat[2][1] = -sinf(fTheta * 0.5f);
    matRotX.mat[2][2] = cosf(fTheta * 0.5f);
    matRotX.mat[3][3] = 1;


    for(auto& mesh : meshes) {
        for(auto& tri : mesh.get_mesh()){
            triangle out {};
            triangle tri_ztranslated {};
            triangle tri_rz {};
            triangle tri_rzx {};


            this->sim::proj(tri.vertices[0], tri_rz.vertices[0], matRotZ);
            this->sim::proj(tri.vertices[1], tri_rz.vertices[1], matRotZ);
            this->sim::proj(tri.vertices[2], tri_rz.vertices[2], matRotZ);

            this->sim::proj(tri_rz.vertices[0], tri_rzx.vertices[0], matRotX);
            this->sim::proj(tri_rz.vertices[1], tri_rzx.vertices[1], matRotX);
            this->sim::proj(tri_rz.vertices[2], tri_rzx.vertices[2], matRotX);


            tri_ztranslated = tri_rzx;

            tri_ztranslated.vertices[0].z = tri_rzx.vertices[0].z + 10.0f;
            tri_ztranslated.vertices[1].z = tri_rzx.vertices[1].z + 10.0f;
            tri_ztranslated.vertices[2].z = tri_rzx.vertices[2].z + 10.0f;

            this->sim::proj(tri_ztranslated.vertices[0], out.vertices[0], this->scfg->proj_mat);
            this->sim::proj(tri_ztranslated.vertices[1], out.vertices[1], this->scfg->proj_mat);
            this->sim::proj(tri_ztranslated.vertices[2], out.vertices[2], this->scfg->proj_mat);

            
            out.scale(window->getSize());
            out.draw(*window);
        }

        
        for(point3d point : mesh.get_vertices()) {
            point3d out = point;
            point3d p_ztranslated = point;
            point3d p_rz = point;
            point3d p_rzx = point;


            this->sim::proj(point.point_coords, p_rz.point_coords, matRotZ);

            this->sim::proj(p_rz.point_coords, p_rzx.point_coords, matRotX);


            p_ztranslated = p_rzx;

            p_ztranslated.point_coords.z = p_rzx.point_coords.z + 10.0f;

            this->sim::proj(p_ztranslated.point_coords, out.point_coords, this->scfg->proj_mat);
    
            out.scale(window->getSize());
            out.draw(*window);
        }
        

        for(line3d line : mesh.get_edges()) {
            point3d start1 = point3d();
            point3d end1 = point3d();
            line3d l_rz = line3d(&start1, &end1);
            
            point3d start2 = point3d();
            point3d end2 = point3d();
            line3d l_rzx = line3d(&start2, &end2);
            
            point3d start3 = point3d();
            point3d end3 = point3d();
            line3d l_ztranslated = line3d(&start3, &end3);
            
            point3d start4 = point3d();
            point3d end4 = point3d();
            line3d out = line3d(&start4, &end4);

            this->sim::proj(line.get_p1()->point_coords, l_rz.get_p1()->point_coords, matRotZ);
            this->sim::proj(line.get_p2()->point_coords, l_rz.get_p2()->point_coords, matRotZ);  
            this->sim::proj(l_rz.get_p1()->point_coords, l_rzx.get_p1()->point_coords, matRotX);
            this->sim::proj(l_rz.get_p2()->point_coords, l_rzx.get_p2()->point_coords, matRotX);
            
            l_ztranslated = l_rzx;
            l_ztranslated.get_p1()->point_coords.z = l_rzx.get_p1()->point_coords.z + 10.0f;
            l_ztranslated.get_p2()->point_coords.z = l_rzx.get_p2()->point_coords.z + 10.0f;

            this->sim::proj(l_ztranslated.get_p1()->point_coords, out.get_p1()->point_coords, this->scfg->proj_mat);
            this->sim::proj(l_ztranslated.get_p2()->point_coords, out.get_p2()->point_coords, this->scfg->proj_mat);
            
            out.scale(window->getSize());
            out.draw(*window);
        }

    }

    for(point3d point : free_points) {
        point3d out {};
        point3d p_ztranslated {};
        point3d p_rz {};
        point3d p_rzx {};


        this->sim::proj(point.point_coords, p_rz.point_coords, matRotZ);

        this->sim::proj(p_rz.point_coords, p_rzx.point_coords, matRotX);


        p_ztranslated = p_rzx;

        p_ztranslated.point_coords.z = p_rzx.point_coords.z + 3.0f;

        this->sim::proj(p_ztranslated.point_coords, out.point_coords, this->scfg->proj_mat);
 
        out.scale(window->getSize());
        out.draw(*window);
    }
}

void sim::postrender(){

}

void sim::start(){
    while (window->isOpen())
    {
        while (const std::optional event = window->pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window->close();
            } else if(const auto* key_pressed = event->getIf<sf::Event::KeyPressed>()) {
                if(key_pressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window->close();
                }
            } 
        }

        window->clear(sf::Color::White);
        render();
        window->display();
    }
}
void sim::stop(){
    
}

void sim::pause(){
    
}

void sim::resume(){
    
}