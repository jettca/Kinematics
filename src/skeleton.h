#include <string>
#include <iostream>

using namespace std;

#include "joint.h"

class skeleton
{
    public:
        skeleton();
        skeleton(string skl_dir, int firstName);
        void draw() const;
        void dump() const;
        int numJoints() const;
        joint* getjoint(int i);

        ~skeleton();

    private:
        void dump(joint *j) const;
        int draw(joint j, int name) const;
        void freejoint(joint *j);

        joint *root;
        vector<joint*> joints;
        int firstName;
};
