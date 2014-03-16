#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

#include <iostream>
#include <string>
#include <sstream>

#include "skeleton.h"

#define BUFFER_LENGTH 64

using namespace std;

GLfloat camRotX, camRotY, camPosX, camPosY, camPosZ;

// Lights & Materials
GLfloat ambient[] = {0.2, 0.2, 0.2, 1.0};
GLfloat position[] = {0.0, 0.0, 2.0, 1.0};
GLfloat mat_diffuse[] = {0.6, 0.6, 0.6, 1.0};
GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_shininess[] = {50.0};

// Skeletons
string skel_dir1("static/skeleton1.skl");
skeleton robot;

// Input stuff
int selectedJoint = -1;
vector<joint*> joints;
GLint viewport[4];
bool UDRL[4] = {false, false, false, false};

// Clipping
GLdouble fovy = 45;
GLdouble zNear = 1;
GLdouble zFar = 100;

void initLights()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL) ;
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE) ;
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void setupRC()
{
    // Place Camera
    camPosX = 0.0f;
    camPosY = 0.0f;
    camPosZ = 15.0f;
    camRotY = 0.0f;
    
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    initLights();
}

void loadRobot()
{
    robot = *(new skeleton(skel_dir1, 0));
    int size = robot.numJoints();
    for(int i = 0; i < size; i++)
    {
        joint* j = robot.getjoint(i);
        joints.push_back(j);
    }
}

void setCamera()
{
    glTranslatef(-camPosX, -camPosY, -camPosZ);
    glRotatef(camRotY, 0, 1, 0);
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Set the clipping volume
    gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 1.0f, 100.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void drawText()
{
    glPushMatrix();

    GLfloat scalar = .0008;
    glTranslatef(-2.7, 1.9, -5);
    glScalef(scalar, scalar, scalar);

    vector<joint*>::iterator joint_it;


    for(joint_it = joints.begin(); joint_it != joints.end(); joint_it++)
    {
        glPushMatrix();

        ostringstream s;
        s << (*joint_it)->gettheta();
        string datum = s.str();

        string::iterator digit;
        for(digit = datum.begin(); digit != datum.end(); digit++)
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *digit);
            glTranslatef(20, 0, 0);
        }
        glPopMatrix();

        glTranslatef(0, -150, 0);
    }
    glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    {
        drawText();
        setCamera();
        robot.draw();
    }
    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}

void processSelection(int x, int y)
{
    static GLuint selectBuff[BUFFER_LENGTH];
    glSelectBuffer(BUFFER_LENGTH, selectBuff);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    glMatrixMode(GL_PROJECTION);

    glPushMatrix();
    {
        glRenderMode(GL_SELECT);
        glLoadIdentity();
        gluPickMatrix(x, viewport[3] - y + viewport[1], .1, .1, viewport);

        GLdouble aspect = (float)viewport[2]/viewport[3];
        gluPerspective(fovy, aspect, zNear, zFar);

        glPushMatrix();
        {
            drawText();
            setCamera();
            robot.draw();
        }
        glPopMatrix();

        selectedJoint = selectBuff[3];

        glRenderMode(GL_RENDER);
        glMatrixMode(GL_PROJECTION);
    }
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        processSelection(x, y);
    }
}

void special(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            UDRL[0] = true;
            break;
        case GLUT_KEY_DOWN:
            UDRL[1] = true;
            break;
        case GLUT_KEY_RIGHT:
            UDRL[2] = true;
            break;
        case GLUT_KEY_LEFT:
            UDRL[3] = true;
            break;
    }
}

void specialUp(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            UDRL[0] = false;
            break;
        case GLUT_KEY_DOWN:
            UDRL[1] = false;
            break;
        case GLUT_KEY_RIGHT:
            UDRL[2] = false;
            break;
        case GLUT_KEY_LEFT:
            UDRL[3] = false;
            break;
    }
}

void update()
{
    if(UDRL[0])
        if(selectedJoint != -1)
            joints.at(selectedJoint)->rotate(5);
    if(UDRL[1])
        if(selectedJoint != -1)
            joints.at(selectedJoint)->rotate(-5);
    if(UDRL[2])
        camRotY += 3;
    if(UDRL[3])
        camRotY -= 3;

    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    int win_width = 800;
    int win_height = 600;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow("jett has a new thing");

    setupRC();
    loadRobot();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutSpecialFunc(special);
    glutSpecialUpFunc(specialUp);
    glutIdleFunc(update);

    glutMainLoop();

    delete &robot;
}
