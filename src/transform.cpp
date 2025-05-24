#include "transform.hpp"
#include <algorithm>
#include <cmath>

namespace SFSim {

Transform::Transform()
    : _position(Vector3f::zero())
    , _rotation(Vector3f::zero())
    , _scale(Vector3f::one())
    , _localMatrixDirty(true)
    , _worldMatrixDirty(true)
    , _parent(nullptr)
{
}

Transform::Transform(const Vector3f& position, const Vector3f& rotation, const Vector3f& scale)
    : _position(position)
    , _rotation(rotation)
    , _scale(scale)
    , _localMatrixDirty(true)
    , _worldMatrixDirty(true)
    , _parent(nullptr)
{
}

void Transform::setPosition(const Vector3f& position) {
    _position = position;
    markDirty();
}

void Transform::setRotation(const Vector3f& rotation) {
    _rotation = rotation;
    markDirty();
}

void Transform::setScale(const Vector3f& scale) {
    _scale = scale;
    markDirty();
}

void Transform::setScale(float scale) {
    _scale = Vector3f(scale, scale, scale);
    markDirty();
}

void Transform::translate(const Vector3f& translation) {
    _position += translation;
    markDirty();
}

void Transform::rotate(const Vector3f& rotation) {
    _rotation += rotation;
    markDirty();
}

void Transform::scaleBy(const Vector3f& scale) {
    _scale.x *= scale.x;
    _scale.y *= scale.y;
    _scale.z *= scale.z;
    markDirty();
}

void Transform::scaleBy(float scale) {
    _scale *= scale;
    markDirty();
}

Vector3f Transform::getForward() const {
    Matrix4x4 rotMatrix = Matrix4x4::rotationX(_rotation.x) * 
                          Matrix4x4::rotationY(_rotation.y) * 
                          Matrix4x4::rotationZ(_rotation.z);
    return rotMatrix.transformDirection(Vector3f::forward());
}

Vector3f Transform::getRight() const {
    Matrix4x4 rotMatrix = Matrix4x4::rotationX(_rotation.x) * 
                          Matrix4x4::rotationY(_rotation.y) * 
                          Matrix4x4::rotationZ(_rotation.z);
    return rotMatrix.transformDirection(Vector3f::right());
}

Vector3f Transform::getUp() const {
    Matrix4x4 rotMatrix = Matrix4x4::rotationX(_rotation.x) * 
                          Matrix4x4::rotationY(_rotation.y) * 
                          Matrix4x4::rotationZ(_rotation.z);
    return rotMatrix.transformDirection(Vector3f::up());
}

const Matrix4x4& Transform::getLocalMatrix() const {
    if (_localMatrixDirty) {
        updateLocalMatrix();
    }
    return _localMatrix;
}

const Matrix4x4& Transform::getWorldMatrix() const {
    if (_worldMatrixDirty) {
        updateWorldMatrix();
    }
    return _worldMatrix;
}

void Transform::setParent(Transform* parent) {
    if (_parent) {
        _parent->removeChild(this);
    }
    
    _parent = parent;
    
    if (_parent) {
        _parent->addChild(this);
    }
    
    markDirty();
}

void Transform::addChild(Transform* child) {
    if (child && std::find(_children.begin(), _children.end(), child) == _children.end()) {
        _children.push_back(child);
        if (child->_parent != this) {
            child->setParent(this);
        }
    }
}

void Transform::removeChild(Transform* child) {
    auto it = std::find(_children.begin(), _children.end(), child);
    if (it != _children.end()) {
        _children.erase(it);
        if (child->_parent == this) {
            child->_parent = nullptr;
            child->markDirty();
        }
    }
}

Vector3f Transform::getWorldPosition() const {
    if (_parent) {
        return _parent->getWorldMatrix().transformPoint(_position);
    }
    return _position;
}

Vector3f Transform::getWorldRotation() const {
    if (_parent) {
        return _parent->getWorldRotation() + _rotation;
    }
    return _rotation;
}

Vector3f Transform::getWorldScale() const {
    if (_parent) {
        Vector3f parentScale = _parent->getWorldScale();
        return Vector3f(_scale.x * parentScale.x, _scale.y * parentScale.y, _scale.z * parentScale.z);
    }
    return _scale;
}

Vector3f Transform::transformPoint(const Vector3f& point) const {
    return getWorldMatrix().transformPoint(point);
}

Vector3f Transform::transformDirection(const Vector3f& direction) const {
    return getWorldMatrix().transformDirection(direction);
}

Vector3f Transform::inverseTransformPoint(const Vector3f& point) const {
    return getWorldMatrix().inverted().transformPoint(point);
}

Vector3f Transform::inverseTransformDirection(const Vector3f& direction) const {
    return getWorldMatrix().inverted().transformDirection(direction);
}

void Transform::markDirty() {
    _localMatrixDirty = true;
    _worldMatrixDirty = true;
    markChildrenWorldMatrixDirty();
}

void Transform::updateLocalMatrix() const {
    Matrix4x4 translationMatrix = Matrix4x4::translation(_position);
    Matrix4x4 rotationMatrix = Matrix4x4::rotationX(_rotation.x) * 
                              Matrix4x4::rotationY(_rotation.y) * 
                              Matrix4x4::rotationZ(_rotation.z);
    Matrix4x4 scaleMatrix = Matrix4x4::scale(_scale);
    
    _localMatrix = translationMatrix * rotationMatrix * scaleMatrix;
    _localMatrixDirty = false;
}

void Transform::updateWorldMatrix() const {
    if (_localMatrixDirty) {
        updateLocalMatrix();
    }
    
    if (_parent) {
        _worldMatrix = _parent->getWorldMatrix() * _localMatrix;
    } else {
        _worldMatrix = _localMatrix;
    }
    
    _worldMatrixDirty = false;
}

void Transform::markChildrenWorldMatrixDirty() {
    for (Transform* child : _children) {
        child->_worldMatrixDirty = true;
        child->markChildrenWorldMatrixDirty();
    }
}

} // namespace SFSim