#include <string>
#include <iostream>

using namespace std;

#include "joint.h"

class skeleton
{
    public:
        /* default constructor
         */
        skeleton();

        /* Construct skeleton from directory to load from
         * firstName is for select buffer
         */
        skeleton(string skl_dir, int firstName);

        /* Print out skeleton data
         */
        void dump() const;

        /* Sets selected actuators to touch corresponding targets
         * Least-squares-based IK solver
         */
        void touch(vector<joint*>& actuators, const vector<point>& targets);

        /* Draws skeleton using forward kinematics
         */
        void draw(int mode);

        /* Various getters
         */
        int numJoints() const;
        joint* getjoint(int i);
        vector<joint*> getjoints();
        vector<joint*> getactuators();

        ~skeleton();

    private:
        /* Recursive utility functions
         */
        void dump(joint *j) const;
        void draw(joint j, int mode);
        void freejoint(joint *j);

        joint *root;
        vector<joint*> joints;
        vector<joint*> actuators;
        int firstName, curName;
};
