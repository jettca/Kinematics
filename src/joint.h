#include <iostream>

class joint
{
    public:
        joint();
        joint(double x, double y, joint *parent);

        double getx();
        double gety();
        const joint *getparent();

    private:
        double x, y;
        joint *parent;
};
