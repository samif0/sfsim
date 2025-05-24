#include <iostream>
#include <cassert>
#include <cmath>
#include "math/vector.hpp"
#include "math/matrix.hpp"

using namespace SFSim::Math;

const float EPSILON = 1e-6f;

bool floatEqual(float a, float b) {
    return std::abs(a - b) < EPSILON;
}

bool vector3fEqual(const Vector3f& a, const Vector3f& b) {
    return floatEqual(a.x, b.x) && floatEqual(a.y, b.y) && floatEqual(a.z, b.z);
}

void testVector3f() {
    std::cout << "Testing Vector3f..." << std::endl;
    
    Vector3f v1(1, 2, 3);
    Vector3f v2(4, 5, 6);
    
    Vector3f sum = v1 + v2;
    assert(vector3fEqual(sum, Vector3f(5, 7, 9)));
    
    Vector3f diff = v2 - v1;
    assert(vector3fEqual(diff, Vector3f(3, 3, 3)));
    
    Vector3f scaled = v1 * 2.0f;
    assert(vector3fEqual(scaled, Vector3f(2, 4, 6)));
    
    float dot = v1.dot(v2);
    assert(floatEqual(dot, 32.0f));
    
    Vector3f cross = v1.cross(v2);
    assert(vector3fEqual(cross, Vector3f(-3, 6, -3)));
    
    float length = Vector3f(3, 4, 0).length();
    assert(floatEqual(length, 5.0f));
    
    Vector3f normalized = Vector3f(3, 4, 0).normalized();
    assert(vector3fEqual(normalized, Vector3f(0.6f, 0.8f, 0.0f)));
    
    std::cout << "Vector3f tests passed!" << std::endl;
}

void testMatrix4x4() {
    std::cout << "Testing Matrix4x4..." << std::endl;
    
    Matrix4x4 identity = Matrix4x4::identity();
    Vector3f point(1, 2, 3);
    Vector3f transformed = identity.transformPoint(point);
    assert(vector3fEqual(transformed, point));
    
    Matrix4x4 translation = Matrix4x4::translation(5, 10, 15);
    Vector3f translated = translation.transformPoint(point);
    assert(vector3fEqual(translated, Vector3f(6, 12, 18)));
    
    Matrix4x4 scale = Matrix4x4::scale(2, 3, 4);
    Vector3f scaled = scale.transformPoint(point);
    assert(vector3fEqual(scaled, Vector3f(2, 6, 12)));
    
    Matrix4x4 rotX = Matrix4x4::rotationX(M_PI / 2);
    Vector3f rotated = rotX.transformPoint(Vector3f(1, 1, 0));
    assert(vector3fEqual(rotated, Vector3f(1, 0, 1)));
    
    Matrix4x4 combined = translation * scale;
    Vector3f combinedResult = combined.transformPoint(point);
    Vector3f expected = translation.transformPoint(scale.transformPoint(point));
    assert(vector3fEqual(combinedResult, expected));
    
    std::cout << "Matrix4x4 tests passed!" << std::endl;
}

void testProjection() {
    std::cout << "Testing projection matrices..." << std::endl;
    
    Matrix4x4 persp = Matrix4x4::perspective(M_PI / 4, 16.0f / 9.0f, 0.1f, 100.0f);
    
    Matrix4x4 ortho = Matrix4x4::orthographic(-10, 10, -10, 10, 0.1f, 100.0f);
    
    Vector3f eye(0, 0, 5);
    Vector3f target(0, 0, 0);
    Vector3f up(0, 1, 0);
    Matrix4x4 view = Matrix4x4::lookAt(eye, target, up);
    
    Vector3f worldPoint(1, 1, 0);
    Vector3f viewPoint = view.transformPoint(worldPoint);
    
    std::cout << "Projection matrix tests passed!" << std::endl;
}

int main() {
    std::cout << "Running math library tests..." << std::endl;
    
    testVector3f();
    testMatrix4x4();
    testProjection();
    
    std::cout << "All math tests passed!" << std::endl;
    return 0;
}