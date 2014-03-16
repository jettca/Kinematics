/* A class for 3-vectors to simplify various calculations
 */

#ifndef _POINT_H_
#define _POINT_H_

class point
{
    public:
        point();
        point(double x, double y, double z);
        double magnitude();
        point normalize();
        point times(double a);
        point plus(point p);
        double dot(point p);
        point cross(point p);

        double getx();
        double gety();
        double getz();

        void print();
        
    private:
        double x, y, z;
};

#endif
