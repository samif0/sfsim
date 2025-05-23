# Getting Started with SFSim Refactoring

This guide provides concrete first steps to begin refactoring your simulation library, focusing on the math foundation which everything else will depend on.

## First Steps: Math Library (Week 1)

### Day 1-2: Setup Vector Classes

1. **Create the directory structure**
   ```bash
   mkdir -p include/math
   ```

2. **Create Vector3f class**

   Create `include/math/vector3.hpp`:
   ```cpp
   #ifndef MATH_VECTOR3_HPP
   #define MATH_VECTOR3_HPP

   #include <cmath>
   #include <iostream>

   namespace sfsim {
   namespace math {

   class Vector3f {
   public:
       float x, y, z;
       
       // Constructors
       Vector3f() : x(0.0f), y(0.0f), z(0.0f) {}
       Vector3f(float x, float y, float z) : x(x), y(y), z(z) {}
       
       // Copy from SFML vector (for transition)
       Vector3f(const sf::Vector3f& v) : x(v.x), y(v.y), z(v.z) {}
       
       // Convert to SFML vector (for transition)
       operator sf::Vector3f() const { return sf::Vector3f(x, y, z); }
       
       // Basic arithmetic
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
       
       // Vector operations
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
           return std::sqrt(x*x + y*y + z*z);
       }
       
       float lengthSquared() const {
           return x*x + y*y + z*z;
       }
       
       Vector3f normalized() const {
           float len = length();
           if (len > 0) {
               return *this / len;
           }
           return *this;
       }
       
       // Static utility functions
       static Vector3f lerp(const Vector3f& a, const Vector3f& b, float t) {
           return a + (b - a) * t;
       }
   };

   // Non-member operators
   inline Vector3f operator*(float scalar, const Vector3f& vec) {
       return vec * scalar;
   }

   inline std::ostream& operator<<(std::ostream& os, const Vector3f& vec) {
       os << "Vector3f(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
       return os;
   }

   } // namespace math
   } // namespace sfsim

   #endif // MATH_VECTOR3_HPP
   ```

3. **Create a simple test program**

   Create `src/tests/vector_test.cpp`:
   ```cpp
   #include <SFML/Graphics.hpp>
   #include <iostream>
   #include "math/vector3.hpp"

   using namespace sfsim::math;

   int main() {
       // Test constructors
       Vector3f v1(1.0f, 2.0f, 3.0f);
       Vector3f v2(3.0f, 2.0f, 1.0f);
       
       // Test basic operations
       Vector3f sum = v1 + v2;
       Vector3f diff = v1 - v2;
       Vector3f scaled = v1 * 2.0f;
       
       std::cout << "v1: " << v1 << std::endl;
       std::cout << "v2: " << v2 << std::endl;
       std::cout << "v1 + v2: " << sum << std::endl;
       std::cout << "v1 - v2: " << diff << std::endl;
       std::cout << "v1 * 2: " << scaled << std::endl;
       
       // Test vector operations
       float dot = v1.dot(v2);
       Vector3f cross = v1.cross(v2);
       float length = v1.length();
       Vector3f normalized = v1.normalized();
       
       std::cout << "v1 . v2: " << dot << std::endl;
       std::cout << "v1 × v2: " << cross << std::endl;
       std::cout << "Length of v1: " << length << std::endl;
       std::cout << "Normalized v1: " << normalized << std::endl;
       
       // Test conversion with SFML (for transition period)
       sf::Vector3f sfv = v1;
       Vector3f v3(sfv);
       
       std::cout << "SFML conversion test: " << v3 << std::endl;
       
       return 0;
   }
   ```

4. **Update your CMakeLists.txt to include the test**

   Add to your CMakeLists.txt:
   ```cmake
   # Add vector test executable
   add_executable(vector_test src/tests/vector_test.cpp)
   target_link_libraries(vector_test PRIVATE SFML::System)
   ```

### Day 3-4: Implement Matrix4x4 Class

