#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include<stdio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include<iostream>
#ifndef GL_BGR
#define GL_BGR 0x80E0
#endif
using namespace std;

bool movd=false,collisions=false;
float posx=0.8,posz=0.5;
int billm=0;
int score=0;
int scoreinc=5;
int speed=500;

int run=0;
int len1p=0,len2p=-80,len3p=-60,len4p=-40;

int rh=5,rw=5,rl=7;
float ex=3.5,ey=3,ez=13,lx=3.5,ly=1,lz=1;

int l1=25;

void reset()
{
    movd=false,collisions=false;
    posx=0.8,posz=0.5;
    billm=0;
    score=0;
    scoreinc=5;
    speed=500;

    run=0;
    len1p=-10,len2p=-18,len3p=-14,len4p=-22;
}

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

GLuint Textures[20];

void draw_solid_cube(GLfloat size,int rep1x=1,int rep1y=1,int rep2x=1,int rep2y=1,int rep3x=1,int rep3y=1,int rep4x=1,int rep4y=1,int rep5x=1,int rep5y=1,int rep6x=1,int rep6y=1)
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


    float n1_x = 0, n1_y = -1, n1_z = 0; //bottom
    float n2_x = 0, n2_y = 1, n2_z = 0; //up
    float n3_x = 0, n3_y = 0, n3_z = -1; //front
    float n4_x = 0, n4_y = 0, n4_z = 1; //back
    float n5_x = 1, n5_y = 0, n5_z = 0; //right
    float n6_x = -1, n6_y = 0, n6_z = 0; //left

    glBegin(GL_QUADS);
    //Quad 1

    glTexCoord2f(0,0);
    glVertex3f(x1,y1,z1);

    glTexCoord2f(rep1x,0);
    glVertex3f(x2,y2,z2);

    glTexCoord2f(rep1x,rep1y);
    glVertex3f(x3,y3,z3);

    glTexCoord2f(0,rep1y);
    glVertex3f(x4,y4,z4);

    //Quad 2

    glTexCoord2f(0,0);
    glVertex3f(x8,y8,z8);

    glTexCoord2f(rep2x,0);
    glVertex3f(x7,y7,z7);

    glTexCoord2f(rep2x,rep2y);
    glVertex3f(x6,y6,z6);

    glTexCoord2f(0,rep2y);
    glVertex3f(x5,y5,z5);
    //Quad 3

    glTexCoord2f(0,0);
    glVertex3f(x5,y5,z5);

    glTexCoord2f(rep3x,0);
    glVertex3f(x6,y6,z6);

    glTexCoord2f(rep3x,rep3y);
    glVertex3f(x2,y2,z2);

    glTexCoord2f(0,rep3y);
    glVertex3f(x1,y1,z1);
    //Quad 4

    glTexCoord2f(0,0);
    glVertex3f(x7,y7,z7);

    glTexCoord2f(rep4x,0);
    glVertex3f(x8,y8,z8);

    glTexCoord2f(rep4x,rep4y);
    glVertex3f(x4,y4,z4);

    glTexCoord2f(0,rep4y);
    glVertex3f(x3,y3,z3);
    //Quad 5
    glTexCoord2f(0,0);
    glVertex3f(x6,y6,z6);

    glTexCoord2f(rep5x,0);
    glVertex3f(x7,y7,z7);

    glTexCoord2f(rep5x,rep5y);
    glVertex3f(x3,y3,z3);

    glTexCoord2f(0,rep5y);
    glVertex3f(x2,y2,z2);

    //Quad 6
    glTexCoord2f(0,0);
    glVertex3f(x1,y1,z1);

    glTexCoord2f(rep6x,0);
    glVertex3f(x4,y4,z4);

    glTexCoord2f(rep6x,rep6y);
    glVertex3f(x8,y8,z8);

    glTexCoord2f(0,rep6y);
    glVertex3f(x5,y5,z5);

    glEnd();
}

void Write(char *string) //Write string on the screen
{
    while(*string)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *string++);
}

bool collision()

