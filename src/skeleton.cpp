#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

#include "skeleton.h"

skeleton::skeleton(string skl_file) :
    root()
{
}

void skeleton::draw()
{
}
