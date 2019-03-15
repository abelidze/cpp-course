#include <vector>
#include <cmath>
#include <iostream>

#include "geometry.hpp"

using namespace geometry;

/**
 * Vector2D Methods and operations
 */

value_t Vector2D::Dot(const Vector2D& rhs) const
{
  return x * rhs.x + y * rhs.y;
}

value_t Vector2D::Cross(const Vector2D& rhs) const
{
  return x * rhs.y - y * rhs.x;
}

value_t Vector2D::Length() const
{
  return sqrt(Dot(*this));
}

bool Vector2D::IsInSegment(const Vector2D& a, const Vector2D& b) const
{
  return ((a - *this).Dot(b - *this) <= 0.);
}

void Vector2D::Normalize()
{
  (*this) /= Length();
}

Vector2D Vector2D::Normalized() const
{
  return Vector2D(*this) /= Length();
}

Vector2D Vector2D::Normal() const
{
  return Vector2D(-y, x).Normalized();
}

Vector2D Vector2D::Reflect(const Vector2D& normal) const
{
  return Vector2D(*this) -= 2.0 * Dot(normal.Normalized()) * normal.Normalized();
}

Vector2D Vector2D::operator*(value_t scalar)
{
  return Vector2D(*this) *= scalar;
}

Vector2D Vector2D::operator/(value_t scalar)
{
  return Vector2D(*this) /= scalar;
}

Vector2D Vector2D::operator-()
{
  return Vector2D(*this) *= -1;
}

Vector2D Vector2D::operator-(const Vector2D& rhs) const
{
  return Vector2D(*this) -= rhs;
}

Vector2D Vector2D::operator+(const Vector2D& rhs) const
{
  return Vector2D(*this) += rhs;
}

Vector2D& Vector2D::operator*=(value_t scalar)
{
  x *= scalar;
  y *= scalar;
  return *this;
}

Vector2D& Vector2D::operator/=(value_t scalar)
{
  x /= scalar;
  y /= scalar;
  return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& rhs)
{
  x -= rhs.x;
  y -= rhs.y;
  return *this;
}

Vector2D& Vector2D::operator+=(const Vector2D& rhs)
{
  x += rhs.x;
  y += rhs.y;
  return *this;
}

bool Vector2D::operator==(const Vector2D& rhs) const
{
  return (fabs(x - rhs.x) < EPS) && (fabs(y - rhs.y) < EPS);
}

bool Vector2D::operator!=(const Vector2D& rhs) const
{
   return !operator==(rhs);
}

/**
 * Geometry functions
 */

// double geometry::getRange(const Vector2D& p1, const Vector2D& p2)
// {
//   return sqrt(sqr(p2.x - p1.x) + sqr(p2.y - p1.y));
// }

/**
 * Figures constructors
 */

Line::Line(value_t a, value_t b, value_t c) : a(a), b(b), c(c) {};
Line::Line(Vector2D u, Vector2D v)
{
  Vector2D norm = (v - u).Normal();
  a = norm.x;
  b = norm.y;
  c = -(a * u.x + b * u.y);
};
Polyline::Polyline(const Points2D& v) : vertices(v) {};
Circle::Circle(const Vector2D& c, value_t r) : center(c.x, c.y), radius(r) {};
Circle::Circle(value_t x, value_t y, value_t r) : center(x, y), radius(r) {};

/**
 * Figures methods
 */

Vector2D Line::Normal() const
{
  return Vector2D(a, b).Normalized();
}

Vector2D Line::ClosestPoint(const Vector2D& p) const
{
  value_t k = (a*p.x + b*p.y + c) / (sqr(a) + sqr(b));
  return Vector2D(p.x - a*k, p.y - b*k);
}

/**
 * Double dispatch
 */

Points2D Figure::Intersect(const Line& o) const { return Intersect((const Figure&) o); }
Points2D Figure::Intersect(const Circle& o) const { return Intersect((const Figure&) o); }
Points2D Figure::Intersect(const Polyline& o) const { return Intersect((const Figure&) o); }

