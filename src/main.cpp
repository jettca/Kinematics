#if defined(__APPLE__) || defined(MACOSX)
#   include <GLUT/glut.h>
#else
#   include <GL/glut.h>
#endif

#include <iostream>
#include <string>
#include <sstream>

#include "skeleton.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>
#include <math.h>

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
skeleton robot;
vector<joint*> joints;
vector<joint*> actuators;

// Clipping
float fovy = 45;
float zNear = .01;
float zFar = 100;
int win_width = 960;
int win_height = 540;
glm::mat4 projection = glm::perspective(fovy, (float)win_width/win_height, zNear, zFar);

glm::mat4 viewR;
glm::mat4 viewT;

// Input
int selected = -1;
vector<point> xydpoints;
bool UDRL[4] = {false, false, false, false};
bool RM = false;
bool LM = false;
point clickpos;

// FK/IK
int mode = 1;
double radius = .1;
vector<point> targets;
vector<joint*> sources;

// Convert mouse coordinates + depth to world coordinates
point mouseToObj(point xyd)
{
    int mx = xyd.getx();
    int my = xyd.gety();
    double depth = xyd.getz();

    float clipx = 2.0*mx/win_width - 1;
    float clipy = 2.0*(win_height - my)/win_height - 1;
    glm::vec4 clip1 = projection*viewT*glm::vec4(0, 0, depth, 1);
    glm::vec4 clip(clipx, clipy, clip1.z/clip1.w, 1);

    glm::vec4 world = glm::inverse(projection*viewT*viewR)*clip;

    return point(world.x/world.w, world.y/world.w, world.z/world.w);
}

// Draws target points for IK
void drawTargets()
{
    for(int i = 0; i < targets.size(); i++)
    {
        glPushMatrix();
        {
            glPushName(joints.size() + i);
            point target = targets.at(i);

            glTranslatef(target.getx(), target.gety(), target.getz());
            glutSolidSphere(radius, 10, 10);

            glPopName();
        }
        glPopMatrix();
    }
}

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

// Sets the camera and the corresponding glm matrices
void setCamera()
{
    glTranslatef(-camPosX, -camPosY, -camPosZ);
    glRotatef(camRotY, 0, 1, 0);
    viewR = glm::rotate(glm::mat4(1.0f), camRotY, glm::vec3(0, 1, 0));
    viewT = glm::translate(glm::mat4(1.0f), glm::vec3(-camPosX, -camPosY, -camPosZ));
}

// Sets the target positions based on the mouse coordinates
void setTargets()
{
    glPushMatrix();
    {
        setCamera();
        for(int i = 0; i < targets.size(); i++)
        {
            targets[i] = mouseToObj(xydpoints.at(i));
        }
    }
    glPopMatrix();
}

void setupRC()
{
    // Place Camera
    camPosX = 0.0f;
    camPosY = 0.0f;
    camPosZ = 15.0f;
    camRotY = 0.0f;
    
    glEnable(GL_DEPTH);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthRange(0.0f, 1.0f);

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    initLights();
}

// loads the robot/skeleton
void loadRobot(string skel_dir)
{
    robot = *(new skeleton(skel_dir, 0));

    joints = robot.getjoints();
    actuators = robot.getactuators();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Set the clipping volume
    gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, zNear, zFar);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Draws the information text
void drawText()
{
    glPushMatrix();

    GLfloat scalar = .0008;
    glTranslatef(-2.7, 1.9, -5);
    glScalef(scalar, scalar, scalar);

    vector<joint*>::iterator joint_it;

    string text;
    if(mode == 1)
    {
        text = "Fordward Kinematics Mode";
    }
    else
    {
        text = "Inverse Kinematics Mode";
    }


    string::iterator t_it;
    for(t_it = text.begin(); t_it != text.end(); t_it++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *t_it);
        glTranslatef(20, 0, 0);
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
        if(mode == 2)
        {
            drawTargets();
        }
        robot.draw(mode);
    }
    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}

