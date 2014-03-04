#include <string>
#include <iostream>

using namespace std;

#include "joint.h"

class skeleton
{
    public:
        skeleton(string skl_file);
        void draw();

    private:
        joint root;
};
