#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include<stdio.h>
#include <windows.h>
#include <math.h>
#include<iostream>
#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif
using namespace std;

bool movd=false,collisions=false;
float posx=0,posz=0;
int score=0;

int run=0;
int len1p=-10,len2p=-18,len3p=-14,len4p=-22;

int rh=5,rw=5,rl=7;
float ex=3.5,ey=3,ez=13,lx=3.5,ly=3,lz=1;



struct Image_Data
{
    unsigned long Size_X;
    unsigned long Size_Y;
    unsigned char *Data;
};

int Load_Bitmap(char* Filename, Image_Data* Out_Image_Data)
{
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int width, height;
    unsigned int imageSize;   // = width*height*3

    FILE * file = fopen(Filename,"rb");
    if (!file)
    {
        return 0;
    }

    if ( fread(header, 1, 54, file)!=54 )  // If not 54 bytes read : problem
    {
        return 0;
    }

    if ( header[0]!='B' || header[1]!='M' )
    {
        return 0;
    }

    // Read ints from the byte array
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)      dataPos=54; // The BMP header is done that way

    // Create a buffer
    Out_Image_Data->Data = new unsigned char [imageSize];

    // Read the actual data from the file into the buffer
    fread(Out_Image_Data->Data,1,imageSize,file);

    //Everything is in memory now, the file can be closed
    fclose(file);

    Out_Image_Data->Size_X = width;
    Out_Image_Data->Size_Y = height;

    return 1;
}

void Add_Texture(int ID, Image_Data* Data)
{
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,Data->Size_X,Data->Size_Y,0,GL_BGR,GL_UNSIGNED_BYTE,Data->Data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
}

GLuint Textures[10];

void draw_solid_cube(GLfloat size)
{
    float x1,y1,z1;
    float x2,y2,z2;
    float x3,y3,z3;
    float x4,y4,z4;

    x1 = -size/2;
    y1 = -size/2;
    z1 = -size/2;

    x2 = size/2;
    y2 = -size/2;
    z2 = -size/2;

    x3 = size/2;
    y3 = -size/2;
    z3 = size/2;

    x4 = -size/2;
    y4 = -size/2;
    z4 = size/2;

    float x5,y5,z5;
    float x6,y6,z6;
    float x7,y7,z7;
    float x8,y8,z8;

    x5 = -size/2;
    y5 = size/2;
    z5 = -size/2;

    x6 = size/2;
    y6 = size/2;
    z6 = -size/2;

    x7 = size/2;
    y7 = size/2;
    z7 = size/2;

    x8 = -size/2;
    y8 = size/2;
    z8 = size/2;

    glBegin(GL_QUADS);
    //Quad 1

    glTexCoord2f(0,0);
    glVertex3f(x1,y1,z1);

    glTexCoord2f(1,0);
    glVertex3f(x2,y2,z2);

    glTexCoord2f(1,1);
    glVertex3f(x3,y3,z3);

    glTexCoord2f(0,1);
    glVertex3f(x4,y4,z4);

    //Quad 2

    glTexCoord2f(0,0);
    glVertex3f(x8,y8,z8);

    glTexCoord2f(1,0);
    glVertex3f(x7,y7,z7);

    glTexCoord2f(1,1);
    glVertex3f(x6,y6,z6);

    glTexCoord2f(0,1);
    glVertex3f(x5,y5,z5);
    //Quad 3

    glTexCoord2f(0,0);
    glVertex3f(x5,y5,z5);

    glTexCoord2f(1,0);
    glVertex3f(x6,y6,z6);

    glTexCoord2f(1,1);
    glVertex3f(x2,y2,z2);

    glTexCoord2f(0,1);
    glVertex3f(x1,y1,z1);
    //Quad 4

    glTexCoord2f(0,0);
    glVertex3f(x7,y7,z7);

    glTexCoord2f(1,0);
    glVertex3f(x8,y8,z8);

    glTexCoord2f(1,1);
    glVertex3f(x4,y4,z4);

    glTexCoord2f(0,1);
    glVertex3f(x3,y3,z3);
    //Quad 5
    glTexCoord2f(0,0);
    glVertex3f(x6,y6,z6);

    glTexCoord2f(1,0);
    glVertex3f(x7,y7,z7);

    glTexCoord2f(1,1);
    glVertex3f(x3,y3,z3);

    glTexCoord2f(0,1);
    glVertex3f(x2,y2,z2);

    //Quad 6
    glTexCoord2f(0,0);
    glVertex3f(x1,y1,z1);

    glTexCoord2f(1,0);
    glVertex3f(x4,y4,z4);

    glTexCoord2f(1,1);
    glVertex3f(x8,y8,z8);

    glTexCoord2f(0,1);
    glVertex3f(x5,y5,z5);

    glEnd();
}