Points2D Line::Intersect(const Figure& o) const { return o.Intersect(*this); }
Points2D Circle::Intersect(const Figure& o) const { return o.Intersect(*this); }
Points2D Polyline::Intersect(const Figure& o) const { return o.Intersect(*this); }

/**
 * Intersection functions
 */

Points2D Line::Intersect(const Line& other) const
{
  // std::cout << "Line with Line" << std::endl;
  value_t normalCross = Normal().Cross(other.Normal());
  if (fabs(normalCross) < EPS) {
    return {};
  }
  value_t x = Vector2D(b, c).Cross(Vector2D(other.b, other.c)) / normalCross;
  value_t y = -(fabs(b) > EPS ? (a * x + c) / b : (other.a * x + other.c) / other.b);
  return { Vector2D(x, y) };
}

Points2D Circle::Intersect(const Line& other) const
{
  // std::cout << "Circle with Line" << std::endl;
  Vector2D closestPoint = other.ClosestPoint(center);
  value_t dist = (closestPoint - center).Length();

  if (fabs(dist - radius) < EPS) {
    return {closestPoint};
  } else if (dist > radius) {
    return {};
  }

  value_t k = sqrt(sqr(radius) - sqr(dist));
  Vector2D dir = other.Normal().Normal();
  return {closestPoint + dir*k, closestPoint - dir*k};
}

Points2D Circle::Intersect(const Circle& o) const
{
  // std::cout << "Circle with Circle" << std::endl;
  Vector2D cVec = o.center - center;
  value_t radius_diff = fabs(radius - o.radius);

  if ( (center == o.center && radius_diff < EPS) || (radius_diff > cVec.Length()) ) {
    return {};
  }

  // value_t c = (center.Dot(center) - o.center.Dot(o.center) + sqr(o.radius) - sqr(radius)) / 2.;
  value_t c = center.Dot(center) - sqr(radius) - o.center.Dot(o.center) + sqr(o.radius);
  return Line(2.0 * cVec.x, 2.0 * cVec.y, c).Intersect(o);
}

Points2D Polyline::Intersect(const Line& other) const
{
  // std::cout << "PLine with Line" << std::endl;
  Points2D intersection = {};
  Points2D point = {};
  for (auto p1 = vertices.begin(), p2 = vertices.begin() + 1; p2 != vertices.end(); ++p2, ++p1) {
    point = other.Intersect(Line(*p1, *p2));
    if (point.empty() || !point[0].IsInSegment(*p1, *p2)) continue;
    intersection.push_back(point[0]);
  }
  return intersection;
}

Points2D Polyline::Intersect(const Circle& other) const
{
  // std::cout << "PLine with Circle" << std::endl;
  Points2D intersection = {};
  Points2D points = {};
  for (auto p1 = vertices.begin(), p2 = vertices.begin() + 1; p2 != vertices.end(); ++p2, ++p1) {
    points = other.Intersect(Line(*p1, *p2));
    if (points.empty() || !points[0].IsInSegment(*p1, *p2)) continue;
    if (points.size() > 1 && !points[1].IsInSegment(*p1, *p2)) continue;
    intersection.insert(intersection.end(), points.begin(), points.end());
  }
  return intersection;
}

Points2D Polyline::Intersect(const Polyline& other) const
{
  // std::cout << "PLine with PLine" << std::endl;
  Points2D intersection = {};
  Points2D newIntersection;
  for (auto p1 = vertices.begin(), p2 = vertices.begin() + 1; p2 != vertices.end(); ++p2, ++p1) {
    // printf("(%Lf; %Lf) and (%Lf; %Lf)\n", p1->x, p1->y, p2->x, p2->y);
    newIntersection = other.Intersect(Line(*p1, *p2));
    intersection.insert(intersection.end(), newIntersection.begin(), newIntersection.end());
  }
  return intersection;
}
