#include "sim.hpp"

#include "mesh.hpp"
#include "point3d.hpp"
#include "simobj/entity.hpp"
#include "triangle.hpp"

void sim::proj(sf::Vector3f& in, sf::Vector3f& out, mat4x4& pm) {
    // TODO: use gpu if gpu is available
    // IMPL: Move to a dedicated math utility class
    // HINT: Create a Vector4 class to handle homogeneous coordinates properly
    //       and a Matrix4x4 class with a method like transform(Vector4) that handles
    //       the matrix-vector multiplication and perspective division
    // TODO: Optimize matrix calculations with SIMD instructions when available
    // HINT: SIMD libraries like SSE/AVX can speed up calculations significantly
    //       Look into std::experimental::simd or libraries like Eigen
    out.x = in.x * pm.mat[0][0] + in.y * pm.mat[1][0] + in.z * pm.mat[2][0] + pm.mat[3][0];
    out.y = in.x * pm.mat[0][1] + in.y * pm.mat[1][1] + in.z * pm.mat[2][1] + pm.mat[3][1];
    out.z = in.x * pm.mat[0][2] + in.y * pm.mat[1][2] + in.z * pm.mat[2][2] + pm.mat[3][2];
    float w = in.x * pm.mat[0][3] + in.y * pm.mat[1][3] + in.z * pm.mat[2][3] + pm.mat[3][3];
    if (w != 0.0f) {
        out.x /= w;
        out.y /= w;
        out.z /= w;
    }
}

void sim::prerender() {}

