#include <iostream>
#include <vector>

using namespace std;

#include "point.h"

class joint
{
    public:
        joint();
        joint(double length, double theta, point axis, joint *parent, int numchilds);

        void addChild(joint *child);
        void rotate(double phi);
        void draw() const;
        point derivativeAll(joint* j, point p) const;
        point apply(point p) const;
        point applyAll() const;

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