void Write(char *string) //Write string on the screen
{
    while(*string)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *string++);
}

bool collision()

{
    if(posx>=-0.5&&posx<=0.3)
        if(len1p>=0&&len1p<=3)
            return true;
    if(posx>=-1.0&&posx<=-0.5)
        if(len2p>=0&&len2p<=3)
            return true;
    if(posx>=2&&posx<=2.6)
        if(len4p==0)
            return true;
    if(posx>=1&&posx<=2)
        if(len3p>=0&&len3p<=3)
            return true;
    return false;
}

void GameStatus()
{

    char tmp_str[40];

    // Print the status of the game on the screen
    glColor3f(0, 0, 0);
    glRasterPos2f(3, 5);

    if(!collisions)
    {
        sprintf(tmp_str, "Score: %d %d %f", score,collision(),posx);
        Write(tmp_str);
    }
    else
    {
        glColor3f(1, 0, 0);
        sprintf(tmp_str, "Game over");
        Write(tmp_str);
    }
}

void draw_road()
{
    glPushMatrix();
    glColor3f(.2, .2, .2);
    glTranslatef(3,0,-7);
    glScalef(6,1.0,30);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,1);
    glPushMatrix();
    draw_solid_cube(1.1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glTranslatef(-.4,.6,0);
    glScalef(.01,.1,5);
    glColor3f(1,1,0);
    glutSolidCube(1);
    glTranslatef(88,.6,2);
    glScalef(.4,.1,.8);
    glColor3f(1,1,0);
    glutSolidCube(5);
    glPopMatrix();
}

void draw_footpath()//black patch drawn in middle of road
{
    glPushMatrix();
    glTranslatef(0,.5,-7);
    glScalef(0.5,1,35);

    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);
    glPushMatrix();
    draw_solid_cube(1.2);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glTranslatef(13,0,0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);
    glPushMatrix();
    draw_solid_cube(1.2);
    glPopMatrix();
    glScalef(1/0.5,1,1/35);
    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void draw_divider()//black patch drawn in middle of road
{
    glPushMatrix();
    glTranslatef(3,.6,-7+movd);
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

void draw_sideviw()//black patch drawn in middle of road
{
    int mov;
    if(movd)
        mov=3;
    else
        mov=0;
    glPushMatrix();
    glTranslatef(-1.5,1.0,-7+mov);
    glScalef(2,3,1);
    for(int i = 1; i <= 10; i++)
    {
        // glColor3f(1, 1, 0);
        glTranslatef(0,0,4);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,9);
        glPushMatrix();
        draw_solid_cube(1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

        glTranslatef(5,0,0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,9);
        glPushMatrix();
        draw_solid_cube(1);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        glTranslatef(-5,0,0);
    }
    glPopMatrix();
}

void draw_car(float posx,float posz,int h=0,int model=2)
{
    glPushMatrix();
    glColor3f(.5, .5, .5);
    glTranslatef(2+posx,1,9+posz);
    glScalef(.05,.05,.05);
    glRotatef(90,0,1,0);

    glutSolidTorus(1.1,1.2,10,10);
    glTranslatef(0,0,9);
    glutSolidTorus(1.1,1.2,10,10);
    glTranslatef(-15,0,-8);
    glutSolidTorus(1.1,1.2,10,10);
    glTranslatef(0,0,9);
    glutSolidTorus(1.1,1.2,10,10);
    glColor3f(1,1,1);
    glTranslatef(6,4,-5);
    glScalef(3,1,1.5);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,model);
    glPushMatrix();
    draw_solid_cube(6);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    if(h>0)
    {
        glColor3f(1, 1, 1);
        glTranslatef(-3,-1,-1.5);
        glScalef(1,1,1);
        glutSolidSphere(1,10,10);
        glTranslatef(0,0,3);
        glutSolidSphere(1,10,10);
    }
    else
    {
        glColor3f(1, 0, 0);
        glTranslatef(-3,-1,-1.5);
        glScalef(1,1,1);
        glutSolidCube(1);
        glTranslatef(0,0,3);
        glutSolidCube(1);
    }

    glPopMatrix();
}

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

    if(!collisions)
    {
        len1p++;
        len2p++;
        len3p++;
        len4p++;
    }

    glPushMatrix();

    draw_car(len,len1p,1,3);
    draw_car(len-1,len2p,2,4);
    draw_car(len+1.5,len3p,3,5);
    draw_car(len+2.5,len4p,4,2);
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
    score+=5;
    GameStatus();
    draw_road();
    draw_divider();
    draw_footpath();
    draw_sideviw();
    draw_car(posx,posz);
    op_car(0);

    if(!collisions)
        movd=!movd;


    if(collision())
    {
        collisions = true;
    }

    Sleep(100);
    glFlush();
    glutSwapBuffers();

}