{
    if(posx>=-.7&&posx<=0.7)
        if((len1p>=0&&len1p<=2)||(len1p>=-3*l1&&len1p<=-3*l1+3)||(len1p>=-2*l1&&len1p<=-2*l1+3)||(len1p>=-1*l1&&len1p<=-1*l1+3))
            return true;
    if(posx<=-0.3)
        if((len2p>=0&&len2p<=2)||(len2p>=-3*l1&&len2p<=-3*l1+3)||(len2p>=-2*l1&&len2p<=-2*l1+3)||(len2p>=-1*l1&&len2p<=-1*l1+3))
            return true;
    if(posx>=.8&&posx<=2.2)
        if((len3p>=0&&len3p<=2)||(len3p>=-2*l1&&len3p<=-3*l1+3)||(len3p>=-2*l1&&len3p<=-2*l1+3)||(len3p>=-1*l1&&len3p<=-1*l1+3))
            return true;
    if(posx>=1.8)
        if((len4p>=0&&len4p<=2)||(len1p>=-2*l1&&len4p<=-3*l1+3)||(len4p>=-2*l1&&len4p<=-2*l1+3)||(len4p>=-1*l1&&len4p<=-1*l1+3))
            return true;
    return false;
}

void Score()
{

    char tmp_str[40];

    // Print the status of the game on the screen
    glColor3f(1, 0, 0);
    glRasterPos2f(8.5, 8.0);

    if(!collisions)
    {
        sprintf(tmp_str, "Score: %d %d %f", score,collision(),posx);
        Write(tmp_str);
        glRasterPos2f(8.5, 7.5);
        sprintf(tmp_str, "Speed %d Kmh",(500-speed)*2+10);
        Write(tmp_str);
        glRasterPos2f(8.5, 7.0);
        sprintf(tmp_str, "Multiplier %d",scoreinc);
        Write(tmp_str);
    }
    else
    {
        glRasterPos2f(2, 5.0);
        glColor3f(1, 0, 0);
        sprintf(tmp_str, "Game over");
        Write(tmp_str);
        glRasterPos2f(2, 4.0);
        sprintf(tmp_str, "Your Score is %d",score);
        Write(tmp_str);
    }
}

void display_score()
{
    glPushMatrix();
    glColor3f(0,1,1);
    glTranslatef(9.8,7.8,-1);
    glScalef(1.5,1,0.01);
    glutSolidCube(2);
    glPopMatrix();
}



void draw_road()
{
    glPushMatrix();
    glColor3f(.2, .2, .2);
    glTranslatef(3,0,-44);
    glScalef(6,1.0,100);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,1);
    glPushMatrix();
    draw_solid_cube(1.1,5,2);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glTranslatef(-.4,.6,0);
    glScalef(.01,.1,2);
    glColor3f(1,1,0);
    glutSolidCube(1);
    glTranslatef(88,.6,2);
    glScalef(.4,.1,1);
    glColor3f(1,1,0);
    glutSolidCube(5);
    glPopMatrix();
}

