#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

#include "joint.h"

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
}

void joint::draw() const
{
    glCallList(displist);
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
