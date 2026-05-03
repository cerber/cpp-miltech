#include <iostream>

#define ENABLE_LOG 1
#define ENABLE_DEBUG 1

#if ENABLE_LOG
#define LOG(msg) std::cout << "[LOG] " << msg << std::endl
#else
#define LOG(msg)
#endif

#if ENABLE_DEBUG
#define DEBUG(msg) std::cout << "[DEBUG] " << msg << std::endl
#else
#define DEBUG(msg)
#endif

struct Coord {
  float x{0.0f};  // X coordinate
  float y{0.0f};  // Y coordinate

  // Relod the addition operator to add two coordinates together
  Coord operator+(const Coord& other) const {
    return {x + other.x, y + other.y};
  }

  // Relod the subtraction operator to subtract one coordinate from another
  Coord operator-(const Coord& other) const {
    return {x - other.x, y - other.y};
  }

  // Relod the multiplication operator to scale a coordinate by a factor
  Coord operator*(float factor) const { return {x * factor, y * factor}; }

  // Relod the division operator to scale a coordinate by the inverse of a
  // factor
  Coord operator/(float factor) const { return {x / factor, y / factor}; }

  // Calculate the distance from this coordinate to another coordinate
  float distanceTo(const Coord& other) const {
    return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2));
  }

  // Calculate the angle from this coordinate to another coordinate in radians
  float angleTo(const Coord& other) const {
    return std::atan2(other.y - y, other.x - x);
  }

  Coord moveTo(const float direction, const float distance) const {
    return {x + cos(direction) * distance, y + sin(direction) * distance};
  }

  Coord move(const float direction, const float speed, const float time) const {
    return {x + speed * cos(direction) * time, y + speed * sin(direction) * time};
  }

  // Calculate the drop point to meet the ballistic path
  Coord dropPoint(const Coord& other, float path) const {
    float distance = other.distanceTo({x, y});
    float ratio = (distance - path) / distance;
    return {x + (other.x - x) * ratio, y + (other.y - y) * ratio};
  }

  // Normalize the coordinate to a unit vector
  Coord normalize() const {
    float length = std::sqrt(x * x + y * y);
    if (length > 0) {
      return {x / length, y / length};
    } else {
      return {0.0f, 0.0f};  // Return a zero vector if the length is zero to
                            // avoid division by zero
    }
  }

  // Rotate the coordinate by a given angle in radians
  Coord rotate(float angle) const {
    float cosAngle = std::cos(angle);
    float sinAngle = std::sin(angle);
    return {x * cosAngle - y * sinAngle, x * sinAngle + y * cosAngle};
  }

  // Calculate the dot product of this coordinate with another coordinate
  float dot(const Coord& other) const { return x * other.x + y * other.y; }
};

std::ostream& operator<<(std::ostream& s, const Coord& c) {
  std::ios_base::fmtflags old_flags = s.flags();
  std::streamsize old_prec = s.precision();
  s << std::fixed << std::setprecision(6);
  s << "(" << c.x << ", " << c.y << ")";
  s.precision(old_prec);
  s.flags(old_flags);
  return s;
}

int main(int argc, char** argv) {
    // The program expects exactly one argument: a path to telemetry samples.
    if (argc != 2) {
        std::cerr << "usage: ugv_odometry <input_path>\n";
        return 1;
    }

    // TODO: implement wheel odometry for a 4-wheel differential-drive UGV.
    //
    // Model parameters:
    //   ticks_per_revolution = 1024
    //   wheel_radius_m       = 0.3
    //   wheelbase_m          = 1.0
    //
    // Input: a text file with 5 whitespace-separated values per line:
    //         timestamp_ms fl_ticks fr_ticks bl_ticks br_ticks
    // Output: a table on stdout, starting from the second sample:
    //         timestamp_ms x y theta

    return 0;
}