1. **Create Matrix4x4 class**

   Create `include/math/matrix4.hpp`:
   ```cpp
   #ifndef MATH_MATRIX4_HPP
   #define MATH_MATRIX4_HPP

   #include <cmath>
   #include <array>
   #include "vector3.hpp"

   namespace sfsim {
   namespace math {

   class Matrix4x4 {
   public:
       // Store as a 2D array for easy indexing (row-major for readability)
       std::array<std::array<float, 4>, 4> m;
       
       // Constructor (identity matrix by default)
       Matrix4x4() {
           // Initialize as identity
           m = {{
               {1.0f, 0.0f, 0.0f, 0.0f},
               {0.0f, 1.0f, 0.0f, 0.0f},
               {0.0f, 0.0f, 1.0f, 0.0f},
               {0.0f, 0.0f, 0.0f, 1.0f}
           }};
       }
       
       // Constructor from existing mat4x4 (for transition)
       Matrix4x4(const mat4x4& other) {
           for (int i = 0; i < 4; i++) {
               for (int j = 0; j < 4; j++) {
                   m[i][j] = other.mat[i][j];
               }
           }
       }
       
       // Matrix multiplication
       Matrix4x4 operator*(const Matrix4x4& other) const {
           Matrix4x4 result;
           
           for (int i = 0; i < 4; i++) {
               for (int j = 0; j < 4; j++) {
                   result.m[i][j] = 0.0f;
                   for (int k = 0; k < 4; k++) {
                       result.m[i][j] += m[i][k] * other.m[k][j];
                   }
               }
           }
           
           return result;
       }
       
       // Apply to a point (assuming w=1)
       Vector3f transformPoint(const Vector3f& point) const {
           float x = point.x;
           float y = point.y;
           float z = point.z;
           float w = 1.0f;
           
           float xp = m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3] * w;
           float yp = m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3] * w;
           float zp = m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3] * w;
           float wp = m[3][0] * x + m[3][1] * y + m[3][2] * z + m[3][3] * w;
           
           // Perspective division
           if (wp != 0.0f) {
               return Vector3f(xp / wp, yp / wp, zp / wp);
           }
           
           return Vector3f(xp, yp, zp);
       }
       
       // Apply to a vector (assuming w=0, meaning no translation)
       Vector3f transformVector(const Vector3f& vector) const {
           float x = vector.x;
           float y = vector.y;
           float z = vector.z;
           
           float xp = m[0][0] * x + m[0][1] * y + m[0][2] * z;
           float yp = m[1][0] * x + m[1][1] * y + m[1][2] * z;
           float zp = m[2][0] * x + m[2][1] * y + m[2][2] * z;
           
           return Vector3f(xp, yp, zp);
       }
       
       // Static factory methods
       static Matrix4x4 createTranslation(float x, float y, float z) {
           Matrix4x4 result;
           
           result.m[0][3] = x;
           result.m[1][3] = y;
           result.m[2][3] = z;
           
           return result;
       }
       
       static Matrix4x4 createScale(float x, float y, float z) {
           Matrix4x4 result;
           
           result.m[0][0] = x;
           result.m[1][1] = y;
           result.m[2][2] = z;
           
           return result;
       }
       
       static Matrix4x4 createRotationX(float radians) {
           Matrix4x4 result;
           
           float cos = std::cos(radians);
           float sin = std::sin(radians);
           
           result.m[1][1] = cos;
           result.m[1][2] = -sin;
           result.m[2][1] = sin;
           result.m[2][2] = cos;
           
           return result;
       }
       
       static Matrix4x4 createRotationY(float radians) {
           Matrix4x4 result;
           
           float cos = std::cos(radians);
           float sin = std::sin(radians);
           
           result.m[0][0] = cos;
           result.m[0][2] = sin;
           result.m[2][0] = -sin;
           result.m[2][2] = cos;
           
           return result;
       }
       
       static Matrix4x4 createRotationZ(float radians) {
           Matrix4x4 result;
           
           float cos = std::cos(radians);
           float sin = std::sin(radians);
           
           result.m[0][0] = cos;
           result.m[0][1] = -sin;
           result.m[1][0] = sin;
           result.m[1][1] = cos;
           
           return result;
       }
       
       static Matrix4x4 createPerspective(float fov, float aspectRatio, float near, float far) {
           Matrix4x4 result;
           
           float tanHalfFov = std::tan(fov * 0.5f);
           float fovY = 1.0f / tanHalfFov;
           float fovX = fovY / aspectRatio;
           
           result.m[0][0] = fovX;
           result.m[1][1] = fovY;
           result.m[2][2] = far / (far - near);
           result.m[2][3] = (-far * near) / (far - near);
           result.m[3][2] = 1.0f;
           result.m[3][3] = 0.0f;
           
           return result;
       }
   };

   } // namespace math
   } // namespace sfsim

   #endif // MATH_MATRIX4_HPP
   ```

