#pragma once

#include "math/vector.hpp"
#include "math/matrix.hpp"

namespace SFSim {

using namespace Math;

class Camera {
public:
    Camera();
    Camera(const Vector3f& position, const Vector3f& target, const Vector3f& up = Vector3f::up());
    
    void setPosition(const Vector3f& position);
    void setTarget(const Vector3f& target);
    void setUp(const Vector3f& up);
    
    const Vector3f& getPosition() const { return _position; }
    const Vector3f& getTarget() const { return _target; }
    const Vector3f& getUp() const { return _up; }
    
    Vector3f getForward() const;
    Vector3f getRight() const;
    
    void setPerspective(float fovY, float aspect, float nearPlane, float farPlane);
    void setOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane);
    
    void lookAt(const Vector3f& position, const Vector3f& target, const Vector3f& up = Vector3f::up());
    
    void pan(float deltaX, float deltaY);
    void orbit(float deltaX, float deltaY);
    void zoom(float delta);
    void moveForward(float distance);
    void moveRight(float distance);
    void moveUp(float distance);
    
    void rotate(float deltaX, float deltaY, float deltaZ);
    void rotateX(float deltaAngle);
    void rotateY(float deltaAngle);
    void rotateZ(float deltaAngle);
    void setRotation(const Vector3f& rotation);
    const Vector3f& getRotation() const { return _rotation; }
    
    void rotateWithMouse(float mouseDeltaX, float mouseDeltaY, float sensitivity = 0.002f);
    void setMouseSensitivity(float sensitivity) { _mouseSensitivity = sensitivity; }
    float getMouseSensitivity() const { return _mouseSensitivity; }
    
    const Matrix4x4& getViewMatrix() const;
    const Matrix4x4& getProjectionMatrix() const;
    Matrix4x4 getViewProjectionMatrix() const;
    
    void updateMatrices();
    
private:
    Vector3f _position;
    Vector3f _target;
    Vector3f _up;
    Vector3f _rotation;
    
    float _mouseSensitivity;
    
    mutable Matrix4x4 _viewMatrix;
    mutable Matrix4x4 _projectionMatrix;
    mutable bool _viewMatrixDirty;
    
    bool _isPerspective;
    
    float _fovY;
    float _aspect;
    float _nearPlane;
    float _farPlane;
    
    float _left, _right, _bottom, _top;
    
    void updateViewMatrix() const;
};

} // namespace SFSim