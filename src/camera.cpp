#include "camera.hpp"

#include <algorithm>
#include <cmath>

namespace SFSim {

Camera::Camera()
    : _position(0, 0, 5),
      _target(0, 0, 0),
      _up(0, 1, 0),
      _viewMatrixDirty(true),
      _isPerspective(true),
      _fovY(M_PI / 4),
      _aspect(16.0f / 9.0f),
      _nearPlane(0.1f),
      _farPlane(100.0f),
      _left(-1),
      _right(1),
      _bottom(-1),
      _top(1) {
    updateMatrices();
}

Camera::Camera(const Vector3f& position, const Vector3f& target, const Vector3f& up)
    : _position(position),
      _target(target),
      _up(up),
      _viewMatrixDirty(true),
      _isPerspective(true),
      _fovY(M_PI / 4),
      _aspect(16.0f / 9.0f),
      _nearPlane(0.1f),
      _farPlane(100.0f),
      _left(-1),
      _right(1),
      _bottom(-1),
      _top(1) {
    updateMatrices();
}

void Camera::setPosition(const Vector3f& position) {
    _position = position;
    _viewMatrixDirty = true;
}

void Camera::setTarget(const Vector3f& target) {
    _target = target;
    _viewMatrixDirty = true;
}

void Camera::setUp(const Vector3f& up) {
    _up = up;
    _viewMatrixDirty = true;
}

Vector3f Camera::getForward() const { return (_target - _position).normalized(); }

Vector3f Camera::getRight() const { return getForward().cross(_up).normalized(); }

void Camera::setPerspective(float fovY, float aspect, float nearPlane, float farPlane) {
    _isPerspective = true;
    _fovY = fovY;
    _aspect = aspect;
    _nearPlane = nearPlane;
    _farPlane = farPlane;

    _projectionMatrix = Matrix4x4::perspective(_fovY, _aspect, _nearPlane, _farPlane);
}

void Camera::setOrthographic(float left, float right, float bottom, float top, float nearPlane,
                             float farPlane) {
    _isPerspective = false;
    _left = left;
    _right = right;
    _bottom = bottom;
    _top = top;
    _nearPlane = nearPlane;
    _farPlane = farPlane;

    _projectionMatrix =
        Matrix4x4::orthographic(_left, _right, _bottom, _top, _nearPlane, _farPlane);
}

void Camera::lookAt(const Vector3f& position, const Vector3f& target, const Vector3f& up) {
    _position = position;
    _target = target;
    _up = up;
    _viewMatrixDirty = true;
}

void Camera::pan(float deltaX, float deltaY) {
    Vector3f right = getRight();
    Vector3f up = _up;

    Vector3f offset = right * deltaX + up * deltaY;
    _position += offset;
    _target += offset;
    _viewMatrixDirty = true;
}

void Camera::orbit(float deltaX, float deltaY) {
    Vector3f toCamera = _position - _target;
    float distance = toCamera.length();

    float theta = std::atan2(toCamera.x, toCamera.z) + deltaX;
    float phi = std::acos(toCamera.y / distance) + deltaY;

    phi = std::fmax(0.1f, std::fmin(M_PI - 0.1f, phi));

    _position.x = _target.x + distance * std::sin(phi) * std::sin(theta);
    _position.y = _target.y + distance * std::cos(phi);
    _position.z = _target.z + distance * std::sin(phi) * std::cos(theta);

    _viewMatrixDirty = true;
}

void Camera::zoom(float delta) {
    Vector3f direction = (_position - _target).normalized();
    float distance = (_position - _target).length();
    distance = std::fmax(0.1f, distance + delta);

    _position = _target + direction * distance;
    _viewMatrixDirty = true;
}

void Camera::moveForward(float distance) {
    Vector3f forward = getForward();
    _position += forward * distance;
    _target += forward * distance;
    _viewMatrixDirty = true;
}

void Camera::moveRight(float distance) {
    Vector3f right = getRight();
    _position += right * distance;
    _target += right * distance;
    _viewMatrixDirty = true;
}

void Camera::moveUp(float distance) {
    _position += _up * distance;
    _target += _up * distance;
    _viewMatrixDirty = true;
}

const Matrix4x4& Camera::getViewMatrix() const {
    if (_viewMatrixDirty) {
        updateViewMatrix();
    }
    return _viewMatrix;
}

const Matrix4x4& Camera::getProjectionMatrix() const { return _projectionMatrix; }

Matrix4x4 Camera::getViewProjectionMatrix() const {
    return getProjectionMatrix() * getViewMatrix();
}

void Camera::updateMatrices() {
    updateViewMatrix();

    if (_isPerspective) {
        _projectionMatrix = Matrix4x4::perspective(_fovY, _aspect, _nearPlane, _farPlane);
    } else {
        _projectionMatrix =
            Matrix4x4::orthographic(_left, _right, _bottom, _top, _nearPlane, _farPlane);
    }
}

void Camera::updateViewMatrix() const {
    _viewMatrix = Matrix4x4::lookAt(_position, _target, _up);
    _viewMatrixDirty = false;
}

}  // namespace SFSim
