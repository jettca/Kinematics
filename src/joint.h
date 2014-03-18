#include <iostream>
#include <vector>

using namespace std;

#include "point.h"

class joint
{
    public:
        /* Default constructor
         */
        joint();

        /* Constructs a joint with required data
         * numchilds, the final size of children,
         * is used for constructing skeletons
         */
        joint(double length, double theta, point axis, joint *parent, int numchilds);

        /* Adds a joint to the children list
         */
        void addChild(joint *child);

        /* Adds phi to theta
         */
        void rotate(double phi);

        /* draws the joint in the positive z direction
         */
        void draw() const;

        /* Calculates the derivative with respect to theta
         * of the point p  connected to this joint
         * as joint j rotates
         */
        point derivativeAll(joint* j, point p) const;

        /* Shifts and rotates the point p with the joint
         */
        point apply(point p) const;

        /* Gets the ending point of the joint in skeleton coordinates
         */
        point applyAll() const;

        /* miscellaneous getters and setters
         */
        void settheta(double t);
        double getlength() const;
        point getaxis() const;
        double gettheta() const;
        int toAdd() const;
        joint *getparent();
        vector<joint*> getchildren();

    private:
        double length, theta;
        point axis;

        joint *parent;
        int numchilds;
        vector<joint*> children;

        GLuint displist;
};