void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 'j':
        if(posx>=-1)
            posx=posx-0.1;
        cout<<posx<<endl;
        break;
    case 'k':
        if(posx<=2.5)
            posx=posx+0.1;
        cout<<posx<<endl;
        break;

    case 't':

        break;
    case 'g':
        break;

    case 27:	// Escape key
        exit(1);
    }
}

void animate()
{
    glutPostRedisplay();

}
int main (int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(0,0);
    glutInitWindowSize(800,900);
    glutCreateWindow("myRoom");

    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_NORMALIZE);

    glutKeyboardFunc(myKeyboardFunc);

    glutDisplayFunc(display);

    glutIdleFunc(animate);

    glEnable(GL_NORMALIZE);



    glGenTextures(10,Textures);

    Image_Data* Bitmap_footpath = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\myGame\\texture\\foot.bmp", Bitmap_footpath) == 0)
    {
        return 0;
    }

    Image_Data* Bitmap_road = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\myGame\\texture\\road.bmp", Bitmap_road) == 0)
    {
        return 0;
    }

    Image_Data* Bitmap_car = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\myGame\\texture\\car.bmp", Bitmap_car) == 0)
    {
        return 0;
    }

    Image_Data* Bitmap_building = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\myGame\\texture\\building.bmp", Bitmap_building) == 0)
    {
        return 0;
    }

    Image_Data* Bitmap_car1 = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\myGame\\texture\\car1.bmp", Bitmap_car1) == 0)
    {
        return 0;
    }


    Image_Data* Bitmap_car2 = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\myGame\\texture\\car2.bmp", Bitmap_car2) == 0)
    {
        return 0;
    }

    Image_Data* Bitmap_car3 = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\myGame\\texture\\car3.bmp", Bitmap_car3) == 0)
    {
        return 0;
    }

    Image_Data* Bitmap_wheel = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\myGame\\texture\\wheel.bmp", Bitmap_wheel) == 0)
    {
        return 0;
    }

    Image_Data* Bitmap_hlight= (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\myGame\\texture\\hlight.bmp", Bitmap_hlight) == 0)
    {
        return 0;
    }

    Image_Data* Bitmap_table = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\Lab4\\myRoom-Light-Tex\\table.bmp", Bitmap_table) == 0)
    {
        return 0;
    }

    Add_Texture(0,Bitmap_footpath);
    Add_Texture(1,Bitmap_road);
    Add_Texture(2,Bitmap_car);
    Add_Texture(3,Bitmap_car1);
    Add_Texture(4,Bitmap_car2);
    Add_Texture(5,Bitmap_car3);
    Add_Texture(6,Bitmap_wheel);
    Add_Texture(7,Bitmap_hlight);
    Add_Texture(8,Bitmap_table);
    Add_Texture(9,Bitmap_building);

    glutMainLoop();

    return 0;
}
