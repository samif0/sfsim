# SFSim Refactoring Roadmap

This roadmap provides a structured approach to refactoring the simulation library, prioritizing changes based on impact and dependency order.

## Phase 1: Core Mathematics Foundation

Before anything else, create a solid foundation for all 3D operations:

1. **Vector Math Library** (1-2 weeks)
   - Create `math/vector.hpp` with:
     - `Vector2f`, `Vector3f`, `Vector4f` classes
     - Arithmetic operations (add, subtract, multiply, divide)
     - Dot and cross products, normalization, interpolation
   - Create `math/matrix.hpp` with:
     - `Matrix4x4` class with proper operations
     - Helper functions for rotation, translation, scaling matrices
     - Projection matrix generation (perspective, orthographic)
   - Unit tests for math operations

```cpp
// Example structure for Vector3f
class Vector3f {
public:
    float x, y, z;
    
    Vector3f() : x(0), y(0), z(0) {}
    Vector3f(float x, float y, float z) : x(x), y(y), z(z) {}
    
    // Arithmetic operators
    Vector3f operator+(const Vector3f& other) const;
    Vector3f operator-(const Vector3f& other) const;
    Vector3f operator*(float scalar) const;
    Vector3f operator/(float scalar) const;
    
    // Vector operations
    float dot(const Vector3f& other) const;
    Vector3f cross(const Vector3f& other) const;
    float length() const;
    Vector3f normalized() const;
    
    // Utilities
    static Vector3f lerp(const Vector3f& a, const Vector3f& b, float t);
};
```

## Phase 2: Rendering Infrastructure

With the math library in place, focus on rendering fundamentals:

1. **Camera System** (1 week)
   - Create `camera.hpp` with a `Camera` class that:
     - Stores position, target, up vector
     - Generates view and projection matrices
     - Has movement methods (pan, orbit, zoom)

2. **Transform System** (1 week)
   - Create `transform.hpp` with a `Transform` class that:
     - Stores position, rotation, scale
     - Computes local and world transformation matrices
     - Supports parent-child relationships

3. **Primitive Refactoring** (1-2 weeks)
   - Create `geometry/geometry.hpp` with a base `Geometry` class
   - Create derived classes for primitives in separate files:
     - `geometry/triangle.hpp`
     - `geometry/point.hpp`
     - `geometry/line.hpp`
   - Update them to use your new Vector/Matrix classes

## Phase 3: Scene Management

Now build scene management on top of the foundation:

1. **Basic Entity-Component System** (2 weeks)
   - Create `ecs/component.hpp` with a base `Component` class
   - Create initial components:
     - `ecs/transform_component.hpp`
     - `ecs/render_component.hpp`
   - Create `ecs/entity.hpp` with component management
   - Create `ecs/system.hpp` for the system base class

2. **Scene Graph** (1-2 weeks)
   - Create `scene/scene.hpp` that manages entities
   - Implement hierarchical transformations
   - Add scene traversal for rendering

## Phase 4: Rendering Pipeline

Improve the rendering with a proper pipeline:

1. **Renderer Abstraction** (2 weeks)
   - Create `renderer/renderer.hpp` for drawing abstraction
   - Implement software renderer first (current approach)
   - Prepare for future GPU acceleration

2. **Material System** (1 week)
   - Create `renderer/material.hpp` for surface properties
   - Support basic color and shading properties

3. **Mesh Improvements** (1-2 weeks)
   - Refactor mesh to use index buffers
   - Add support for different primitive types
   - Support Wavefront OBJ loading (simple format)

## Phase 5: Simulation Core

Now focus on the simulation aspects:

1. **Time Management** (1 week)
   - Create `core/time.hpp` for frame timing
   - Implement fixed timestep for physics
   - Separate update frequency from render frequency

2. **Input Handling** (1 week)
   - Create `core/input.hpp` for input abstraction
   - Map inputs to actions

3. **Physics Foundations** (2+ weeks)
   - Create `physics/physics.hpp` for a basic system
   - Implement basic collision detection
   - Add simple dynamics (velocity, acceleration)

## Implementation Strategy

For each component, follow this process:

1. **Design**
   - Write the interface (header file) first
   - Document the purpose of each function
   - Consider how it interacts with other systems

2. **Implement Core**
   - Start with minimal functionality
   - Focus on correctness before optimization

3. **Integrate**
   - Update existing code to use the new system
   - Keep old systems working until transition is complete

4. **Test**
   - Create examples that validate the system
   - Fix issues before moving to the next phase

## Getting Started Today

Begin with these immediate steps:

1. Create the math library directory and files
2. Port the matrix operations from sim.cpp to a Matrix4x4 class
3. Create Vector3f and start replacing sf::Vector3f usages
4. Create a simple test program to validate your math operations

Remember, refactoring is a gradual process. You don't need to change everything at once. Focus on creating solid foundations and incrementally improving the codebase.