2. **Create a matrix test program**

   Create `src/tests/matrix_test.cpp`:
   ```cpp
   #include <SFML/Graphics.hpp>
   #include <iostream>
   #include "math/vector3.hpp"
   #include "math/matrix4.hpp"

   using namespace sfsim::math;

   int main() {
       // Test matrix creation
       Matrix4x4 identity; // Default constructor creates identity
       Matrix4x4 translation = Matrix4x4::createTranslation(1.0f, 2.0f, 3.0f);
       Matrix4x4 scale = Matrix4x4::createScale(2.0f, 2.0f, 2.0f);
       Matrix4x4 rotationX = Matrix4x4::createRotationX(M_PI / 4.0f); // 45 degrees
       
       // Test point transformation
       Vector3f point(1.0f, 0.0f, 0.0f);
       
       Vector3f translatedPoint = translation.transformPoint(point);
       Vector3f scaledPoint = scale.transformPoint(point);
       Vector3f rotatedPoint = rotationX.transformPoint(point);
       
       std::cout << "Original point: " << point << std::endl;
       std::cout << "Translated point: " << translatedPoint << std::endl;
       std::cout << "Scaled point: " << scaledPoint << std::endl;
       std::cout << "Rotated point: " << rotatedPoint << std::endl;
       
       // Test matrix concatenation
       Matrix4x4 combined = translation * rotationX * scale;
       Vector3f transformedPoint = combined.transformPoint(point);
       
       std::cout << "Combined transform: " << transformedPoint << std::endl;
       
       // Test perspective matrix
       Matrix4x4 perspective = Matrix4x4::createPerspective(M_PI / 4.0f, 4.0f/3.0f, 0.1f, 100.0f);
       Vector3f farPoint(0.0f, 0.0f, -10.0f);
       Vector3f projectedPoint = perspective.transformPoint(farPoint);
       
       std::cout << "Projected point: " << projectedPoint << std::endl;
       
       return 0;
   }
   ```

3. **Update your CMakeLists.txt to include the test**

   Add to your CMakeLists.txt:
   ```cmake
   # Add matrix test executable
   add_executable(matrix_test src/tests/matrix_test.cpp)
   target_link_libraries(matrix_test PRIVATE SFML::System)
   ```

### Day 5-7: Start Integrating With Existing Code

1. **Modify the sim.hpp file**

   Update the `proj` method in sim.hpp to use your Matrix4x4 class:
   ```cpp
   // Instead of:
   void proj(sf::Vector3f& in, sf::Vector3f& out, mat4x4& pm);
   
   // Change to:
   void proj(sf::Vector3f& in, sf::Vector3f& out, sfsim::math::Matrix4x4& pm);
   ```

2. **Update the sim.cpp file**

   Update the implementation to use your math library:
   ```cpp
   void sim::proj(sf::Vector3f& in, sf::Vector3f& out, sfsim::math::Matrix4x4& pm) {
       // Use the new transform method
       sfsim::math::Vector3f v_in(in.x, in.y, in.z);
       sfsim::math::Vector3f v_out = pm.transformPoint(v_in);
       
       // Convert back to sf::Vector3f
       out.x = v_out.x;
       out.y = v_out.y;
       out.z = v_out.z;
   }
   ```

3. **Gradually update the sim_config struct**

   Transition to use your Matrix4x4 class:
   ```cpp
   struct sim_config {
       sf::String title;
       unsigned int win_width;
       unsigned int win_height;
       float fNear = 0.1f;
       float fFar = 1000.0f;
       float fFovj = 90.0f;
       float fAspectRatio = float(win_height) / float(win_width);
       
       // Create a perspective matrix using your new class
       sfsim::math::Matrix4x4 proj_mat = sfsim::math::Matrix4x4::createPerspective(
           fFovj * M_PI / 180.0f,  // Convert to radians
           fAspectRatio,
           fNear,
           fFar
       );
   };
   ```

This starting point establishes the foundation for your math library and begins the integration process. Take it step-by-step, testing each component thoroughly before moving on to the next phase in the refactoring roadmap.
