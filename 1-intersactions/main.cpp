#include <iostream>
#include <vector>
#include <memory>

#include "geometry.hpp"

using namespace geometry;
using namespace std;

int main(int argc, char const *argv[])
{
  Vector2D l(-1,0), r(1,0), u(0,1), d(0,-1);
  vector<Figure*> figures;
  // figures.push_back(new Line(l,r));
  // figures.push_back(new Line(u,d));
  // figures.push_back(new Circle(1., -0.5, 0.));
  // figures.push_back(new Circle(1., 0.5, 0.));
  Points2D dig = {Vector2D(-1,0), Vector2D(0,2), Vector2D(1,0)};
  Points2D hor = {Vector2D(-1,1), Vector2D(1,1)};
  figures.push_back(new Polyline(hor));
  figures.push_back(new Polyline(dig));

  Points2D intersetion = figures[0]->Intersect(*figures[1]);
  for (auto p = intersetion.begin(); p != intersetion.end(); ++p) {
    printf("(%Lf; %Lf)\n", p->x, p->y);
  }

  // Vector2D a(0, 2);
  // Vector2D b(4, 6);
  // Vector2D c(4, -9);

  // Points2D points(3);
  // points[0] = Vector2D(5, 3);
  // points[1] = Vector2D(-6, 8);
  // points[2] = Vector2D(11, -7);

  // vector< shared_ptr<Figure> > figures(3);
  // figures[0] = make_shared<Line>( a, b );
  // figures[1] = make_shared<Polyline>( points );
  // figures[2] = make_shared<Circle>( c, 8.0 );

  // for (auto i = 0; i < 3; ++i) {
  //   for (auto j = 0; j < 3; ++j) {
  //     figures[i]->Intersect(*figures[j]);
  //   }
  // }


  // Vector2D a(2, 0), b(0, 1), z(0, 0);
  // cout << boolalpha << "a != b ? " << (a != b) << endl;
  // cout << boolalpha << "z == a-a ? " << (z == a-a) << endl;
  // cout << boolalpha << "len n(a) == 1.0 ? " << (a.Normalized().Length() == 1.) << endl;
  // cout << boolalpha << "dot a,b == 0.0 ? " << (a.Dot(b) == 0.) << endl;
  // cout << boolalpha << "dot a,a+b == 4.0 ? " << (a.Dot(a+b) == 4.) << endl;
  // cout << boolalpha << "cross a,b == 2.0 ? " << (a.Cross(b) == 2.) << endl;
  // cout << boolalpha << "cross a,a == 0.0 ? " << (a.Cross(a) == 0.) << endl;
  // cout << boolalpha << "cross a,-a == 0.0 ? " << (a.Cross(-a) == 0.) << endl;
  // cout << boolalpha << "norm reflect a,n(a)-n(b) == n(b) ? " << ((a.Reflect(a.Normalized() - b.Normalized())).Normalized() == b.Normalized()) << endl;
  // cout << boolalpha << "(a+b).y == 1.0 ? " << ((a+b).y == 1.) << endl;
  // cout << boolalpha << "(a-b).y == -1.0 ? " << ((a-b).y == -1.) << endl;
  // cout << boolalpha << "a * 2.35 == 2.35 * a ? " << (a * 2.35 == 2.35 * a) << endl;

  return 0;
}
