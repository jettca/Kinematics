/* A class for 3-vectors to simplify various calculations
 */

#ifndef _POINT_H_
#define _POINT_H_

class point
{
    public:
        point();
        point(double x, double y, double z);
        double magnitude() const;
        point normalize() const;
        point times(double a) const;
        point plus(point p) const;
        double dot(point p) const;
        point cross(point p) const;

        double getx() const;
        double gety() const;
        double getz() const;

        void print() const;
        
    private:
        double x, y, z;
};

#endif
