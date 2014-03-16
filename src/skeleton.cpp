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
    ifstream skl_file(skl_dir);

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
            j = new joint(length, theta, axis, parent, numchilds);

            if(parent)
            {
                joints.push_back(j);
                while(parent->toAdd() == 0)
                {
                    parent = parent->getparent();
                }
                parent->addChild(j);
                parent = j;
            }
            else
            {
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

void skeleton::draw() const
{
    vector<joint*> children = root->getchildren();
    int size = children.size();

    glInitNames();
    glPushName(0);
    for(int i = 0; i < size; i++)
    {
        draw(*children.at(i), firstName + i);
    }
}

int skeleton::draw(joint j, int name) const
{
    glLoadName(name);
    glPushMatrix();
    {
        point axis = j.getaxis();

        glRotatef(j.gettheta(), axis.getx(), axis.gety(), axis.getz());
        j.draw();
        glTranslatef(0, 0, j.getlength());

        vector<joint*> children = j.getchildren();
        vector<joint*>::iterator child;
        for(child = children.begin(); child != children.end(); child++)
        {
            name = draw(**child, name + 1);
        }
    }
    glPopMatrix();

    return name;
}

int skeleton::numJoints() const
{
    return joints.size();
}

joint* skeleton::getjoint(int i)
{
    return joints.at(i);
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
