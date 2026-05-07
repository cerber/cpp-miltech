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
const float distance_per_tick = 2 * M_PI * wheel_radius_m / ticks_per_revolution;

struct Coord {
  float x{0.0f};  // X coordinate
  float y{0.0f};  // Y coordinate

  Coord moveTo(const float direction, const float distance) const
  {
    return {x + std::cos(direction) * distance, y + std::sin(direction) * distance};
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

struct Motion {
  float d;
  float dtheta;
};

struct Odometry {
  long timestamp_ms;
  long fl_ticks, fr_ticks;
  long bl_ticks, br_ticks;

  Motion moveFrom(const Odometry& prev)
  {
    long d_fl = fl_ticks - prev.fl_ticks;
    long d_fr = fr_ticks - prev.fr_ticks;
    long d_bl = bl_ticks - prev.bl_ticks;
    long d_br = br_ticks - prev.br_ticks;

    float d_l = distance_per_tick * (d_fl + d_bl) / 2;
    float d_r = distance_per_tick * (d_fr + d_br) / 2;

    return {(d_l + d_r) / 2, (d_r - d_l) / wheelbase_m};
  }
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

  Odometry current, previous;
  Coord c{0.0f, 0.0f};
  float direction = 0.0f;

  std::ifstream in(argv[1]);
  if (!in) {
    LOG("Error opening file " << argv[1]);
    return 1;
  }

  in >> previous;
  if (in.bad()) {
    LOG("Error reading first position");
    return 1;
  };

  while (in >> current) {
    std::cout << std::fixed << std::setprecision(4);
    Motion m = current.moveFrom(previous);
    c = c.moveTo((direction + m.dtheta) / 2, m.d);
    direction += m.dtheta;
    std::cout << current.timestamp_ms << " " << c.x << " " << c.y << " " << direction << std::endl;
    previous = current;
  }

  return 0;
}
