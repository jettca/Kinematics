#ifndef __POINT_H__
#   define __POINT_H__
#   include "point.h"
#endif

class quaternion
{
    public:
        /* create zero quaternion
         */
        quaternion();

        /* create quaternion from direct values
         * spelled out or in point form
         */
        quaternion(double w, double x, double y, double z);
        quaternion(double w, point p);

        /* get rotation angle
         */
        double theta();

        /* get rotation vector components
         */
        double vx();
        double vy();
        double vz();

        /* get values stored directly in quaternion
         */
        double getw();
        double getx();
        double gety();
        double getz();

        /* multiple q with current quaternion; return result
         */
        quaternion compose(quaternion q);

        /* interpolate quaternions with slerp
         */
        quaternion interpolate(quaternion q, double h);

        /* rotate p by theta about <vx, vy, vz>
         */
        point rotate(point p);

        /* random quaternion operations
         */
        void normalize();
        double magnitude();
        void conjugate();

        void print();

    private:
        double w, x, y, z;
};

/* generate quaternion from rotation angle and vector
 */
quaternion makeQuaternion(double theta, point p);
