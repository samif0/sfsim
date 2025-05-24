#pragma once

#include "math/vector.hpp"
#include "ecs/component.hpp"
#include "ecs/system.hpp"
#include <vector>
#include <memory>

namespace SFSim {
namespace Physics {

using namespace Math;
using namespace ECS;

struct AABB {
    Vector3f min;
    Vector3f max;
    
    AABB() : min(Vector3f::zero()), max(Vector3f::zero()) {}
    AABB(const Vector3f& minPoint, const Vector3f& maxPoint) : min(minPoint), max(maxPoint) {}
    
    Vector3f getCenter() const { return (min + max) * 0.5f; }
    Vector3f getSize() const { return max - min; }
    Vector3f getExtents() const { return getSize() * 0.5f; }
    
    bool contains(const Vector3f& point) const;
    bool intersects(const AABB& other) const;
    AABB merge(const AABB& other) const;
    void expand(const Vector3f& point);
    void expand(float amount);
};

struct Sphere {
    Vector3f center;
    float radius;
    
    Sphere() : center(Vector3f::zero()), radius(1.0f) {}
    Sphere(const Vector3f& c, float r) : center(c), radius(r) {}
    
    bool contains(const Vector3f& point) const;
    bool intersects(const Sphere& other) const;
    bool intersects(const AABB& aabb) const;
};

struct Ray {
    Vector3f origin;
    Vector3f direction;
    
    Ray() : origin(Vector3f::zero()), direction(Vector3f::forward()) {}
    Ray(const Vector3f& o, const Vector3f& d) : origin(o), direction(d.normalized()) {}
    
    Vector3f getPoint(float t) const { return origin + direction * t; }
};

struct RaycastHit {
    bool hit;
    Vector3f point;
    Vector3f normal;
    float distance;
    Entity* entity;
    
    RaycastHit() : hit(false), point(Vector3f::zero()), normal(Vector3f::up()), distance(0.0f), entity(nullptr) {}
};

class RigidbodyComponent : public ComponentBase<RigidbodyComponent> {
public:
    RigidbodyComponent();
    
    void setMass(float mass) { _mass = mass; _invMass = (mass > 0) ? 1.0f / mass : 0.0f; }
    float getMass() const { return _mass; }
    float getInverseMass() const { return _invMass; }
    
    void setVelocity(const Vector3f& velocity) { _velocity = velocity; }
    const Vector3f& getVelocity() const { return _velocity; }
    
    void setAngularVelocity(const Vector3f& angularVelocity) { _angularVelocity = angularVelocity; }
    const Vector3f& getAngularVelocity() const { return _angularVelocity; }
    
    void setDrag(float drag) { _drag = drag; }
    float getDrag() const { return _drag; }
    
    void setAngularDrag(float angularDrag) { _angularDrag = angularDrag; }
    float getAngularDrag() const { return _angularDrag; }
    
    void setGravityScale(float scale) { _gravityScale = scale; }
    float getGravityScale() const { return _gravityScale; }
    
    void setKinematic(bool kinematic) { _kinematic = kinematic; }
    bool isKinematic() const { return _kinematic; }
    
    void addForce(const Vector3f& force) { _force += force; }
    void addImpulse(const Vector3f& impulse) { _velocity += impulse * _invMass; }
    void addTorque(const Vector3f& torque) { _torque += torque; }
    
    void clearForces() { _force = Vector3f::zero(); _torque = Vector3f::zero(); }
    
    const Vector3f& getForce() const { return _force; }
    const Vector3f& getTorque() const { return _torque; }
    
private:
    float _mass;
    float _invMass;
    Vector3f _velocity;
    Vector3f _angularVelocity;
    Vector3f _force;
    Vector3f _torque;
    float _drag;
    float _angularDrag;
    float _gravityScale;
    bool _kinematic;
};

class ColliderComponent : public ComponentBase<ColliderComponent> {
public:
    enum Type { Box, Sphere, Capsule };
    
    ColliderComponent(Type type = Box);
    
    void setType(Type type) { _type = type; }
    Type getType() const { return _type; }
    
    void setSize(const Vector3f& size) { _size = size; }
    const Vector3f& getSize() const { return _size; }
    
    void setRadius(float radius) { _radius = radius; }
    float getRadius() const { return _radius; }
    
    void setHeight(float height) { _height = height; }
    float getHeight() const { return _height; }
    
    void setCenter(const Vector3f& center) { _center = center; }
    const Vector3f& getCenter() const { return _center; }
    
    void setTrigger(bool trigger) { _isTrigger = trigger; }
    bool isTrigger() const { return _isTrigger; }
    
    AABB getBounds(const Vector3f& position, const Vector3f& scale) const;
    Sphere getBoundingSphere(const Vector3f& position, const Vector3f& scale) const;
    
private:
    Type _type;
    Vector3f _size;
    float _radius;
    float _height;
    Vector3f _center;
    bool _isTrigger;
};

class PhysicsSystem : public System {
public:
    PhysicsSystem();
    
    void update(float deltaTime, const std::vector<std::unique_ptr<Entity>>& entities) override;
    
    void setGravity(const Vector3f& gravity) { _gravity = gravity; }
    const Vector3f& getGravity() const { return _gravity; }
    
    void setSimulationSpeed(float speed) { _simulationSpeed = speed; }
    float getSimulationSpeed() const { return _simulationSpeed; }
    
    RaycastHit raycast(const Ray& ray, float maxDistance = 1000.0f, const std::vector<std::unique_ptr<Entity>>& entities = {});
    std::vector<Entity*> overlapSphere(const Vector3f& center, float radius, const std::vector<std::unique_ptr<Entity>>& entities);
    std::vector<Entity*> overlapBox(const Vector3f& center, const Vector3f& size, const std::vector<std::unique_ptr<Entity>>& entities);
    
private:
    Vector3f _gravity;
    float _simulationSpeed;
    
    void integrateVelocity(RigidbodyComponent* rigidbody, float deltaTime);
    void integratePosition(Entity* entity, RigidbodyComponent* rigidbody, float deltaTime);
    void checkCollisions(const std::vector<std::unique_ptr<Entity>>& entities);
    
    bool checkCollision(Entity* entityA, Entity* entityB);
    bool checkAABBCollision(const AABB& a, const AABB& b);
    bool checkSphereCollision(const Sphere& a, const Sphere& b);
    bool checkSpherAABBCollision(const Sphere& sphere, const AABB& aabb);
    
    void resolveCollision(Entity* entityA, Entity* entityB, const Vector3f& normal, float penetration);
};

} // namespace Physics
} // namespace SFSim