void sim::render() {
    // TODO: Replace hard-coded rotation with a proper camera and transform system
    // HINT: Create a Transform class with:
    //       - Position, rotation, and scale properties
    //       - Methods to rotate, translate, scale
    //       - Method to get the combined transformation matrix
    //       - Parent-child relationship for hierarchical transforms
    // IMPL: Create dedicated transformation classes for rotation, scaling, etc.
    mat4x4 matRotZ, matRotX;

    // TODO: Use proper time delta for frame-rate independent rotation
    // HINT: Calculate deltaTime = currentTime - lastFrameTime 
    //       and use that instead of hardcoded 0.01
    //       This makes animations consistent regardless of framerate
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

    // TODO: Implement a proper scene graph traversal system
    // HINT: Create a SceneGraph class with nodes that have parent-child relationships
    //       Then traverse the graph depth-first or breadth-first to render objects
    //       in the correct order, with proper transformations applied
    // IMPL: Use a dedicated render pipeline with configurable stages
    // HINT: RenderPipeline could have stages like: 
    //       - Shadow map generation
    //       - Depth pre-pass
    //       - Main geometry render
    //       - Post-processing effects
    for (auto& mesh : meshes) {
        // TODO: Replace priority queue with a proper visibility determination system
        // HINT: Consider techniques like:
        //       - Z-buffer/depth buffer for hardware-accelerated depth sorting
        //       - Occlusion culling to avoid drawing hidden objects
        //       - Frustum culling to avoid drawing objects outside the camera view
        std::priority_queue<triangle> triangles_rasterized;

        for (auto& tri : mesh.get_mesh()) {
            // TODO: Consolidate transformation steps into a unified matrix multiplication pipeline
            triangle out{};
            triangle tri_ztranslated{};
            triangle tri_rz{};
            triangle tri_rzx{};

            this->sim::proj(tri.vertices[0], tri_rz.vertices[0], matRotZ);
            this->sim::proj(tri.vertices[1], tri_rz.vertices[1], matRotZ);
            this->sim::proj(tri.vertices[2], tri_rz.vertices[2], matRotZ);

            this->sim::proj(tri_rz.vertices[0], tri_rzx.vertices[0], matRotX);
            this->sim::proj(tri_rz.vertices[1], tri_rzx.vertices[1], matRotX);
            this->sim::proj(tri_rz.vertices[2], tri_rzx.vertices[2], matRotX);

            tri_ztranslated = tri_rzx;

            // TODO: Replace hard-coded z-offset with a proper view/camera system
            tri_ztranslated.vertices[0].z = tri_rzx.vertices[0].z + 10.0f;
            tri_ztranslated.vertices[1].z = tri_rzx.vertices[1].z + 10.0f;
            tri_ztranslated.vertices[2].z = tri_rzx.vertices[2].z + 10.0f;

            this->sim::proj(tri_ztranslated.vertices[0], out.vertices[0], this->scfg->proj_mat);
            this->sim::proj(tri_ztranslated.vertices[1], out.vertices[1], this->scfg->proj_mat);
            this->sim::proj(tri_ztranslated.vertices[2], out.vertices[2], this->scfg->proj_mat);

            // TODO: Implement a proper vector math library
            // IMPL: Create a dedicated normal calculation function
            sf::Vector3f normal, line1, line2;

            line1.x = out.vertices[1].x - out.vertices[0].x;
            line1.y = out.vertices[1].y - out.vertices[0].y;
            line1.z = out.vertices[1].z - out.vertices[0].z;

            line2.x = out.vertices[2].x - out.vertices[0].x;
            line2.y = out.vertices[2].y - out.vertices[0].y;
            line2.z = out.vertices[2].z - out.vertices[0].z;

            // TODO: Create cross product function in vector math library
            normal.x = line1.y * line2.z - line1.z * line2.y;
            normal.y = line1.z * line2.x - line1.x * line2.z;
            normal.z = line1.x * line2.y - line1.y * line2.x;

            // TODO: Create normalize function in vector math library
            float norm_normal = sqrtf(pow(normal.x, 2) + pow(normal.y, 2) + pow(normal.z, 2));
            normal.x /= norm_normal;
            normal.y /= norm_normal;
            normal.z /= norm_normal;

            // TODO: Create a proper camera class with configurable position
            sf::Vector3f camera{0, 0, 0};

            // TODO: Create dot product function in vector math library
            // IMPL: Implement proper back-face culling with configurable options
            float simil = normal.x * (out.vertices[0].x - camera.x) +
                          normal.y * (out.vertices[0].y - camera.y) +
                          normal.z * (out.vertices[0].z - camera.z);

            if (simil < 0) {
                out.set_depth();
                triangles_rasterized.push(out);
            }
        }

        // TODO: Implement a proper rendering backend with configurable render targets
        // HINT: Create a RenderTarget class that can be:
        //       - The main screen (window)
        //       - An offscreen framebuffer for techniques like shadow mapping
        //       - Render-to-texture for post-processing effects
        // IMPL: Consider using modern OpenGL/GPU acceleration instead of software rendering
        // HINT: Even a basic OpenGL setup with shaders will be much faster than software
        //       rendering. Look into modern OpenGL tutorials to get started with VBOs, VAOs,
        //       and shaders for efficient GPU-based rendering.
        while (!triangles_rasterized.empty()) {
            triangle tmp = triangles_rasterized.top();
            tmp.scale(window->getSize());
            tmp.draw(*window);
            triangles_rasterized.pop();
        }

        for (point3d point : mesh.get_vertices()) {
            if (!point.is_visible())
                continue;

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

        for (line3d line : mesh.get_edges()) {
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

            this->sim::proj(l_ztranslated.get_p1()->point_coords, out.get_p1()->point_coords,
                            this->scfg->proj_mat);
            this->sim::proj(l_ztranslated.get_p2()->point_coords, out.get_p2()->point_coords,
                            this->scfg->proj_mat);

            out.scale(window->getSize());
            out.draw(*window);
        }
    }

    for (point3d point : free_points) {
        point3d out{};
        point3d p_ztranslated{};
        point3d p_rz{};
        point3d p_rzx{};

        this->sim::proj(point.point_coords, p_rz.point_coords, matRotZ);

        this->sim::proj(p_rz.point_coords, p_rzx.point_coords, matRotX);

        p_ztranslated = p_rzx;

        p_ztranslated.point_coords.z = p_rzx.point_coords.z + 3.0f;

        this->sim::proj(p_ztranslated.point_coords, out.point_coords, this->scfg->proj_mat);

        out.scale(window->getSize());
        out.draw(*window);
    }
}

void sim::postrender() {}

void sim::start() {
    // TODO: Implement a proper game loop with fixed timestep updates
    // HINT: Use a fixed timestep loop like this:
    //       double t = 0.0;
    //       double dt = 0.01; // Fixed time step (e.g., 100 Hz)
    //       double currentTime = getCurrentTime();
    //       double accumulator = 0.0;
    //       
    //       while (running) {
    //           double newTime = getCurrentTime();
    //           double frameTime = newTime - currentTime;
    //           currentTime = newTime;
    //           accumulator += frameTime;
    //           
    //           while (accumulator >= dt) {
    //               updatePhysics(dt); // Fixed update step
    //               t += dt;
    //               accumulator -= dt;
    //           }
    //           
    //           render(); // Variable framerate rendering
    //       }
    // IMPL: Separate rendering frequency from physics/logic update frequency
    // TODO: Add proper input handling system and event manager
    // HINT: Create an InputManager class that:
    //       - Abstracts input devices (keyboard, mouse, gamepad)
    //       - Provides methods like IsKeyPressed(), GetMousePosition()
    //       - Allows binding actions to input (e.g., "Jump" -> Space key)
    //       - Supports an event system for callbacks when input occurs
    while (window->isOpen()) {
        while (const std::optional event = window->pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window->close();
            } else if (const auto* key_pressed = event->getIf<sf::Event::KeyPressed>()) {
                if (key_pressed->scancode == sf::Keyboard::Scancode::Escape) {
                    window->close();
                }
            }
        }

        window->clear(sf::Color::White);
        render();
        window->display();
    }
}
void sim::stop() {}

void sim::pause() {}

void sim::resume() {}
