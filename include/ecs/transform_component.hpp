#pragma once

#include "component.hpp"
#include "transform.hpp"

namespace SFSim {
namespace ECS {

class TransformComponent : public ComponentBase<TransformComponent> {
public:
    TransformComponent();
    TransformComponent(const Vector3f& position, const Vector3f& rotation = Vector3f::zero(), const Vector3f& scale = Vector3f::one());
    
    Transform& getTransform() { return _transform; }
    const Transform& getTransform() const { return _transform; }
    
    void setPosition(const Vector3f& position) { _transform.setPosition(position); }
    void setRotation(const Vector3f& rotation) { _transform.setRotation(rotation); }
    void setScale(const Vector3f& scale) { _transform.setScale(scale); }
    void setScale(float scale) { _transform.setScale(scale); }
    
    const Vector3f& getPosition() const { return _transform.getPosition(); }
    const Vector3f& getRotation() const { return _transform.getRotation(); }
    const Vector3f& getScale() const { return _transform.getScale(); }
    
    void translate(const Vector3f& translation) { _transform.translate(translation); }
    void rotate(const Vector3f& rotation) { _transform.rotate(rotation); }
    void scaleBy(const Vector3f& scale) { _transform.scaleBy(scale); }
    void scaleBy(float scale) { _transform.scaleBy(scale); }
    
    const Matrix4x4& getLocalMatrix() const { return _transform.getLocalMatrix(); }
    const Matrix4x4& getWorldMatrix() const { return _transform.getWorldMatrix(); }
    
private:
    Transform _transform;
};

} // namespace ECS
} // namespace SFSim