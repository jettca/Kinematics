#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

#include <math.h>

#include "joint.h"
#include "quaternion.h"

joint::joint() :
    length(),
    theta(0),
    axis(),
    parent(NULL),
    numchilds(0),
    children(),
    displist()
{}

joint::joint(double length, double theta, point axis, joint *parent, int numchilds) :
    length(length),
    theta(theta),
    axis(axis),
    parent(parent),
    numchilds(numchilds),
    children()
{
    displist = glGenLists(1);

    double radius, slices, stacks;
    radius = .1;
    slices = 20;
    stacks = 5;

    // Set the display list for drawing the joint as a cylinder
    GLUquadric *jointQuadric = gluNewQuadric();

    glNewList(displist, GL_COMPILE);
    {
        gluCylinder(jointQuadric, radius, radius, length, slices, stacks);
    }
    glEndList();
    gluDeleteQuadric(jointQuadric);
}

void joint::addChild(joint *child)
{
    children.push_back(child);
}

void joint::rotate(double phi)
{
    theta = theta + phi;
    if(theta >= 360)
        theta -= 360;
    else if(theta < 0)
        theta += 360;
}

void joint::draw() const
{
    glCallList(displist);
}

point joint::derivativeAll(joint *j, point p) const
{
    // Performs a first order taylor approximation
    double dtheta = .001;
    j->rotate(-dtheta);
    point p1 = applyAll();
    j->rotate(2*dtheta);
    point p2 = applyAll();
    j->rotate(-dtheta);
    return p2.plus(p1.times(-1)).times(1/(2*dtheta));
}

point joint::apply(point p) const
{
    p = p.plus(point(0, 0, length));
    quaternion q = makeQuaternion(theta, axis);
    return q.rotate(p);
}

point joint::applyAll() const
{
    const joint* j = this;
    point pos(0, 0, 0);

    while(j->parent != NULL)
    {
        pos = j->apply(pos);
        j = j->parent;
    }

    return pos;
}

void joint::settheta(double t)
{
    theta = t;
}

double joint::getlength() const
{
    return length;
}

point joint::getaxis() const
{
    return axis;
}

double joint::gettheta() const
{
    return theta;
}

int joint::toAdd() const
{
    return numchilds - children.size();
}

joint *joint::getparent()
{
    return parent;
}

vector<joint*> joint::getchildren()
{
    return children;
}
