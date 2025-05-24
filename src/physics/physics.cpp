#include "physics/physics.hpp"
#include "ecs/transform_component.hpp"
#include <algorithm>
#include <cmath>

namespace SFSim {
namespace Physics {

bool AABB::contains(const Vector3f& point) const {
    return point.x >= min.x && point.x <= max.x &&
           point.y >= min.y && point.y <= max.y &&
           point.z >= min.z && point.z <= max.z;
}

bool AABB::intersects(const AABB& other) const {
    return !(other.min.x > max.x || other.max.x < min.x ||
             other.min.y > max.y || other.max.y < min.y ||
             other.min.z > max.z || other.max.z < min.z);
}

AABB AABB::merge(const AABB& other) const {
    return AABB(
        Vector3f(std::min(min.x, other.min.x), std::min(min.y, other.min.y), std::min(min.z, other.min.z)),
        Vector3f(std::max(max.x, other.max.x), std::max(max.y, other.max.y), std::max(max.z, other.max.z))
    );
}

void AABB::expand(const Vector3f& point) {
    min.x = std::min(min.x, point.x);
    min.y = std::min(min.y, point.y);
    min.z = std::min(min.z, point.z);
    max.x = std::max(max.x, point.x);
    max.y = std::max(max.y, point.y);
    max.z = std::max(max.z, point.z);
}

void AABB::expand(float amount) {
    Vector3f expansion(amount, amount, amount);
    min -= expansion;
    max += expansion;
}

bool Sphere::contains(const Vector3f& point) const {
    return (point - center).lengthSquared() <= radius * radius;
}

bool Sphere::intersects(const Sphere& other) const {
    float distanceSquared = (center - other.center).lengthSquared();
    float radiusSum = radius + other.radius;
    return distanceSquared <= radiusSum * radiusSum;
}

bool Sphere::intersects(const AABB& aabb) const {
    Vector3f closest = Vector3f(
        std::max(aabb.min.x, std::min(center.x, aabb.max.x)),
        std::max(aabb.min.y, std::min(center.y, aabb.max.y)),
        std::max(aabb.min.z, std::min(center.z, aabb.max.z))
    );
    
    return (closest - center).lengthSquared() <= radius * radius;
}

RigidbodyComponent::RigidbodyComponent()
    : _mass(1.0f)
    , _invMass(1.0f)
    , _velocity(Vector3f::zero())
    , _angularVelocity(Vector3f::zero())
    , _force(Vector3f::zero())
    , _torque(Vector3f::zero())
    , _drag(0.01f)
    , _angularDrag(0.05f)
    , _gravityScale(1.0f)
    , _kinematic(false)
{
}

ColliderComponent::ColliderComponent(Type type)
    : _type(type)
    , _size(Vector3f::one())
    , _radius(0.5f)
    , _height(1.0f)
    , _center(Vector3f::zero())
    , _isTrigger(false)
{
}

AABB ColliderComponent::getBounds(const Vector3f& position, const Vector3f& scale) const {
    Vector3f worldCenter = position + _center;
    
    switch (_type) {
        case Box: {
            Vector3f halfSize = (_size * scale) * 0.5f;
            return AABB(worldCenter - halfSize, worldCenter + halfSize);
        }
        case Sphere: {
            float scaledRadius = _radius * std::max({scale.x, scale.y, scale.z});
            Vector3f extent(scaledRadius, scaledRadius, scaledRadius);
            return AABB(worldCenter - extent, worldCenter + extent);
        }
        case Capsule: {
            float scaledRadius = _radius * std::max(scale.x, scale.z);
            float scaledHeight = _height * scale.y;
            float halfHeight = scaledHeight * 0.5f;
            Vector3f extent(scaledRadius, halfHeight + scaledRadius, scaledRadius);
            return AABB(worldCenter - extent, worldCenter + extent);
        }
    }
    return AABB();
}

Sphere ColliderComponent::getBoundingSphere(const Vector3f& position, const Vector3f& scale) const {
    Vector3f worldCenter = position + _center;
    
    switch (_type) {
        case Box: {
            float scaledRadius = (_size * scale).length() * 0.5f;
            return Sphere(worldCenter, scaledRadius);
        }
        case Sphere: {
            float scaledRadius = _radius * std::max({scale.x, scale.y, scale.z});
            return Sphere(worldCenter, scaledRadius);
        }
        case Capsule: {
            float scaledRadius = _radius * std::max(scale.x, scale.z);
            float scaledHeight = _height * scale.y;
            float totalRadius = scaledRadius + scaledHeight * 0.5f;
            return Sphere(worldCenter, totalRadius);
        }
    }
    return Sphere();
}

PhysicsSystem::PhysicsSystem()
    : _gravity(0, -9.81f, 0)
    , _simulationSpeed(1.0f)
{
}

void PhysicsSystem::update(float deltaTime, const std::vector<std::unique_ptr<Entity>>& entities) {
    float scaledDeltaTime = deltaTime * _simulationSpeed;
    
    auto rigidbodies = getEntitiesWith<RigidbodyComponent, TransformComponent>(entities);
    
    for (Entity* entity : rigidbodies) {
        auto* rigidbody = entity->getComponent<RigidbodyComponent>();
        
        if (!rigidbody->isKinematic()) {
            integrateVelocity(rigidbody, scaledDeltaTime);
            integratePosition(entity, rigidbody, scaledDeltaTime);
        }
        
        rigidbody->clearForces();
    }
    
    checkCollisions(entities);
}

RaycastHit PhysicsSystem::raycast(const Ray& ray, float maxDistance, const std::vector<std::unique_ptr<Entity>>& entities) {
    RaycastHit closestHit;
    closestHit.distance = maxDistance;
    
    auto colliders = getEntitiesWith<ColliderComponent, TransformComponent>(entities);
    
    for (Entity* entity : colliders) {
        auto* collider = entity->getComponent<ColliderComponent>();
        auto* transform = entity->getComponent<TransformComponent>();
        
        if (collider->isTrigger()) continue;
        
        AABB bounds = collider->getBounds(transform->getPosition(), transform->getScale());
        
        Vector3f invDir = Vector3f(1.0f / ray.direction.x, 1.0f / ray.direction.y, 1.0f / ray.direction.z);
        Vector3f t1 = (bounds.min - ray.origin) * invDir;
        Vector3f t2 = (bounds.max - ray.origin) * invDir;
        
        Vector3f tMin = Vector3f(std::min(t1.x, t2.x), std::min(t1.y, t2.y), std::min(t1.z, t2.z));
        Vector3f tMax = Vector3f(std::max(t1.x, t2.x), std::max(t1.y, t2.y), std::max(t1.z, t2.z));
        
        float tNear = std::max({tMin.x, tMin.y, tMin.z});
        float tFar = std::min({tMax.x, tMax.y, tMax.z});
        
        if (tNear <= tFar && tFar > 0 && tNear < closestHit.distance) {
            closestHit.hit = true;
            closestHit.distance = tNear > 0 ? tNear : tFar;
            closestHit.point = ray.getPoint(closestHit.distance);
            closestHit.entity = entity;
            
            Vector3f center = bounds.getCenter();
            Vector3f diff = closestHit.point - center;
            Vector3f size = bounds.getSize();
            
            Vector3f normal = Vector3f::zero();
            float maxComponent = 0;
            
            if (std::abs(diff.x / size.x) > maxComponent) {
                maxComponent = std::abs(diff.x / size.x);
                normal = Vector3f(diff.x > 0 ? 1 : -1, 0, 0);
            }
            if (std::abs(diff.y / size.y) > maxComponent) {
                maxComponent = std::abs(diff.y / size.y);
                normal = Vector3f(0, diff.y > 0 ? 1 : -1, 0);
            }
            if (std::abs(diff.z / size.z) > maxComponent) {
                normal = Vector3f(0, 0, diff.z > 0 ? 1 : -1);
            }
            
            closestHit.normal = normal;
        }
    }
    
    return closestHit;
}

std::vector<Entity*> PhysicsSystem::overlapSphere(const Vector3f& center, float radius, const std::vector<std::unique_ptr<Entity>>& entities) {
    std::vector<Entity*> overlapping;
    Sphere querySphere(center, radius);
    
    auto colliders = getEntitiesWith<ColliderComponent, TransformComponent>(entities);
    
    for (Entity* entity : colliders) {
        auto* collider = entity->getComponent<ColliderComponent>();
        auto* transform = entity->getComponent<TransformComponent>();
        
        Sphere entitySphere = collider->getBoundingSphere(transform->getPosition(), transform->getScale());
        
        if (querySphere.intersects(entitySphere)) {
            overlapping.push_back(entity);
        }
    }
    
    return overlapping;
}

std::vector<Entity*> PhysicsSystem::overlapBox(const Vector3f& center, const Vector3f& size, const std::vector<std::unique_ptr<Entity>>& entities) {
    std::vector<Entity*> overlapping;
    Vector3f halfSize = size * 0.5f;
    AABB queryBox(center - halfSize, center + halfSize);
    
    auto colliders = getEntitiesWith<ColliderComponent, TransformComponent>(entities);
    
    for (Entity* entity : colliders) {
        auto* collider = entity->getComponent<ColliderComponent>();
        auto* transform = entity->getComponent<TransformComponent>();
        
        AABB entityBounds = collider->getBounds(transform->getPosition(), transform->getScale());
        
        if (queryBox.intersects(entityBounds)) {
            overlapping.push_back(entity);
        }
    }
    
    return overlapping;
}

void PhysicsSystem::integrateVelocity(RigidbodyComponent* rigidbody, float deltaTime) {
    Vector3f acceleration = rigidbody->getForce() * rigidbody->getInverseMass();
    acceleration += _gravity * rigidbody->getGravityScale();
    
    rigidbody->setVelocity(rigidbody->getVelocity() + acceleration * deltaTime);
    
    Vector3f velocity = rigidbody->getVelocity();
    velocity = velocity * (1.0f - rigidbody->getDrag() * deltaTime);
    rigidbody->setVelocity(velocity);
    
    Vector3f angularVelocity = rigidbody->getAngularVelocity();
    angularVelocity = angularVelocity * (1.0f - rigidbody->getAngularDrag() * deltaTime);
    rigidbody->setAngularVelocity(angularVelocity);
}

void PhysicsSystem::integratePosition(Entity* entity, RigidbodyComponent* rigidbody, float deltaTime) {
    auto* transform = entity->getComponent<TransformComponent>();
    if (!transform) return;
    
    Vector3f position = transform->getPosition();
    position += rigidbody->getVelocity() * deltaTime;
    transform->setPosition(position);
    
    Vector3f rotation = transform->getRotation();
    rotation += rigidbody->getAngularVelocity() * deltaTime;
    transform->setRotation(rotation);
}

void PhysicsSystem::checkCollisions(const std::vector<std::unique_ptr<Entity>>& entities) {
    auto colliders = getEntitiesWith<ColliderComponent, TransformComponent>(entities);
    
    for (size_t i = 0; i < colliders.size(); ++i) {
        for (size_t j = i + 1; j < colliders.size(); ++j) {
            if (checkCollision(colliders[i], colliders[j])) {
            }
        }
    }
}

bool PhysicsSystem::checkCollision(Entity* entityA, Entity* entityB) {
    auto* colliderA = entityA->getComponent<ColliderComponent>();
    auto* colliderB = entityB->getComponent<ColliderComponent>();
    auto* transformA = entityA->getComponent<TransformComponent>();
    auto* transformB = entityB->getComponent<TransformComponent>();
    
    if (!colliderA || !colliderB || !transformA || !transformB) return false;
    
    if (colliderA->getType() == ColliderComponent::Box && colliderB->getType() == ColliderComponent::Box) {
        AABB boundsA = colliderA->getBounds(transformA->getPosition(), transformA->getScale());
        AABB boundsB = colliderB->getBounds(transformB->getPosition(), transformB->getScale());
        return checkAABBCollision(boundsA, boundsB);
    }
    else if (colliderA->getType() == ColliderComponent::Sphere && colliderB->getType() == ColliderComponent::Sphere) {
        Sphere sphereA = colliderA->getBoundingSphere(transformA->getPosition(), transformA->getScale());
        Sphere sphereB = colliderB->getBoundingSphere(transformB->getPosition(), transformB->getScale());
        return checkSphereCollision(sphereA, sphereB);
    }
    else {
        AABB boundsA = colliderA->getBounds(transformA->getPosition(), transformA->getScale());
        AABB boundsB = colliderB->getBounds(transformB->getPosition(), transformB->getScale());
        return checkAABBCollision(boundsA, boundsB);
    }
}

bool PhysicsSystem::checkAABBCollision(const AABB& a, const AABB& b) {
    return a.intersects(b);
}

bool PhysicsSystem::checkSphereCollision(const Sphere& a, const Sphere& b) {
    return a.intersects(b);
}

bool PhysicsSystem::checkSpherAABBCollision(const Sphere& sphere, const AABB& aabb) {
    return sphere.intersects(aabb);
}

void PhysicsSystem::resolveCollision(Entity* entityA, Entity* entityB, const Vector3f& normal, float penetration) {
    auto* rigidbodyA = entityA->getComponent<RigidbodyComponent>();
    auto* rigidbodyB = entityB->getComponent<RigidbodyComponent>();
    auto* transformA = entityA->getComponent<TransformComponent>();
    auto* transformB = entityB->getComponent<TransformComponent>();
    
    if (!transformA || !transformB) return;
    
    if (!rigidbodyA && !rigidbodyB) return;
    
    float invMassA = rigidbodyA ? rigidbodyA->getInverseMass() : 0.0f;
    float invMassB = rigidbodyB ? rigidbodyB->getInverseMass() : 0.0f;
    float totalInvMass = invMassA + invMassB;
    
    if (totalInvMass == 0) return;
    
    Vector3f separation = normal * (penetration / totalInvMass);
    
    if (rigidbodyA && !rigidbodyA->isKinematic()) {
        Vector3f posA = transformA->getPosition();
        posA -= separation * invMassA;
        transformA->setPosition(posA);
    }
    
    if (rigidbodyB && !rigidbodyB->isKinematic()) {
        Vector3f posB = transformB->getPosition();
        posB += separation * invMassB;
        transformB->setPosition(posB);
    }
    
    Vector3f relativeVelocity = Vector3f::zero();
    if (rigidbodyA) relativeVelocity -= rigidbodyA->getVelocity();
    if (rigidbodyB) relativeVelocity += rigidbodyB->getVelocity();
    
    float velAlongNormal = relativeVelocity.dot(normal);
    
    if (velAlongNormal > 0) return;
    
    float restitution = 0.5f;
    float impulseScalar = -(1 + restitution) * velAlongNormal / totalInvMass;
    Vector3f impulse = normal * impulseScalar;
    
    if (rigidbodyA && !rigidbodyA->isKinematic()) {
        rigidbodyA->addImpulse(-impulse);
    }
    
    if (rigidbodyB && !rigidbodyB->isKinematic()) {
        rigidbodyB->addImpulse(impulse);
    }
}

} // namespace Physics
} // namespace SFSim