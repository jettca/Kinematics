#include "point.h"
#include <math.h>
#include <iostream>

using namespace std;

point::point() :
    x(0),
    y(0),
    z(0)
{}

point::point(double x, double y, double z) :
    x(x),
    y(y),
    z(z)
{}

double point::magnitude() const
{
    return sqrt(x*x + y*y + z*z);
}

point point::normalize() const
{
    double m = magnitude();
    if(m > 0)
        return point(x, y, z).times(1/m);
    else
        return *this;
}

point point::times(double a) const
{
    return point(x*a, y*a, z*a);
}

point point::plus(point p) const
{
    return point(x + p.getx(), y + p.gety(), z + p.getz());
}

double point::dot(point p) const
{
    return x*p.getx() + y*p.gety() + z*p.getz();
}

point point::cross(point p) const
{
    return point(y*p.getz()-z*p.gety(), z*p.getx()-x*p.getz(), x*p.gety()-y*p.getx());
}

double point::getx() const
{
    return x;
}

double point::gety() const
{
    return y;
}

double point::getz() const
{
    return z;
}

void point::print() const
{
    cout << x << " " << y << " " << z << endl;
}
