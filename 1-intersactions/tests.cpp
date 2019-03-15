#include <vector>
#include <memory>

#include "catch.hpp"
#include "geometry.hpp"

using namespace geometry;
using namespace std;


TEST_CASE("Vector2D Tests:\na = {2, 0}\nb = {0, 1}\nc = {3, 7}\nz = {0; 0}\n") {

  Vector2D a(2, 0), b(0, 1), c(-2, 2), z(0, 0);

  SECTION("CHECK: a != b") {
    REQUIRE(a != b);
  }
  SECTION("CHECK: z == a-a") {
    REQUIRE(z == a-a);
  }
  SECTION("CHECK: (a+b).y == 1.0") {
    REQUIRE((a+b).y == 1.);
  }
  SECTION("CHECK: (a-b).y == -1.0") {
    REQUIRE((a-b).y == -1.);
  }
  SECTION("CHECK: a * 2.35 == 2.35 * a") {
    REQUIRE(a * 2.35 == 2.35 * a);
  }
  SECTION("CHECK: len n(a) == 1.0") {
    REQUIRE(a.Normalized().Length() == 1.);
  }
  SECTION("CHECK: dot a,b == 0.0") {
    REQUIRE(a.Dot(b) == 0.);
  }
  SECTION("CHECK: dot a,a+b == 4.0") {
    REQUIRE(a.Dot(a+b) == 4.);
  }
  SECTION("CHECK: cross a,b == 2.0") {
    REQUIRE(a.Cross(b) == 2.);
  }
  SECTION("CHECK: cross a,a == 0.0") {
    REQUIRE(a.Cross(a) == 0.);
  }
  SECTION("CHECK: cross a,-a == 0.0") {
    REQUIRE(a.Cross(-a) == 0.);
  }
  SECTION("CHECK: n(reflect a,n(a)-n(b)) == n(b)") {
    REQUIRE( (a.Reflect(a.Normalized() - b.Normalized())).Normalized() == b.Normalized() );
  }
  SECTION("CHECK: c.normal == {-1/sqrt(2); -1/sqrt(2)}") {
    REQUIRE( c.Normal() == Vector2D(-1 / sqrt(2), -1 / sqrt(2)) );
  }
  SECTION("CHECK: b is in segment [a; c]") {
    REQUIRE( b.IsInSegment(a, c) == true );
  }
  SECTION("CHECK: z is NOT in segment [a; c]") {
    REQUIRE( z.IsInSegment(a, c) == true );
  }
  SECTION("CHECK: n(a)") {
    a.Normalize();
    REQUIRE( a == Vector2D(1, 0) );
  }

}

TEST_CASE("Different figures in one vector") {

  Vector2D a(0, 2);
  Vector2D b(4, 6);
  Vector2D c(4, -9);

  Points2D points(3);
  points[0] = Vector2D(5, 3);
  points[1] = Vector2D(-6, 8);
  points[2] = Vector2D(11, -7);

  vector< shared_ptr<Figure> > figures(3);
  figures[0] = make_shared<Line>(a, b);
  figures[1] = make_shared<Polyline>(points);
  figures[2] = make_shared<Circle>(c, 8.0);

  for (auto i = 0; i < 3; ++i) {
    for (auto j = 0; j < 3; ++j) {
      figures[i]->Intersect(*figures[j]);
    }
  }

}

TEST_CASE("Line ClosestPoint") {
    Vector2D point(0, 0);
    Line line(Vector2D(4, 0), Vector2D(0, 4));

    Vector2D expected_point(2.0, 2.0);

    Vector2D pnt = line.ClosestPoint(point);
    REQUIRE(pnt == expected_point);
}

