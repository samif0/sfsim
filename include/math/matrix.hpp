#pragma once

#include "vector.hpp"
#include <cmath>

namespace SFSim {
namespace Math {

class Matrix4x4 {
public:
    float m[16];
    
    Matrix4x4() {
        identity();
    }
    
    Matrix4x4(float m00, float m01, float m02, float m03,
              float m10, float m11, float m12, float m13,
              float m20, float m21, float m22, float m23,
              float m30, float m31, float m32, float m33) {
        m[0]  = m00; m[1]  = m01; m[2]  = m02; m[3]  = m03;
        m[4]  = m10; m[5]  = m11; m[6]  = m12; m[7]  = m13;
        m[8]  = m20; m[9]  = m21; m[10] = m22; m[11] = m23;
        m[12] = m30; m[13] = m31; m[14] = m32; m[15] = m33;
    }
    
    float& operator()(int row, int col) {
        return m[row * 4 + col];
    }
    
    const float& operator()(int row, int col) const {
        return m[row * 4 + col];
    }
    
    float& operator[](int index) {
        return m[index];
    }
    
    const float& operator[](int index) const {
        return m[index];
    }
    
    Matrix4x4 operator*(const Matrix4x4& other) const {
        Matrix4x4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result(i, j) = 0;
                for (int k = 0; k < 4; ++k) {
                    result(i, j) += (*this)(i, k) * other(k, j);
                }
            }
        }
        return result;
    }
    
    Matrix4x4& operator*=(const Matrix4x4& other) {
        *this = *this * other;
        return *this;
    }
    
