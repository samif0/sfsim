#pragma once

#include "math/vector.hpp"
#include "math/matrix.hpp"
#include <vector>
#include <memory>

namespace SFSim {

using namespace Math;

class Transform {
public:
    Transform();
    Transform(const Vector3f& position, const Vector3f& rotation = Vector3f::zero(), const Vector3f& scale = Vector3f::one());
    
    void setPosition(const Vector3f& position);
    void setRotation(const Vector3f& rotation);
    void setScale(const Vector3f& scale);
    void setScale(float scale);
    
    const Vector3f& getPosition() const { return _position; }
    const Vector3f& getRotation() const { return _rotation; }
    const Vector3f& getScale() const { return _scale; }
    
    void translate(const Vector3f& translation);
    void rotate(const Vector3f& rotation);
    void scaleBy(const Vector3f& scale);
    void scaleBy(float scale);
    
    Vector3f getForward() const;
    Vector3f getRight() const;
    Vector3f getUp() const;
    
    const Matrix4x4& getLocalMatrix() const;
    const Matrix4x4& getWorldMatrix() const;
    
    void setParent(Transform* parent);
    Transform* getParent() const { return _parent; }
    
    void addChild(Transform* child);
    void removeChild(Transform* child);
    const std::vector<Transform*>& getChildren() const { return _children; }
    
    Vector3f getWorldPosition() const;
    Vector3f getWorldRotation() const;
    Vector3f getWorldScale() const;
    
    Vector3f transformPoint(const Vector3f& point) const;
    Vector3f transformDirection(const Vector3f& direction) const;
    Vector3f inverseTransformPoint(const Vector3f& point) const;
    Vector3f inverseTransformDirection(const Vector3f& direction) const;
    
    void markDirty();
    
private:
    Vector3f _position;
    Vector3f _rotation;
    Vector3f _scale;
    
    mutable Matrix4x4 _localMatrix;
    mutable Matrix4x4 _worldMatrix;
    mutable bool _localMatrixDirty;
    mutable bool _worldMatrixDirty;
    
    Transform* _parent;
    std::vector<Transform*> _children;
    
    void updateLocalMatrix() const;
    void updateWorldMatrix() const;
    void markChildrenWorldMatrixDirty();
};

} // namespace SFSim