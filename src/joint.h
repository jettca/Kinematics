#include <iostream>

#include "point.h"

class joint
{
    public:
        joint(point pos, joint *parent);

        point getpos();
        const joint *getparent();

    private:
        point pos;
        joint *parent;
};
