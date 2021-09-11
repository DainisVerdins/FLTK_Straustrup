#include "superellipse.h"



void Superellipse::add_connected(const Point& a, const Point& b)
{
	graph.add(a, b);
}

double Superellipse::sgn(double x)
{
    return ((x > 0) - (x < 0)) * 1;
}

Superellipse::Superellipse(const Point& centerCoords, double a, double b, double m, double n, double precision)
{
    graph.set_color(Color::black);
    if (precision >= 1.00 || precision < 0.001)
        error("use numbers from range [0.001 ; 1.00) for precision parametr\n");

    Point temp{0,0};
    Point P;
    double t{};
    int x{};
    int y{};
    for (double d = -1.0; d < 1.00; d += precision) {
         t = d * M_PI;
         x = pow((abs(cos(t))), 2.0 / m) * a * sgn(cos(t));
         y = pow((abs(sin(t))), 2.0 / n) * b * sgn(sin(t));
        P = Point(x + centerCoords.x, y + centerCoords.y);
        if (P != temp) { // ignore dublicates
            points.push_back(P);
        }
        temp = P;
    }
    for (size_t i = 0; i < points.size()-1; i++)
    {
        this->add_connected(points[i], points[i+1]);
    }
    std::cout << points.size() << std::endl;
   // this->add_connected(points[0], points[1]);
}

Superellipse::~Superellipse()
{
}
