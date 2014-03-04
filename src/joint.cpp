#include "joint.h"

joint::joint(point pos, joint *parent) :
    pos(pos),
    parent(parent)
{}

point joint::getpos()
{
    return pos;
}

const joint *joint::getparent()
{
    return parent;
}
