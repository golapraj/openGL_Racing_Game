#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include<stdio.h>
#include <windows.h>
#include <math.h>
#include<iostream>
using namespace std;

bool movd=false;
float posx=0,posz=0;
int score=0;

GLfloat alpha = 0.0, theta = 0.0, axis_x=0.0, axis_y=0.0;
GLboolean bRotate = false, uRotate = false;

int rh=5,rw=5,rl=7;
float ex=3,ey=3,ez=13,lx=3,ly=3,lz=1;

void Write(char *string) //Write string on the screen
{
    while(*string)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *string++);
}

void GameStatus()
{

    char tmp_str[40];

    // Print the status of the game on the screen
    glColor3f(0, 0, 0);
    glRasterPos2f(3, 5);

    sprintf(tmp_str, "Score: %d", score);
    Write(tmp_str);
}

void draw_road()
{
    glPushMatrix();
    glColor3f(.2, .2, .2);
    glTranslatef(3,0,-7);
    glScalef(6,1.0,30);
    glutSolidCube(1);
    glPopMatrix();
}
void draw_divider()//black patch drawn in middle of road
{
    glPushMatrix();
    glTranslatef(3,.5,-7+movd);
    glScalef(0.2,0.01,1);
    for(int i = 1; i <= 10; i++)
    {
        glColor3f(1, 1, 1);
        glTranslatef(0,0,2);
        glutSolidCube(.5);
        glTranslatef(0,0,2);
        glutSolidCube(.5);
    }
    glPopMatrix();
}

void draw_car(float posx,float posz,int h=0)
{
    glPushMatrix();
    glColor3f(0, 0, 0);
    glTranslatef(2+posx,1,8+posz);
    glScalef(.05,.05,.05);
    glRotatef(90,0,1,0);
    glutSolidTorus(1.1,1.2,10,10);
    glTranslatef(0,0,9);
    glutSolidTorus(1.1,1.2,10,10);
    glTranslatef(-15,0,-8);
    glutSolidTorus(1.1,1.2,10,10);
    glTranslatef(0,0,9);
    glutSolidTorus(1.1,1.2,10,10);
    glColor3f(0,1,0);
    glTranslatef(6,4,-5);
    glScalef(3,1,1.5);
    glutSolidCube(6);

    if(h>0)
    {
        glTranslatef(-3,1,0);
        glScalef(1,1,1);
        glColor3f(1, 1, 1);
        glutSolidSphere(1,10,10);
    }
    glPopMatrix();
}
int run=0;
int len1p=-10,len2p=-18,len3p=-14,len4p=-22;
void op_car(float len)
{
    if(len1p>20)
    {
        len1p=-10;
    }
    if(len2p>20)
    {
        len2p=-18;
    }

    if(len3p>20)
    {
        len3p=-14;
    }
    if(len4p>20)
    {
        len4p=-22;
    }

    glPushMatrix();

    draw_car(len,len1p++,1);
    draw_car(len-1,len2p++,2);
    draw_car(len+1.5,len3p++,3);
    draw_car(len+2.5,len4p++,4);
    //cout<<len1p<<endl;
    //cout<<len2p<<endl;
    //cout<<len3p<<endl;
    //cout<<len4p<<endl;
    Sleep(200);
    glPopMatrix();
}
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode( GL_PROJECTION );
    glClearColor(.5,.5,.5,1);
    glLoadIdentity();
    //glFrustum(-5,5,-5,5, 4, 50);
    gluPerspective(65,1,0.50,20);
    //glOrtho(-5,5,-5,5, 4, 50);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    gluLookAt(ex,ey,ez, lx,ly,lz, 0,1,0);
    glViewport(0,0,800,800);
    score++;
    GameStatus();
    draw_road();
    draw_divider();
    draw_car(posx,posz);
    op_car(0);
    movd=!movd;

    Sleep(100);
    glFlush();
    glutSwapBuffers();

}


void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 'j':
        posx=-1;
        break;
    case 'k':
        if(posx==-1)
            posx=0;
        else if(posx==0)
            posx=1.5;
        else if(posx==1.5)
            posx=2.5;
        break;

    case 't':

        break;
    case 'g':
        break;

    case 's':
    case 'S':
        bRotate = !bRotate;
        uRotate = false;
        axis_x=0.0;
        axis_y=1.0;
        break;

    case 'r':
    case 'R':
        uRotate = !uRotate;
        bRotate = false;
        axis_x=1.0;
        axis_y=0.0;
        break;

    case 27:	// Escape key
        exit(1);
    }
}

void animate()
{
    if (bRotate == true)
    {
        theta += 0.1;
        if(theta > 360.0)
            theta -= 360.0*floor(theta/360.0);
    }

    if (uRotate == true)
    {
        alpha += 0.1;
        if(alpha > 360.0)
            alpha -= 360.0*floor(alpha/360.0);
    }
    glutPostRedisplay();

}
int main (int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(0,0);
    glutInitWindowSize(800,800);
    glutCreateWindow("myRoom");

    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_NORMALIZE);

    glutKeyboardFunc(myKeyboardFunc);

    glutDisplayFunc(display);

    glutIdleFunc(animate);

    glEnable(GL_NORMALIZE);

    glutMainLoop();

    return 0;
}
