#ifndef MATH_VECTOR3f_HPP
#define MATH_VECTOR3f_HPP

#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

namespace sfsim {
namespace math {
class vector3f {
 private:
 public:
  float x, y, z;

  vector3f(float x, float y, float z) : x(x), y(y), z(z) {}
  vector3f(const sf::Vector3f& sv) : x(sv.x), y(sv.y), z(sv.z) {}
  operator sf::Vector3f() const { return sf::Vector3f(x, y, z); }

  vector3f operator+(const vector3f& other) const {
    return vector3f(x + other.x, y + other.y, z + other.z);
  }

  vector3f operator-(const vector3f& other) const {
    return vector3f(x - other.x, y - other.y, z - other.z);
  }

  vector3f operator*(float scalar) const { return vector3f(x * scalar, y * scalar, z * scalar); }

  vector3f operator/(float scalar) const { return vector3f(x / scalar, y / scalar, z / scalar); }

  float dot(const vector3f& other) const { return x * other.x + y * other.y + z * other.z; }

  vector3f cross(const vector3f& other) const {
    return vector3f(y * other.z - z * other.y, z * other.x - x * other.z,
                    x * other.y - y * other.x);
  }

  float len() const { return std::sqrt(x * x + y * y + z * z); }
  float lensq() const { return std::pow(this->len(), 2); }

  vector3f normalize() const {
    float l = this->len();
    if (l > 0) {
      return *this / l;
    }
    return *this;
  }

  static vector3f lerp(const vector3f& u, const vector3f v, float t) { return u + (v - u) * t; }
};

inline vector3f operator*(float scalar, const vector3f& u) { return u * scalar; }
inline std::ostream& operator<<(std::ostream& os, const vector3f& u) {
  os << "vector3f(" << u.x << ", " << u.y << ", " << u.z << ", " << ")";
  return os;
}
}  // namespace math
}  // namespace sfsim

#endif
