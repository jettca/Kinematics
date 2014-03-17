#include <string>
#include <iostream>

using namespace std;

#include "joint.h"

class skeleton
{
    public:
        skeleton();
        skeleton(string skl_dir, int firstName);

        void dump() const;

        void touch(vector<joint*>& actuators, const vector<point>& targets);
        void draw(int mode);

        int numJoints() const;
        joint* getjoint(int i);
        vector<joint*> getjoints();
        vector<joint*> getactuators();

        ~skeleton();

    private:
        void dump(joint *j) const;
        void draw(joint j, int mode);
        void freejoint(joint *j);

        joint *root;
        vector<joint*> joints;
        vector<joint*> actuators;
        int firstName, curName;
};
