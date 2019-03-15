#pragma once

#include <vector>
// #include <memory>

namespace geometry {

const double PI = 3.14159265358979323846;
const double EPS = 1e-8;

class Vector2D;
class Line;
class Polyline;
class Circle;

typedef std::vector<Vector2D> Points2D;
typedef long double value_t;

class Vector2D
{
friend inline Vector2D operator*(value_t lhs, const Vector2D& rhs) { return Vector2D(rhs) *= lhs; }

public:
  value_t x;
  value_t y;
  Vector2D() : x(0.), y(0.) {};
  Vector2D(value_t x, value_t y) : x(x), y(y) {};
  virtual ~Vector2D() = default;

  value_t Dot(const Vector2D&) const;
  value_t Cross(const Vector2D&) const;
  value_t Length() const;
  bool IsInSegment(const Vector2D&, const Vector2D&) const;
  void Normalize();
  Vector2D Normalized() const;
  Vector2D Normal() const;
  Vector2D Reflect(const Vector2D&) const;

  Vector2D operator*(value_t);
  Vector2D operator/(value_t);
  Vector2D operator-();
  Vector2D operator-(const Vector2D&) const;
  Vector2D operator+(const Vector2D&) const;
  
  Vector2D& operator*=(value_t);
  Vector2D& operator/=(value_t);
  Vector2D& operator-=(const Vector2D&);
  Vector2D& operator+=(const Vector2D&);

  bool operator==(const Vector2D&) const;
  bool operator!=(const Vector2D&) const;
};

class Figure
{
public:
  virtual ~Figure() = default;

  virtual Points2D Intersect(const Figure&) const = 0;
  virtual Points2D Intersect(const Line&) const;
  virtual Points2D Intersect(const Polyline&) const;
  virtual Points2D Intersect(const Circle&) const;
};

class Line: public Figure
{
public:
  value_t a;
  value_t b;
  value_t c;
  Line(value_t a = 0., value_t b = 0., value_t c = 0.);
  Line(Vector2D u, Vector2D v);

  Points2D Intersect(const Figure&) const override;
  Points2D Intersect(const Line&) const override;
  Vector2D Normal() const;
  Vector2D ClosestPoint(const Vector2D&) const;
};

class Circle: public Figure
{
public:
  Circle(const Vector2D& c, value_t r);
  Circle(value_t x = 0., value_t y = 0., value_t r = 0.);

  Points2D Intersect(const Figure&) const override;
  Points2D Intersect(const Line&) const override;
  Points2D Intersect(const Circle&) const override;

private:
  Vector2D center;
  value_t radius;
};

class Polyline: public Figure
{
public:
  Polyline(const Points2D& v);

  Points2D Intersect(const Figure&) const override;
  Points2D Intersect(const Line&) const override;
  Points2D Intersect(const Circle&) const override;
  Points2D Intersect(const Polyline&) const override;

private:
  Points2D vertices;
};

static inline value_t sqr(value_t x) { return x * x; }
// static inline double getRange(const Vector2D&, const Vector2D&);

} // namespace geometry
