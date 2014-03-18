#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

#include "skeleton.h"
#include "Eigen/Dense"
#include "unsupported/Eigen/NonLinearOptimization"
#include "unsupported/Eigen/NumericalDiff"

skeleton::skeleton() :
    root(NULL),
    joints(),
    firstName(0)
{}

skeleton::skeleton(string skl_dir, int firstName) :
    root(NULL),
    joints(),
    firstName(firstName)
{
    ifstream skl_file(skl_dir.c_str());

    if(skl_file.is_open())
    {
        string line;
        string datum;
        joint *parent = NULL;
        joint *j;

        const int data_size = 6;
        double data[data_size];
        int numchilds = 0;

        while(getline(skl_file, line))
        {
            istringstream iss(line);
            vector<string> tokens;
            while(iss >> datum)
            {
                tokens.push_back(datum);
            }

            for(int i = 0; i < data_size; i++)
            {
                istringstream(tokens.at(i)) >> data[i];
            }
            double length = data[0];
            point axis(data[1], data[2], data[3]);
            double theta = data[4];
            numchilds = data[5];

            if(parent)
            {
                while(parent->toAdd() == 0)
                {
                    parent = parent->getparent();
                }

                j = new joint(length, theta, axis, parent, numchilds);
                parent->addChild(j);
                parent = j;

                joints.push_back(j);
                if(numchilds == 0)
                {
                    actuators.push_back(j);
                }

            }
            else
            {
                j = new joint(length, theta, axis, parent, numchilds);
                parent = j;
                root = j;
            }
        }
        skl_file.close();
    }
}

void skeleton::dump() const
{
    dump(root);
}

void skeleton::dump(joint *j) const
{
    cout << endl;
    printf("%f", j->getlength());
    j->getaxis().print();

    vector<joint*> children = j->getchildren();
    int size = children.size();
    for(int i = 0; i < size; i++)
    {
        dump(children.at(i));
    }
}

void skeleton::draw(int mode)
{
    vector<joint*> children = root->getchildren();
    int size = children.size();

    curName = firstName;
    for(int i = 0; i < size; i++)
    {
        draw(*children.at(i), mode);
    }
}

void skeleton::draw(joint j, int mode)
{
    glPushMatrix();
    {
        point axis = j.getaxis();
        vector<joint*> children = j.getchildren();

        glRotatef(j.gettheta(), axis.getx(), axis.gety(), axis.getz());

        if(mode == 1 || (mode == 2 && children.size() == 0))
        {
            glPushName(curName);
            j.draw();
            glPopName();
            curName++;
        }
        else
        {
            j.draw();
        }

        glTranslatef(0, 0, j.getlength());

        vector<joint*>::iterator child;
        for(child = children.begin(); child != children.end(); child++)
        {
            if(mode == 1 || children.size() == 0)
            {
                draw(**child, mode);
            }
            else
            {
                draw(**child, mode);
            }
        }
    }
    glPopMatrix();
}

int skeleton::numJoints() const
{
    return joints.size();
}

joint* skeleton::getjoint(int i)
{
    return joints.at(i);
}

vector<joint*> skeleton::getjoints()
{
    return joints;
}

vector<joint*> skeleton::getactuators()
{
    return actuators;
}

skeleton::~skeleton()
{
    freejoint(root);
}

void skeleton::freejoint(joint *j)
{
    vector<joint*> children = j->getchildren();
    vector<joint*>::iterator child_it;
    for(child_it = children.begin(); child_it != children.end(); child_it++)
    {
        freejoint(*child_it);
    }
    delete j;
}

/* The following code is for calling the Eigen 
 */
using namespace Eigen;

template<typename _Scalar, int NX=Dynamic, int NY=Dynamic>
struct Functor
{
    typedef _Scalar Scalar;
    enum {
        InputsAtCompileTime = NX,
        ValuesAtCompileTime = NY
    };
    typedef Matrix<Scalar,InputsAtCompileTime,1> InputType;
    typedef Matrix<Scalar,ValuesAtCompileTime,1> ValueType;
    typedef Matrix<Scalar,ValuesAtCompileTime,InputsAtCompileTime> JacobianType;

    const int m_inputs, m_values;

    Functor() : m_inputs(InputsAtCompileTime), m_values(ValuesAtCompileTime) {}
    Functor(int inputs, int values) : m_inputs(inputs), m_values(values) {}

    int inputs() const { return m_inputs; }
    int values() const { return m_values; }
};

int max(int x, int y)
{
    if(x > y)
        return x;
    return y;
}

struct dist_functor : Functor<double>
{
    vector<joint*> joints;
    vector<joint*> acts;
    vector<point> targets;
    dist_functor(vector<joint*>& _joints, vector<joint*>& _acts, const vector<point>& _targets): Functor<double>(_joints.size(), max(_joints.size(), 3*_targets.size()))
    {
        joints = _joints;
        acts = _acts;
        targets = _targets;
    };

    // Calculates difference between target and actuator positions
    int operator()(const VectorXd &x, VectorXd &fvec) const
    {
        int tsize = targets.size();
        for(int i = 0; i < inputs(); i++)
        {
            joints[i]->rotate(x[i]);
        }

        for(int i = 0; i < tsize; i++)
        { 
            point p = targets.at(i).plus(acts.at(i)->applyAll().times(-1));
            fvec[3*i] = p.getx();
            fvec[3*i+1] = p.gety();
            fvec[3*i+2] = p.getz();
        }
        for(int i = 3*tsize; i < values(); i++)
        {
            fvec[i] = 0;
        }

        for(int i = 0; i < inputs(); i++)
        {
            joints[i]->rotate(-x[i]);
        }
        return 0;
    }

    // Calculates the Jacobian of the function to be optimixed
    int df(const VectorXd &x, MatrixXd &fjac) const
    {
        int tsize = targets.size();
        for(int i = 0; i < inputs(); i++)
        {
            joints[i]->rotate(x[i]);
        }

        for(int j = 0; j < inputs(); j++)
        {
            for(int i = 0; i < tsize; i++)
            {
                joint *a = acts[i];
                point d = a->derivativeAll(joints[j], point(0, 0, 0)).times(-1);

                fjac(3*i, j) = d.getx();
                fjac(3*i+1, j) = d.gety();
                fjac(3*i+2, j) = d.getz();
            }
            for(int i = 3*tsize; i < values(); i++)
            {
                fjac(i, j) = 0;
            }
        }

        for(int i = 0; i < inputs(); i++)
        {
            joints[i]->rotate(-x[i]);
        }

        return 0;
    }
};

void skeleton::touch(vector<joint*>& acts, const vector<point>& targets)
{
    dist_functor functor(joints, acts, targets);
    LevenbergMarquardt<dist_functor> lm(functor);
    lm.parameters.xtol = 1.0e-30;
    lm.parameters.ftol = 0;

    int size = joints.size();
    VectorXd thetas(size);
    for(int i = 0; i < size; i++)
    {
        thetas[i] = 0;
    }
    lm.lmder1(thetas);
    for(int i = 0; i < thetas.size(); i++)
    {
        joints.at(i)->rotate(thetas[i]);
    }
}
