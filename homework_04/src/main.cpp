#include <sys/types.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <iomanip>

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

const int ticks_per_revolution = 1024; 
const float wheel_radius_m = 0.3f;
const float wheelbase_m = 1.0f;

struct Coord {
  float x{0.0f};  // X coordinate
  float y{0.0f};  // Y coordinate

  // Relod the addition operator to add two coordinates together
  Coord operator+(const Coord& other) const { return {x + other.x, y + other.y}; }

  // Relod the subtraction operator to subtract one coordinate from another
  Coord operator-(const Coord& other) const { return {x - other.x, y - other.y}; }

  // Relod the multiplication operator to scale a coordinate by a factor
  Coord operator*(float factor) const { return {x * factor, y * factor}; }

  // Relod the division operator to scale a coordinate by the inverse of a
  // factor
  Coord operator/(float factor) const { return {x / factor, y / factor}; }

  // Calculate the distance from this coordinate to another coordinate
  float distanceTo(const Coord& other) const { return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2)); }

  // Calculate the angle from this coordinate to another coordinate in radians
  float angleTo(const Coord& other) const { return std::atan2(other.y - y, other.x - x); }

  Coord moveTo(const float direction, const float distance) const
  {
    return {static_cast<float>(x + std::cos(direction) * distance), static_cast<float>(y + std::sin(direction) * distance)};
  }
};

std::ostream& operator<<(std::ostream& s, const Coord& c)
{
  std::ios_base::fmtflags old_flags = s.flags();
  std::streamsize old_prec = s.precision();
  s << std::fixed << std::setprecision(6);
  s << "(" << c.x << ", " << c.y << ")";
  s.precision(old_prec);
  s.flags(old_flags);
  return s;
}

struct Odometry {
  long timestamp_ms;
  long fl_ticks, fr_ticks;
  long bl_ticks, br_ticks;
};

std::ostream& operator<<(std::ostream& s, const Odometry& o)
{
  std::ios_base::fmtflags old_flags = s.flags();
  std::streamsize old_prec = s.precision();
  s << std::fixed << std::setprecision(6);
  s << "timestamp: " << o.timestamp_ms << ", ";
  s << "front ticks: (" << o.fl_ticks << ", " << o.fr_ticks << ") ";
  s << "back ticks: (" << o.bl_ticks << ", " << o.br_ticks << ")";
  s.precision(old_prec);
  s.flags(old_flags);
  return s;
}

std::istream& operator>>(std::istream& s, Odometry& o)
{
  s >> o.timestamp_ms >> o.fl_ticks >> o.fr_ticks >> o.bl_ticks >> o.br_ticks;
  return s;
}

int loadData(char* filename, Odometry*& o)
{
  std::ifstream in(filename);
  if (!in) {
    LOG("Error opening file " << filename);
    return -1;
  }

  u_int cnt = 0;
  Odometry dummy;

  while (in >> dummy)
    cnt++;

  in.clear();
  in.seekg(0);

  o = new Odometry[cnt];
  for (u_int i = 0; i < cnt; i++) {
    in >> o[i];
  }
  return cnt;
}

int main(int argc, char** argv)
{
  // The program expects exactly one argument: a path to telemetry samples.
  if (argc != 2) {
    std::cerr << "usage: ugv_odometry <input_path>\n";
    return 1;
  }

  Odometry* o = nullptr;

  u_int cnt = loadData(argv[1], o);
  if (cnt < 0 || o == nullptr) {
    LOG("Odometry data not loaded");
    return 1;
  }

  for (u_int i = 0; i < cnt; i++) {
    LOG(o[i]);
  }

  delete[] o;
  return 0;
}
