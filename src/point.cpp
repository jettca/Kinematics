#include "point.h"
#include <math.h>
#include <iostream>

using namespace std;

point::point(double x, double y, double z) :
    x(x),
    y(y),
    z(z)
{}

double point::magnitude()
{
    return sqrt(x*x + y*y + z*z);
}

point point::normalize()
{
    double m = magnitude();
    if(m > 0)
        return point(x, y, z).times(1/m);
    else
        return *this;
}

point point::times(double a)
{
    return point(x*a, y*a, z*a);
}

point point::plus(point p)
{
    return point(x + p.getx(), y + p.gety(), z + p.getz());
}

double point::dot(point p)
{
    return x*p.getx() + y*p.gety() + z*p.getz();
}

point point::cross(point p)
{
    return point(y*p.getz()-z*p.gety(), z*p.getx()-x*p.getz(), x*p.gety()-y*p.getx());
}

double point::getx()
{
    return x;
}

double point::gety()
{
    return y;
}

double point::getz()
{
    return z;
}

void point::print()
{
    cout << x << " " << y << " " << z << endl;
}