TEST_CASE("Intersect Line with Line") {

  SECTION("Basic intersection") {
    Line l1(Vector2D(0, 0), Vector2D(4, 4));
    Line l2(Vector2D(2, 3), Vector2D(2, 4));

    Vector2D expected_point(2.0, 2.0);
    Points2D result = l1.Intersect(l2);

    REQUIRE(result.empty() == false);
    REQUIRE(result[0] == expected_point);
  }

  SECTION("No intersection") {
    Line l1(Vector2D(0, 0), Vector2D(4, 4));
    Line l2(Vector2D(0, 2), Vector2D(2, 4));

    REQUIRE(l1.Intersect(l2).empty() == true);
  }

  SECTION("Same lines") {
    Line l1(Vector2D(0, 2), Vector2D(2, 4));
    Line l2(Vector2D(0, 2), Vector2D(2, 4));

    REQUIRE(l1.Intersect(l2).empty() == true);
  }

}

TEST_CASE("Intersect Line with Polyline") {

  Points2D points(4);
  points[0] = Vector2D(0.0, 0.0);
  points[1] = Vector2D(4.0, 4.0);
  points[2] = Vector2D(2.0, 0.0);
  points[3] = Vector2D(2.0, 4.0);
  Polyline polyline(points);

  Vector2D a(0.0, 3.0);
  Vector2D b(4.0, 3.0);
  Line line(a, b);

  Points2D expected_points(3);
  expected_points[0] = Vector2D(3.0, 3.0);
  expected_points[1] = Vector2D(3.5, 3.0);
  expected_points[2] = Vector2D(2.0, 3.0);

  SECTION("Line.Intersect") {
    Points2D result = line.Intersect(polyline);
    REQUIRE(result.size() == expected_points.size());
    for (auto i = 0; i < result.size(); ++i) {
      REQUIRE(result[i] == expected_points[i]);
    }
  }

  SECTION("Polyline.Intersect") {
    Points2D result = polyline.Intersect(line);
    REQUIRE(result.size() == expected_points.size());
    for (auto i = 0; i < result.size(); ++i) {
      REQUIRE(result[i] == expected_points[i]);
    }
  }

}

TEST_CASE("Intersect Polyline with Polyline") {

  SECTION("Basic intersection") {
    Points2D points(3);
    points[0] = Vector2D(6.0, 4.0);
    points[1] = Vector2D(0.0, 1.0);
    points[2] = Vector2D(5.0, 1.0);
    Polyline pl1(points);

    points[0] = Vector2D(1.0, 3.0);
    points[1] = Vector2D(4.0, 0.0);
    points[2] = Vector2D(4.0, 4.0);
    Polyline pl2(points);

    Points2D expected_points(4);
    expected_points[0] = Vector2D(2.0, 2.0);
    expected_points[1] = Vector2D(4.0, 3.0);
    expected_points[2] = Vector2D(3.0, 1.0);
    expected_points[3] = Vector2D(4.0, 1.0);

    Points2D result = pl1.Intersect(pl2);
    REQUIRE(result.size() == expected_points.size());
    for (auto i = 0; i < result.size(); ++i) {
      REQUIRE(result[i] == expected_points[i]);
    }
  }

  SECTION("Same point segments") {
    Polyline pl1({Vector2D(0.0, 0.0), Vector2D(4.0, 4.0)});
    Polyline pl2({Vector2D(0.0, 0.0), Vector2D(0.0, 2.0)});

    Vector2D expected_point(0.0, 0.0);

    Points2D result = pl1.Intersect(pl2);
    REQUIRE(result[0] == expected_point);
  }

  SECTION("Not intersecting close segments") {
    Polyline pl1({Vector2D(0.0, 0.0), Vector2D(4.0, 4.0)});
    Polyline pl2({Vector2D(2.0, 0.0), Vector2D(2.0, 1.999999999)});

    Points2D result = pl1.Intersect(pl2);
    REQUIRE(result.empty() == true);
  }

}

