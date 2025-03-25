# sfsim

A library for my future simulations.

--- 
# Milestones

Tracking certain milestones that get me closer to a good enough simuluation library.

## M1. Create rotating 3D cube with triangles (DONE)
8 verticies with 2 triangles per face. These triangles are drawn via perspective projection with field of view.
 <p align="center">
   <img src="https://github.com/user-attachments/assets/badebe55-ff11-491e-9842-f8fedcf79563" width="25%" height="25%"/>
</p>

#### Learned
- Using matricies to transform a point in space to another point in space.
- Efficient way to draw lines using Brasenham line algorithm.
- Getting lldb debugger to run in VScode with a CMake/VCPKG C++ project (rest my soul...)

## M2. Create a 3D grid (DONE)
Grids shown in example (width, height, depth, cell size):
 * (1, 1, 1, 0.1) 
 * (3, 3, 3, 0.3)
 * (5, 5, 5, 0.5)
 * (7, 7, 7, 0.7)
 * (11, 11, 11, 1.1)
 * (13, 13, 13, 1.3)
  
<p align="center">
   <img src="https://github.com/user-attachments/assets/2d55be1a-aac4-486c-98ca-5f170d0dd4a5" width="30%" height="30%"/>
</p>

#### Learned 
- How to connect edges in a 3d grid (and hiding verbose lines)

#### New thoughts
- Need to add a camera/view with movement control instead of depending on manual rotations.
- Passed a certain grid length there was performance issues. Need to optimze drawing and updates such that I can easily have at least a volume of 100000 units if I wanted.


## M3. Create some simple 3D objects (Spheres, Cones, Prisms)
## M4. Figure out how to make 3D objects interact and collide
## M5. Create a simple simulation of objects interacting in a 3D grid
## M6. Enact graviy in a simulation and visualize the change in the 3D grid.
## M7. Create a simple simulation of objects orbiting each other
## M8. Create a simple fluid simulation
## M9. Create a living system simulation where objects are "living" agents.

All I could think of as of now that would give me confidence that this library is good enough.
---