// Figures out what object you're selecting by name
void processSelection(int x, int y)
{
    static GLuint selectBuff[BUFFER_LENGTH];
    glSelectBuffer(BUFFER_LENGTH, selectBuff);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    glMatrixMode(GL_PROJECTION);

    glPopMatrix();

    glPushMatrix();
    {
        glRenderMode(GL_SELECT);
        glInitNames();

        glLoadIdentity();
        gluPickMatrix(x, viewport[3] - y + viewport[1], .1, .1, viewport);

        GLdouble aspect = (float)viewport[2]/viewport[3];
        gluPerspective(fovy, aspect, zNear, zFar);

        glPushMatrix();
        {
            drawText();
            setCamera();
            drawTargets();
            robot.draw(mode);
        }
        glPopMatrix();

        selected = selectBuff[3];

        glRenderMode(GL_RENDER);
        glMatrixMode(GL_PROJECTION);
    }
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);

    if(mode == 2 && selected >=0 && selected < joints.size())
    {
        joint *j = actuators.at(selected);
        int foundPos = -1;
        for(int i = 0; i < sources.size(); i++)
        {
            if(sources.at(i) == j)
            {
                foundPos = i;
                break;
            }
        }
        if(foundPos == -1)
        {
            sources.push_back(j);
            point t = j->applyAll();
            targets.push_back(t);
            xydpoints.push_back(point(0, 0, 0));
        }
        else
        {
            sources.erase(foundPos + sources.begin());
            targets.erase(foundPos + targets.begin());
            xydpoints.erase(foundPos + xydpoints.begin());
        }
    }
}

// Sets mouse booleans, gets click positions, and finds selected object
void mouse(int button, int state, int x, int y)
{
    if(state == GLUT_DOWN)
    {
        processSelection(x, y);
    }
    if(button == GLUT_RIGHT_BUTTON)
    {
        if(state == GLUT_DOWN && selected >= joints.size())
        {
            RM = true;
            clickpos = point(x, y, targets.at(selected-joints.size()).getz());
        }
        else
        {
            RM = false;
        }
    }
    else if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN && selected >= 0 && selected < joints.size())
        {
            LM = true;
            clickpos = point(x, y, joints.at(selected)->gettheta());
        }
        else
        {
            LM = false;
        }
    }
}

// Switches between FK and IK mode
void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case ' ':
            if(mode == 1)
                mode = 2;
            else
                mode = 1;
            selected = -1;
            break;
    }
}

// Sets arrow key booleans
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

// Repeatedly calls IK and processes rotation
void update()
{
    if(UDRL[2])
        camRotY += 3;
    if(UDRL[3])
        camRotY -= 3;

    if(mode == 2)
    {
        robot.touch(sources, targets);
    }

    glutPostRedisplay();
}

// Processes mouse movements for skeleton manipulation
void trackMouse(int x, int y)
{
    if(mode == 2 && selected >= joints.size())
    {
        point *s = &xydpoints.at(selected - joints.size());
        if(RM)
        {
            double shift = (clickpos.gety() - y)/50.;
            *s = point(s->getx(), s->gety(), clickpos.getz()+shift);
        }
        else
        {
            *s = point(x, y, s->getz());
        }
        setTargets();
    }
    else if(mode == 1)
    {
        if(LM && selected >= 0 && selected <= joints.size())
        {
            double shift = clickpos.gety() - y;
            joints.at(selected)->settheta(clickpos.getz() + shift);
        }
    }
}

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        cout << "incorrect number of arguments\n";
        exit(1);
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow("jett has a new thing");

    gluPerspective(fovy, (GLfloat)win_width/win_height, zNear, zFar);

    setupRC();
    loadRobot(string(argv[1]));

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutSpecialUpFunc(specialUp);
    glutIdleFunc(update);
    glutMotionFunc(trackMouse);

    glutMainLoop();

    delete &robot;

    return 0;
}