void draw_sky()
{
    glPushMatrix();
    glColor3f(1,1,1);
    glTranslatef(3,20,-86);
    glScalef(128,90,1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,8);
    glPushMatrix();
    draw_solid_cube(1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void draw_footpath()
{
    int mov;
    if(movd)
        mov=10;
    else
        mov=0;
    glPushMatrix();
    glTranslatef(0,0.5,-50+mov);
    glScalef(0.5,.5,100);
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);
    glPushMatrix();
    draw_solid_cube(1.2,1,1,1,100,1,1,1,1,100,2);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glTranslatef(13,0,0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);
    glPushMatrix();
    draw_solid_cube(1.2,1,1,1,100,1,1,1,1,1,1,100,2);
    glPopMatrix();
    glScalef(1/0.5,1,1/35);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void draw_divider()//black patch drawn in middle of road
{
    glPushMatrix();
    glTranslatef(3,.6,-100+movd);
    glScalef(0.2,0.01,1);
    for(int i = 1; i <= 100; i++)
    {
        glColor3f(1, 1, 1);
        glTranslatef(0,0,2);
        glutSolidCube(.5);
        glTranslatef(0,0,2);
        glutSolidCube(.5);
    }
    glPopMatrix();
}

void draw_billboard()
{
    if(!collisions)
    {
        if(billm>48)
            billm=0;
        else
            billm+=2;
    }

    glPushMatrix();
    glTranslatef(6.5,2,-100+billm);
    for(int i = 1; i <= 20; i++)
    {
        glColor3f(1, 1, 1);
        glScalef(.01,1,.02);
        glutSolidCube(4.5);
        glScalef(1/0.01,1,1/.02);
        glTranslatef(0,3,0);
        glScalef(1,1,0.01);

        glEnable(GL_TEXTURE_2D);
        if(i%2==0)
            glBindTexture(GL_TEXTURE_2D,10);
        else
            glBindTexture(GL_TEXTURE_2D,11);
        glPushMatrix();
        draw_solid_cube(1.5);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);


        glScalef(1,1,1/0.01);

        glTranslated(-6.5,-3,0);

        glColor3f(1,1, 1);
        glScalef(.01,1,.02);
        glutSolidCube(4.5);
        glScalef(1/0.01,1,1/.02);
        glTranslatef(0,3,0);
        glScalef(1,1,0.01);
        glEnable(GL_TEXTURE_2D);
        if(i%2==0)
            glBindTexture(GL_TEXTURE_2D,12);
        else
            glBindTexture(GL_TEXTURE_2D,13);
        glPushMatrix();
        draw_solid_cube(1.5);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        glScalef(1,1,1/0.01);

        glTranslated(6.5,0,0);
        glTranslated(0,-3,20);
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
    glTranslatef(-1.5,2.5,-20+mov);
    glScalef(2,5,2);
    for(int i = 1; i <= 10; i++)
    {
        // glColor3f(1, 1, 0);
        glTranslatef(0,0,3);
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
        glRotatef(180,0,1,0);
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
    glScalef(.04,.05,.05);
    glRotatef(90,0,1,0);

    glutSolidTorus(1.1,1.2,10,10);
    glTranslatef(0,0,18);
    glutSolidTorus(1.1,1.2,10,10);

    glTranslatef(-15,0,-18);

    glutSolidTorus(1.1,1.2,10,10);
    glTranslatef(0,0,18);
    glutSolidTorus(1.1,1.2,10,10);

    glColor3f(1,1,1);
    glTranslatef(6,10,-9);
    glScalef(3.5,3,3);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,model);
    glPushMatrix();
    draw_solid_cube(6);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    if(h>0)
    {
        glColor3f(1, 1, 1);
        glTranslatef(-3,-2,-1.5);
        glScalef(1,1,1);
        glutSolidSphere(1,10,10);
        glTranslatef(0,0,3.2);
        glutSolidSphere(1,10,10);
    }
    else
    {
        glColor3f(1, 0, 0);
        glTranslatef(-3,-2,-1.5);
        glScalef(1,1,1);
        glutSolidCube(1);
        glTranslatef(0,0,3.2);
        glutSolidCube(1);
    }

    glPopMatrix();
}
float len1,len2,len3,len4;
void op_car(float len)
{
    if(len1p>0)
    {
        len1p=-100;
    }
    if(len2p>0)
    {
        len2p=-100;
    }

    if(len3p>0)
    {
        len3p=-100;
    }
    if(len4p>0)
    {
        len4p=-100;
    }

    if(!collisions)
    {
        len1p++;
        len2p++;
        len3p++;
        len4p++;
    }

    len1,len2,len3,len4=len;


//    srand((unsigned)time(0));
//    int ran;
//    ran = (rand()%4)+1;
//    if(ran==1)
//        len1-=1.0;

    glPushMatrix();

    draw_car(len1,len1p,1,5);
    draw_car(len1,len1p+l1,1,4);
    draw_car(len1,len1p+l1*2,1,3);
    draw_car(len1,len1p+l1*3,1,5);

    draw_car(len2-1,len2p,2,4);
    draw_car(len2-1,len2p+l1,2,5);
    draw_car(len2-1,len2p+l1*2,2,3);
    draw_car(len2-1,len2p+l1*3,2,4);

    draw_car(len3+1.5,len3p,3,5);
    draw_car(len3+1.5,len3p+l1,3,4);
    draw_car(len3+1.5,len3p+l1*2,3,3);
    draw_car(len3+1.5,len3p+l1*3,3,5);

    draw_car(len4+2.5,len4p,4,2);
    draw_car(len4+2.5,len4p+l1,4,3);
    draw_car(len4+2.5,len4p+l1*2,4,4);
    draw_car(len4+2.5,len4p+l1*3,4,5);

    cout<<len1p<<" "<<len2p<<" "<<len3p<<" "<<len4p<<endl;
    glPopMatrix();
}

void play_game()
{
    Score();
    draw_road();
    draw_sky();
    draw_billboard();
    draw_divider();
    draw_footpath();
    draw_sideviw();
    draw_car(posx,posz);
    op_car(0);
    display_score();


    if(collision())
    {
        //collisions = true;
    }

    if(!collisions)
    {
        score+=scoreinc;
        movd=!movd;
    }

    if(score%500==0)
    {
        if(speed>0)
            speed-=25;
        scoreinc+=5;
    }


    Sleep(speed);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode( GL_PROJECTION );
    glClearColor(0,0.2,0,1);
    glLoadIdentity();
    //glFrustum(-5,5,-5,5, 4, 50);
    gluPerspective(65,1,0.01,100);
    //glOrtho(-5,5,-5,5, 4, 50);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    gluLookAt(ex,ey,ez, lx,ly,lz, 0,1,0);
    //glViewport(0,0,1366,768);
    if(!collisions)
        play_game();
    else
        Score();


    glutPostRedisplay();
    //glFlush();
    glutSwapBuffers();

}


void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 'j':
        if(posx>=-.8)
            posx=posx-0.1;
        cout<<posx<<endl;
        break;
    case 'k':
        if(posx<=2.8)
            posx=posx+0.1;
        cout<<posx<<endl;
        break;
    case 'g':
        reset();
        glutPostRedisplay();
        break;

    case 27:	// Escape key
        exit(1);
    }
}

