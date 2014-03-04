#include "joint.h"

joint::joint() :
    x(0),
    y(0),
    parent(NULL)
{}

joint::joint(double x, double y, joint *parent) :
    x(x),
    y(y),
    parent(parent)
{}

double joint::getx()
{
    return x;
}

double joint::gety()
{
    return y;
}

const joint *joint::getparent()
{
    return parent;
}
