#pragma once

#include <cmath>

namespace SFSim {
namespace Math {

class Vector2f {
public:
    float x, y;
    
    Vector2f() : x(0), y(0) {}
    Vector2f(float x, float y) : x(x), y(y) {}
    
    Vector2f operator+(const Vector2f& other) const {
        return Vector2f(x + other.x, y + other.y);
    }
    
    Vector2f operator-(const Vector2f& other) const {
        return Vector2f(x - other.x, y - other.y);
    }
    
    Vector2f operator*(float scalar) const {
        return Vector2f(x * scalar, y * scalar);
    }
    
    Vector2f operator/(float scalar) const {
        return Vector2f(x / scalar, y / scalar);
    }
    
    Vector2f& operator+=(const Vector2f& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    
    Vector2f& operator-=(const Vector2f& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    
    Vector2f& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    
    Vector2f& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }
    
    float dot(const Vector2f& other) const {
        return x * other.x + y * other.y;
    }
    
    float length() const {
        return std::sqrt(x * x + y * y);
    }
    
    float lengthSquared() const {
        return x * x + y * y;
    }
    
    Vector2f normalized() const {
        float len = length();
        if (len == 0) return Vector2f(0, 0);
        return *this / len;
    }
    
    void normalize() {
        float len = length();
        if (len != 0) {
            x /= len;
            y /= len;
        }
    }
    
    static Vector2f lerp(const Vector2f& a, const Vector2f& b, float t) {
        return a + (b - a) * t;
    }
    
    static float distance(const Vector2f& a, const Vector2f& b) {
        return (b - a).length();
    }
};

class Vector3f {
public:
    float x, y, z;
    
    Vector3f() : x(0), y(0), z(0) {}
    Vector3f(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector3f(const Vector2f& v, float z) : x(v.x), y(v.y), z(z) {}
    
    Vector3f operator+(const Vector3f& other) const {
        return Vector3f(x + other.x, y + other.y, z + other.z);
    }
    
    Vector3f operator-(const Vector3f& other) const {
        return Vector3f(x - other.x, y - other.y, z - other.z);
    }
    
    Vector3f operator*(float scalar) const {
        return Vector3f(x * scalar, y * scalar, z * scalar);
    }
    
    Vector3f operator/(float scalar) const {
        return Vector3f(x / scalar, y / scalar, z / scalar);
    }
    
    Vector3f& operator+=(const Vector3f& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    
    Vector3f& operator-=(const Vector3f& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    
    Vector3f& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    
    Vector3f& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }
    
    float dot(const Vector3f& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    
    Vector3f cross(const Vector3f& other) const {
        return Vector3f(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
    
    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }
    
    float lengthSquared() const {
        return x * x + y * y + z * z;
    }
    
    Vector3f normalized() const {
        float len = length();
        if (len == 0) return Vector3f(0, 0, 0);
        return *this / len;
    }
    
    void normalize() {
        float len = length();
        if (len != 0) {
            x /= len;
            y /= len;
            z /= len;
        }
    }
    
    Vector2f xy() const {
        return Vector2f(x, y);
    }
    
    Vector2f xz() const {
        return Vector2f(x, z);
    }
    
    Vector2f yz() const {
        return Vector2f(y, z);
    }
    
    static Vector3f lerp(const Vector3f& a, const Vector3f& b, float t) {
        return a + (b - a) * t;
    }
    
    static float distance(const Vector3f& a, const Vector3f& b) {
        return (b - a).length();
    }
    
    static Vector3f zero() { return Vector3f(0, 0, 0); }
    static Vector3f one() { return Vector3f(1, 1, 1); }
    static Vector3f unitX() { return Vector3f(1, 0, 0); }
    static Vector3f unitY() { return Vector3f(0, 1, 0); }
    static Vector3f unitZ() { return Vector3f(0, 0, 1); }
    static Vector3f up() { return Vector3f(0, 1, 0); }
    static Vector3f right() { return Vector3f(1, 0, 0); }
    static Vector3f forward() { return Vector3f(0, 0, 1); }
};

class Vector4f {
public:
    float x, y, z, w;
    
    Vector4f() : x(0), y(0), z(0), w(0) {}
    Vector4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    Vector4f(const Vector3f& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}
    Vector4f(const Vector2f& v, float z, float w) : x(v.x), y(v.y), z(z), w(w) {}
    
    Vector4f operator+(const Vector4f& other) const {
        return Vector4f(x + other.x, y + other.y, z + other.z, w + other.w);
    }
    
    Vector4f operator-(const Vector4f& other) const {
        return Vector4f(x - other.x, y - other.y, z - other.z, w - other.w);
    }
    
    Vector4f operator*(float scalar) const {
        return Vector4f(x * scalar, y * scalar, z * scalar, w * scalar);
    }
    
    Vector4f operator/(float scalar) const {
        return Vector4f(x / scalar, y / scalar, z / scalar, w / scalar);
    }
    
    Vector4f& operator+=(const Vector4f& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }
    
    Vector4f& operator-=(const Vector4f& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }
    
    Vector4f& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }
    
    Vector4f& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }
    
    float dot(const Vector4f& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }
    
    float length() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }
    
    float lengthSquared() const {
        return x * x + y * y + z * z + w * w;
    }
    
    Vector4f normalized() const {
        float len = length();
        if (len == 0) return Vector4f(0, 0, 0, 0);
        return *this / len;
    }
    
    void normalize() {
        float len = length();
        if (len != 0) {
            x /= len;
            y /= len;
            z /= len;
            w /= len;
        }
    }
    
    Vector3f xyz() const {
        return Vector3f(x, y, z);
    }
    
    Vector2f xy() const {
        return Vector2f(x, y);
    }
    
    static Vector4f lerp(const Vector4f& a, const Vector4f& b, float t) {
        return a + (b - a) * t;
    }
    
    static float distance(const Vector4f& a, const Vector4f& b) {
        return (b - a).length();
    }
};

inline Vector2f operator*(float scalar, const Vector2f& v) {
    return v * scalar;
}

inline Vector3f operator*(float scalar, const Vector3f& v) {
    return v * scalar;
}

inline Vector4f operator*(float scalar, const Vector4f& v) {
    return v * scalar;
}

} // namespace Math
} // namespace SFSim