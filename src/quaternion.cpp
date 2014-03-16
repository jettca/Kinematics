#include "quaternion.h"

#include <iostream>
#include <cmath>
using namespace std;

quaternion::quaternion() :
    w(0),
    x(0),
    y(1),
    z(0)
{}

quaternion::quaternion(double w, double x, double y, double z) :
    w(w),
    x(x),
    y(y),
    z(z)
{}

quaternion::quaternion(double w, point p) :
    w(w),
    x(p.getx()),
    y(p.gety()),
    z(p.getz())
{}

double quaternion::theta()
{
    return acos(w)*360/M_PI;
}

double quaternion::vx()
{
    double st = sin(acos(w));
    if(st != 0)
        return x/sin(acos(w));
    else
        return 0;
}

double quaternion::vy()
{
    double st = sin(acos(w));
    if(st != 0)
        return y/sin(acos(w));
    else
        return 0;
}

double quaternion::vz()
{
    double st = sin(acos(w));
    if(st != 0)
        return z/sin(acos(w));
    else
        return 0;
}

double quaternion::getw()
{
    return w;
}

double quaternion::getx()
{
    return x;
}

double quaternion::gety()
{
    return y;
}

double quaternion::getz()
{
    return z;
}

quaternion quaternion::compose(quaternion q)
{
    double neww, newx, newy, newz;
    neww = w*q.getw() - x*q.getx() - y*q.gety() - z*q.getz();
    newx = y*q.getz() - z*q.gety() + w*q.getx() + q.getw()*x;
    newy = z*q.getx() - x*q.getz() + w*q.gety() + q.getw()*y;
    newz = x*q.gety() - y*q.getx() + w*q.getz() + q.getw()*z;

    return quaternion(neww, newx, newy, newz);
}

quaternion quaternion::interpolate(quaternion q, double h)
{
    double magnitudes = magnitude()*q.magnitude();
    double omega;
    
    // lots of NAN prevention
    if(magnitudes == 0)
        omega = 0;
    else
    {
        double cosOmega = w*q.getw() + x*q.getx() + y*q.gety() + z*q.getz()/magnitudes;
        if(cosOmega >= 1)
            omega = 0;
        else
            omega = acos(cosOmega);
    }

    if(omega == 0)
        return *this;
    else
    {
        double nw = (w*sin((1-h)*omega) + q.getw()*sin(h*omega))/sin(omega);
        double nx = (x*sin((1-h)*omega) + q.getx()*sin(h*omega))/sin(omega);
        double ny = (y*sin((1-h)*omega) + q.gety()*sin(h*omega))/sin(omega);
        double nz = (z*sin((1-h)*omega) + q.getz()*sin(h*omega))/sin(omega);

        return quaternion(nw, nx, ny, nz);
    }
}

void quaternion::normalize()
{
    double m = magnitude();
    if(m > 0)
    {
        w /= m;
        x /= m;
        y /= m;
        z /= m;
    }
}

double quaternion::magnitude()
{
    return sqrt(w*w + x*x + y*y + z*z);
}

point quaternion::rotate(point p)
{
    quaternion v = quaternion(0, p);
    quaternion qconj = quaternion(w, -x, -y, -z);
    quaternion f = (*this).compose(v.compose(qconj));

    point result = point(f.vx(), f.vy(), f.vz());
    return result;
}

void quaternion::conjugate()
{
    x = -x;
    y = -y;
    z = -z;
}

quaternion makeQuaternion(double theta, point p)
{
    theta = theta*M_PI/360;
    double st = sin(theta);
    quaternion q(cos(theta), st*p.getx(), st*p.gety(), st*p.getz());
    q.normalize();
    return q;
}

void quaternion::print()
{
    cout << w << " " << x << " " << y << " " << z << endl;
}