void animate()
{


}
int main (int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(0,0);
    //glutInitWindowSize(1366,768);
    glutInitWindowSize(1000,900);
    glutCreateWindow("myRoom");

    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_NORMALIZE);

    glutKeyboardFunc(myKeyboardFunc);

    glutDisplayFunc(display);

    glGenTextures(20,Textures);

    Image_Data* Bitmap_footpath = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\myGame\\texture\\footpath.bmp", Bitmap_footpath) == 0)
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

    Image_Data* Bitmap_bill1 = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\myGame\\texture\\bill1.bmp", Bitmap_bill1) == 0)
    {
        return 0;
    }

    Image_Data* Bitmap_bill2 = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\myGame\\texture\\bill2.bmp", Bitmap_bill2) == 0)
    {
        return 0;
    }

    Image_Data* Bitmap_bill3 = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\myGame\\texture\\bill3.bmp", Bitmap_bill3) == 0)
    {
        return 0;
    }


    Image_Data* Bitmap_bill4 = (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\myGame\\texture\\bill4.bmp", Bitmap_bill4) == 0)
    {
        return 0;
    }



    Image_Data* Bitmap_hlight= (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\myGame\\texture\\hlight.bmp", Bitmap_hlight) == 0)
    {
        return 0;
    }

    Image_Data* Bitmap_sky= (Image_Data*) malloc(sizeof(Image_Data));
    if (Load_Bitmap("E:\\Study\\Slide\\CSE 4-2\\CSE-4208\\myGame\\texture\\sky.bmp", Bitmap_sky) == 0)
    {
        return 0;
    }

    Add_Texture(0,Bitmap_footpath);
    Add_Texture(1,Bitmap_road);
    Add_Texture(2,Bitmap_car);
    Add_Texture(3,Bitmap_car1);
    Add_Texture(4,Bitmap_car2);
    Add_Texture(5,Bitmap_car3);

    Add_Texture(7,Bitmap_hlight);
    Add_Texture(8,Bitmap_sky);
    Add_Texture(9,Bitmap_building);

    Add_Texture(10,Bitmap_bill1);
    Add_Texture(11,Bitmap_bill2);
    Add_Texture(12,Bitmap_bill3);
    Add_Texture(13,Bitmap_bill4);

    glutMainLoop();

    return 0;
}