    Vector4f operator*(const Vector4f& v) const {
        return Vector4f(
            m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3] * v.w,
            m[4] * v.x + m[5] * v.y + m[6] * v.z + m[7] * v.w,
            m[8] * v.x + m[9] * v.y + m[10] * v.z + m[11] * v.w,
            m[12] * v.x + m[13] * v.y + m[14] * v.z + m[15] * v.w
        );
    }
    
    Vector3f transformPoint(const Vector3f& v) const {
        Vector4f result = *this * Vector4f(v, 1.0f);
        if (result.w != 0) {
            return Vector3f(result.x / result.w, result.y / result.w, result.z / result.w);
        }
        return result.xyz();
    }
    
    Vector3f transformDirection(const Vector3f& v) const {
        Vector4f result = *this * Vector4f(v, 0.0f);
        return result.xyz();
    }
    
    void identity() {
        m[0] = 1; m[1] = 0; m[2] = 0; m[3] = 0;
        m[4] = 0; m[5] = 1; m[6] = 0; m[7] = 0;
        m[8] = 0; m[9] = 0; m[10] = 1; m[11] = 0;
        m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
    }
    
    Matrix4x4 transposed() const {
        return Matrix4x4(
            m[0], m[4], m[8],  m[12],
            m[1], m[5], m[9],  m[13],
            m[2], m[6], m[10], m[14],
            m[3], m[7], m[11], m[15]
        );
    }
    
    void transpose() {
        *this = transposed();
    }
    
    float determinant() const {
        float det = 
            m[0] * (m[5] * (m[10] * m[15] - m[11] * m[14]) -
                    m[6] * (m[9] * m[15] - m[11] * m[13]) +
                    m[7] * (m[9] * m[14] - m[10] * m[13])) -
            m[1] * (m[4] * (m[10] * m[15] - m[11] * m[14]) -
                    m[6] * (m[8] * m[15] - m[11] * m[12]) +
                    m[7] * (m[8] * m[14] - m[10] * m[12])) +
            m[2] * (m[4] * (m[9] * m[15] - m[11] * m[13]) -
                    m[5] * (m[8] * m[15] - m[11] * m[12]) +
                    m[7] * (m[8] * m[13] - m[9] * m[12])) -
            m[3] * (m[4] * (m[9] * m[14] - m[10] * m[13]) -
                    m[5] * (m[8] * m[14] - m[10] * m[12]) +
                    m[6] * (m[8] * m[13] - m[9] * m[12]));
        return det;
    }
    
    Matrix4x4 inverted() const {
        float det = determinant();
        if (std::abs(det) < 1e-6f) {
            return Matrix4x4();
        }
        
        Matrix4x4 inv;
        inv[0] = m[5] * (m[10] * m[15] - m[11] * m[14]) -
                 m[6] * (m[9] * m[15] - m[11] * m[13]) +
                 m[7] * (m[9] * m[14] - m[10] * m[13]);
        inv[1] = -m[1] * (m[10] * m[15] - m[11] * m[14]) +
                 m[2] * (m[9] * m[15] - m[11] * m[13]) -
                 m[3] * (m[9] * m[14] - m[10] * m[13]);
        inv[2] = m[1] * (m[6] * m[15] - m[7] * m[14]) -
                 m[2] * (m[5] * m[15] - m[7] * m[13]) +
                 m[3] * (m[5] * m[14] - m[6] * m[13]);
        inv[3] = -m[1] * (m[6] * m[11] - m[7] * m[10]) +
                 m[2] * (m[5] * m[11] - m[7] * m[9]) -
                 m[3] * (m[5] * m[10] - m[6] * m[9]);
        
        inv[4] = -m[4] * (m[10] * m[15] - m[11] * m[14]) +
                 m[6] * (m[8] * m[15] - m[11] * m[12]) -
                 m[7] * (m[8] * m[14] - m[10] * m[12]);
        inv[5] = m[0] * (m[10] * m[15] - m[11] * m[14]) -
                 m[2] * (m[8] * m[15] - m[11] * m[12]) +
                 m[3] * (m[8] * m[14] - m[10] * m[12]);
        inv[6] = -m[0] * (m[6] * m[15] - m[7] * m[14]) +
                 m[2] * (m[4] * m[15] - m[7] * m[12]) -
                 m[3] * (m[4] * m[14] - m[6] * m[12]);
        inv[7] = m[0] * (m[6] * m[11] - m[7] * m[10]) -
                 m[2] * (m[4] * m[11] - m[7] * m[8]) +
                 m[3] * (m[4] * m[10] - m[6] * m[8]);
        
        inv[8] = m[4] * (m[9] * m[15] - m[11] * m[13]) -
                 m[5] * (m[8] * m[15] - m[11] * m[12]) +
                 m[7] * (m[8] * m[13] - m[9] * m[12]);
        inv[9] = -m[0] * (m[9] * m[15] - m[11] * m[13]) +
                 m[1] * (m[8] * m[15] - m[11] * m[12]) -
                 m[3] * (m[8] * m[13] - m[9] * m[12]);
        inv[10] = m[0] * (m[5] * m[15] - m[7] * m[13]) -
                  m[1] * (m[4] * m[15] - m[7] * m[12]) +
                  m[3] * (m[4] * m[13] - m[5] * m[12]);
        inv[11] = -m[0] * (m[5] * m[11] - m[7] * m[9]) +
                  m[1] * (m[4] * m[11] - m[7] * m[8]) -
                  m[3] * (m[4] * m[9] - m[5] * m[8]);
        
        inv[12] = -m[4] * (m[9] * m[14] - m[10] * m[13]) +
                  m[5] * (m[8] * m[14] - m[10] * m[12]) -
                  m[6] * (m[8] * m[13] - m[9] * m[12]);
        inv[13] = m[0] * (m[9] * m[14] - m[10] * m[13]) -
                  m[1] * (m[8] * m[14] - m[10] * m[12]) +
                  m[2] * (m[8] * m[13] - m[9] * m[12]);
        inv[14] = -m[0] * (m[5] * m[14] - m[6] * m[13]) +
                  m[1] * (m[4] * m[14] - m[6] * m[12]) -
                  m[2] * (m[4] * m[13] - m[5] * m[12]);
        inv[15] = m[0] * (m[5] * m[10] - m[6] * m[9]) -
                  m[1] * (m[4] * m[10] - m[6] * m[8]) +
                  m[2] * (m[4] * m[9] - m[5] * m[8]);
        
        float invDet = 1.0f / det;
        for (int i = 0; i < 16; ++i) {
            inv[i] *= invDet;
        }
        
        return inv;
    }
    
    void invert() {
        *this = inverted();
    }
    
    static Matrix4x4 translation(const Vector3f& t) {
        Matrix4x4 result;
        result.identity();
        result(0, 3) = t.x;
        result(1, 3) = t.y;
        result(2, 3) = t.z;
        return result;
    }
    
    static Matrix4x4 translation(float x, float y, float z) {
        return translation(Vector3f(x, y, z));
    }
    
    static Matrix4x4 scale(const Vector3f& s) {
        Matrix4x4 result;
        result.identity();
        result(0, 0) = s.x;
        result(1, 1) = s.y;
        result(2, 2) = s.z;
        return result;
    }
    
    static Matrix4x4 scale(float x, float y, float z) {
        return scale(Vector3f(x, y, z));
    }
    
    static Matrix4x4 scale(float s) {
        return scale(Vector3f(s, s, s));
    }
    
    static Matrix4x4 rotationX(float angleRadians) {
        Matrix4x4 result;
        result.identity();
        float c = std::cos(angleRadians);
        float s = std::sin(angleRadians);
        result(1, 1) = c;
        result(1, 2) = -s;
        result(2, 1) = s;
        result(2, 2) = c;
        return result;
    }
    
    static Matrix4x4 rotationY(float angleRadians) {
        Matrix4x4 result;
        result.identity();
        float c = std::cos(angleRadians);
        float s = std::sin(angleRadians);
        result(0, 0) = c;
        result(0, 2) = s;
        result(2, 0) = -s;
        result(2, 2) = c;
        return result;
    }
    
    static Matrix4x4 rotationZ(float angleRadians) {
        Matrix4x4 result;
        result.identity();
        float c = std::cos(angleRadians);
        float s = std::sin(angleRadians);
        result(0, 0) = c;
        result(0, 1) = -s;
        result(1, 0) = s;
        result(1, 1) = c;
        return result;
    }
    
    static Matrix4x4 rotation(const Vector3f& axis, float angleRadians) {
        Matrix4x4 result;
        result.identity();
        
        Vector3f n = axis.normalized();
        float c = std::cos(angleRadians);
        float s = std::sin(angleRadians);
        float oneMinusC = 1.0f - c;
        
        result(0, 0) = c + n.x * n.x * oneMinusC;
        result(0, 1) = n.x * n.y * oneMinusC - n.z * s;
        result(0, 2) = n.x * n.z * oneMinusC + n.y * s;
        
        result(1, 0) = n.y * n.x * oneMinusC + n.z * s;
        result(1, 1) = c + n.y * n.y * oneMinusC;
        result(1, 2) = n.y * n.z * oneMinusC - n.x * s;
        
        result(2, 0) = n.z * n.x * oneMinusC - n.y * s;
        result(2, 1) = n.z * n.y * oneMinusC + n.x * s;
        result(2, 2) = c + n.z * n.z * oneMinusC;
        
        return result;
    }
    
    static Matrix4x4 lookAt(const Vector3f& eye, const Vector3f& target, const Vector3f& up) {
        Vector3f zAxis = (eye - target).normalized();
        Vector3f xAxis = up.cross(zAxis).normalized();
        Vector3f yAxis = zAxis.cross(xAxis);
        
        Matrix4x4 result;
        result(0, 0) = xAxis.x; result(0, 1) = xAxis.y; result(0, 2) = xAxis.z; result(0, 3) = -xAxis.dot(eye);
        result(1, 0) = yAxis.x; result(1, 1) = yAxis.y; result(1, 2) = yAxis.z; result(1, 3) = -yAxis.dot(eye);
        result(2, 0) = zAxis.x; result(2, 1) = zAxis.y; result(2, 2) = zAxis.z; result(2, 3) = -zAxis.dot(eye);
        result(3, 0) = 0;       result(3, 1) = 0;       result(3, 2) = 0;       result(3, 3) = 1;
        
        return result;
    }
    
    static Matrix4x4 perspective(float fovY, float aspect, float nearPlane, float farPlane) {
        Matrix4x4 result;
        float tanHalfFovy = std::tan(fovY * 0.5f);
        
        result(0, 0) = 1.0f / (aspect * tanHalfFovy);
        result(1, 1) = 1.0f / tanHalfFovy;
        result(2, 2) = -(farPlane + nearPlane) / (farPlane - nearPlane);
        result(2, 3) = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);
        result(3, 2) = -1.0f;
        result(3, 3) = 0.0f;
        
        return result;
    }
    
    static Matrix4x4 orthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
        Matrix4x4 result;
        result.identity();
        
        result(0, 0) = 2.0f / (right - left);
        result(1, 1) = 2.0f / (top - bottom);
        result(2, 2) = -2.0f / (farPlane - nearPlane);
        result(0, 3) = -(right + left) / (right - left);
        result(1, 3) = -(top + bottom) / (top - bottom);
        result(2, 3) = -(farPlane + nearPlane) / (farPlane - nearPlane);
        
        return result;
    }
    
    static Matrix4x4 createIdentity() {
        Matrix4x4 result;
        result.identity();
        return result;
    }
};

} // namespace Math
} // namespace SFSim