TEST_CASE("Intersect Line with Circle") {

  Circle circle(2.0, 2.0, 2.0);

  SECTION("Basic intersection") {
    Line line(
      Vector2D(1.0, 5.0),
      Vector2D(5.0, 1.0)
    );

    Points2D expected_points(2);
    expected_points[0] = Vector2D(2.0, 4.0);
    expected_points[1] = Vector2D(4.0, 2.0);

    Points2D result = line.Intersect(circle);
    REQUIRE(result.size() == expected_points.size());
    for (auto i = 0; i < result.size(); ++i) {
      REQUIRE(result[i] == expected_points[i]);
    }
  }

  SECTION("No intersection") {
    Line line(
      Vector2D(5.0, 5.0),
      Vector2D(5.0, 1.0)
    );

    Points2D result = circle.Intersect(line);
    REQUIRE(result.empty() == true);
  }

  SECTION("In one point") {
    Line line(
      Vector2D(4.0, 5.0),
      Vector2D(4.0, 1.0)
    );

    Vector2D expected_point(4.0, 2.0);

    Points2D result = line.Intersect(circle);
    REQUIRE(result.size() == 1);
    REQUIRE(expected_point == result[0]);
  }

}

TEST_CASE("Intersect Polyline with Circle") {

  Points2D points(4);
  points[0] = Vector2D(2.0, 5.0);
  points[1] = Vector2D(2.0, -1.0);
  points[2] = Vector2D(4.0, -1.0);
  points[3] = Vector2D(4.0, 5.0);
  Polyline polyline(points);

  SECTION("Basic intersection") {
    Circle circle(Vector2D(2.0, 2.0), 2.0);

    Points2D expected_points(3);
    expected_points[0] = Vector2D(2.0, 4.0);
    expected_points[1] = Vector2D(2.0, 0.0);
    expected_points[2] = Vector2D(4.0, 2.0);

    Points2D result = circle.Intersect(polyline);
    REQUIRE(result.size() == expected_points.size());
    for (auto i = 0; i < result.size(); ++i) {
      REQUIRE(expected_points[i] == result[i]);
    }
  }

  SECTION("No intersection inside") {
    Circle circle(Vector2D(3.0, 2.0), 10.0);

    Points2D result = circle.Intersect(polyline);
    REQUIRE(result.empty() == true);
  }

  SECTION("No intersection outside") {
    Circle circle(Vector2D(3.0, 2.0), 0.5);

    Points2D result = circle.Intersect(polyline);
    REQUIRE(result.empty() == true);
  }

}

TEST_CASE("Intersect Circle with Circle") {

  SECTION("Intersect in two points") {
    Circle first_circle(Vector2D(0.0, 0.0), 2.0);
    Circle second_circle(Vector2D(2.0, 2.0), 2.0);

    Points2D expected_points(2);
    expected_points[0] = Vector2D(0.0, 2.0);
    expected_points[1] = Vector2D(2.0, 0.0);

    Points2D result = first_circle.Intersect(second_circle);
    REQUIRE(result.size() == expected_points.size());
    for (auto i = 0; i < result.size(); ++i) {
      REQUIRE(result[i] == expected_points[i]);
    }
  }

  SECTION("Intersect in one point") {
    Circle first_circle(Vector2D(0.0, 0.0), 3.0);
    Circle second_circle(Vector2D(0.0, 6.0), 3.0);

    Vector2D expected_point(0.0, 3.0);

    Points2D result = first_circle.Intersect(second_circle);
    REQUIRE(result.size() == 1);
    REQUIRE(result[0] == expected_point);
  }

  SECTION("No intersection") {
    Circle first_circle(Vector2D(0.0, 0.0), 3.0);
    Circle second_circle(Vector2D(5.0, 0.0), 1.0);

    Points2D result = first_circle.Intersect(second_circle);
    REQUIRE(result.empty() == true);
  }

  SECTION("Same circles") {
    Circle first_circle(Vector2D(3.0, 3.0), 3.0);
    Circle second_circle(Vector2D(3.0, 3.0), 3.0);

    Points2D result = first_circle.Intersect(second_circle);
    REQUIRE(result.empty() == true);
  }

  SECTION("Nested circles") {
    Circle first_circle(Vector2D(3.0, 3.0), 3.0);
    Circle second_circle(Vector2D(3.0, 3.0), 1.0);

    Points2D result = first_circle.Intersect(second_circle);
    REQUIRE(result.empty() == true);
  }

}