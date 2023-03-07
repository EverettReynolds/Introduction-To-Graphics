#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
using namespace std;

const int WINDOW_HEIGHT = 500;
const int WINDOW_WIDTH = 500;
static int DRAWAXIS = 0;
static int DRAWWIRE = 0;
static int DRAWFILL = 1;
static int PERSPECTIVEVIEW = 0;
static int ORTHOVIEW = 0;
static int CUSTOMVIEW = 0;
static int SIGNSWITCH = 0;
static float spinX = 0.0;
static float spinY = 0.0;
static float spinZ = 0.0;
static float angleX = 0.0;
static float angleY = 0.0;
static float angleZ = 0.0;
static float camAngle = 0.0;
static float camPosX = -3.0;
static float camPosY = -6.0;
static float camPosZ = 6.0;
static float camPosXSave = -3.0;
static float camPosYSave = -6.0;
static float camPosZSave = 6.0;
//static float camPosXSave = -10.0;
//static float camPosYSave = 5.0;
//static float camPosZSave = 10.0;

typedef struct vertex {
        float x;
        float y;
        float z;
        float w;
} vertex;

typedef struct colortype {
	float red;
	float green;
	float blue;
} colortype;

typedef struct box {
	vertex point[4];
	colortype color;	
} box;

typedef struct pmid{ //pmid = pyramid
   vertex point[3];
   colortype color;
} pmid;


void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   //glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
  // glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
   //glOrtho   (-10.0, 10.0, -10.0, 10.0, 1.0, 20.0);
       
   // glMatrixMode (GL_MODELVIEW);

     //glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(40.0, (GLfloat) w/(GLfloat) h, 1.0, 40.0);
   glMatrixMode(GL_MODELVIEW);
}
  void drawString(GLuint x, GLuint y, void *font, const char* string)
{
      const char *c;
      glRasterPos2i(x, y);
      for(c=string; *c!='\0'; c++)
      glutBitmapCharacter(font, *c);
}
  
void drawAxes(int length) //axes labeling & lines code gotten from Dr.Pounds
{   
     int i, len;

    // Set up some character arrays for axis labels
    char xaxis[] = "X"; 
    char yaxis[] = "Y";
    char zaxis[] = "Z";

    // Select the Stroke font we want to use
    void *font = GLUT_STROKE_ROMAN;
   
               /* This provides a coordinate axis about the origin. */
   glPointSize(1.0);
   glBegin(GL_LINES);
      glVertex3i(0,length,0);
      glVertex3i(0,-length,0);
   glEnd();
   glBegin(GL_LINES);
      glVertex3i(length,0,0);
      glVertex3i(-length,0,0);
   glEnd();
   glBegin(GL_LINES);
      glVertex3i(0,0,length);
      glVertex3i(0,0,-length);
   glEnd();

   glPushMatrix();
    glTranslatef(-0.2, 0.2, 4);
    glRotatef(90.0, 0.0, 0.0, 1.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glScalef(0.002,0.002,0.002);
    len = (int) strlen(zaxis);
    for (i = 0; i < len; i++) {
        glutStrokeCharacter(font, zaxis[i]);
    }
    glPopMatrix();

     // Y - Label
    glPushMatrix();
    glTranslatef(0.0, 4, -.4);
    glRotatef(90.0, 0.0, 0.0, 1.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glScalef(0.002,0.002,0.002);
    len = (int) strlen(yaxis);
    for (i = 0; i < len; i++) {
        glutStrokeCharacter(font, yaxis[i]);
    }
    glPopMatrix();

    // X - Label 
    glPushMatrix();
    glTranslatef(4.0, 0.0, -.4);
    glRotatef(90.0, 0.0, 0.0, 1.0);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    glScalef(0.002,0.002,0.002);
    len = (int) strlen(xaxis);
    for (i = 0; i < len; i++) {
        glutStrokeCharacter(font, xaxis[i]);
    }
    glPopMatrix();

    glutPostRedisplay();
}
void defineBox(struct box *face)
{
   face[0].point[0].x = -1.0;  // Bottom
    face[0].point[0].y = -1.0;
	face[0].point[0].z =  0.0;
	face[0].point[0].w =  1.0;
	face[0].point[1].x = -1.0;
    face[0].point[1].y =  1.0;
	face[0].point[1].z =  0.0;
	face[0].point[1].w =  1.0;
	face[0].point[2].x =  1.0;
    face[0].point[2].y =  1.0;
	face[0].point[2].z =  0.0;
	face[0].point[2].w =  1.0;
	face[0].point[3].x =  1.0;
    face[0].point[3].y = -1.0;
	face[0].point[3].z =  0.0;
	face[0].point[3].w =  1.0;

	face[1].point[0].x = -1.0;  // Left Side
    face[1].point[0].y = -1.0;
	face[1].point[0].z =  0.0;
	face[1].point[0].w =  1.0;
	face[1].point[1].x = -1.0;
    face[1].point[1].y = -1.0;
	face[1].point[1].z =  1.0;
	face[1].point[1].w =  1.0;
	face[1].point[2].x =  1.0;
    face[1].point[2].y = -1.0;
	face[1].point[2].z =  1.0;
	face[1].point[2].w =  1.0;
	face[1].point[3].x =  1.0;
    face[1].point[3].y = -1.0;
	face[1].point[3].z =  0.0;
	face[1].point[3].w =  1.0;

	face[2].point[0].x = -1.0;  // Right Side 
    face[2].point[0].y =  1.0;
	face[2].point[0].z =  0.0;
	face[2].point[0].w =  1.0;
	face[2].point[1].x = -1.0;
    face[2].point[1].y =  1.0;
	face[2].point[1].z =  1.0;
	face[2].point[1].w =  1.0;
	face[2].point[2].x =  1.0;
    face[2].point[2].y =  1.0;
	face[2].point[2].z =  1.0;
	face[2].point[2].w =  1.0;
	face[2].point[3].x =  1.0;
    face[2].point[3].y =  1.0;
	face[2].point[3].z =  0.0;
	face[2].point[3].w =  1.0;
	
	face[3].point[0].x = -1.0;  // Back Side 
    face[3].point[0].y = -1.0;
	face[3].point[0].z =  0.0;
	face[3].point[0].w =  1.0;
	face[3].point[1].x = -1.0;
    face[3].point[1].y = -1.0;
	face[3].point[1].z =  1.0;
	face[3].point[1].w =  1.0;
	face[3].point[2].x = -1.0;
    face[3].point[2].y =  1.0;
	face[3].point[2].z =  1.0;
	face[3].point[2].w =  1.0;
	face[3].point[3].x = -1.0;
    face[3].point[3].y =  1.0;
	face[3].point[3].z =  0.0;
	face[3].point[3].w =  1.0;

	face[4].point[0].x =  1.0;  // Front Side 
    face[4].point[0].y = -1.0;
	face[4].point[0].z =  0.0;
	face[4].point[0].w =  1.0;
	face[4].point[1].x =  1.0;
    face[4].point[1].y = -1.0;
	face[4].point[1].z =  1.0;
	face[4].point[1].w =  1.0;
	face[4].point[2].x =  1.0;
    face[4].point[2].y =  1.0;
	face[4].point[2].z =  1.0;
	face[4].point[2].w =  1.0;
	face[4].point[3].x =  1.0;
    face[4].point[3].y =  1.0;
	face[4].point[3].z =  0.0;
	face[4].point[3].w =  1.0;

	face[5].point[0].x = -1.0;  // Top 
    face[5].point[0].y = -1.0;
	face[5].point[0].z =  1.0;
	face[5].point[0].w =  1.0;
	face[5].point[1].x = -1.0;
    face[5].point[1].y =  1.0;
	face[5].point[1].z =  1.0;
	face[5].point[1].w =  1.0;
	face[5].point[2].x =  1.0;
    face[5].point[2].y =  1.0;
	face[5].point[2].z =  1.0;
	face[5].point[2].w =  1.0;
	face[5].point[3].x =  1.0;
    face[5].point[3].y = -1.0;
	face[5].point[3].z =  1.0;
	face[5].point[3].w =  1.0; 
    
   

// Define the colors
//
	face[0].color.red   = 1.0;
	face[0].color.green = 0.7;
	face[0].color.blue  = 1.0;
	
	face[1].color.red   = 0.4;
	face[1].color.green = 0.8;
	face[1].color.blue  = 0.8;
	
	face[2].color.red   = 1.0;
	face[2].color.green = 0.0;
	face[2].color.blue  = 0.0;

	face[3].color.red   = 0.0;
	face[3].color.green = 1.0;
	face[3].color.blue  = 0.0;

	face[4].color.red   = 0.0;
	face[4].color.green = 0.0;
	face[4].color.blue  = 11.0;

	face[5].color.red   = 1.0;
	face[5].color.green = 1.0;
	face[5].color.blue  = 1.0;
	
}

void definePyramid(struct pmid *face) //defining our pyramid roof
{
    face[0].point[0].x = 0.0;  // Bottom
    face[0].point[0].y = 0.0;
	face[0].point[0].z =  2.0;
	face[0].point[0].w =  1.0;
	face[0].point[1].x = -1.0;
    face[0].point[1].y =  -1.0;
	face[0].point[1].z =  1.0;
	face[0].point[1].w =  1.0;
	face[0].point[2].x =  1.0;
    face[0].point[2].y =  -1.0;
	face[0].point[2].z =  1.0;
	face[0].point[2].w =  1.0;
	//face[0].point[3].x =  1.0;
   // face[0].point[3].y = -1.0;
	//face[0].point[3].z =  0.0;
	//face[0].point[3].w =  1.0;

	face[1].point[0].x = 0.0;  // Left Side
    face[1].point[0].y = 0.0;
	face[1].point[0].z =  2.0;
	face[1].point[0].w =  1.0;
	face[1].point[1].x = -1.0;
    face[1].point[1].y = 1.0;
	face[1].point[1].z =  1.0;
	face[1].point[1].w =  1.0;
	face[1].point[2].x =  1.0;
    face[1].point[2].y =  1.0;
	face[1].point[2].z =  1.0;
	face[1].point[2].w =  1.0;
	//face[1].point[3].x =  1.0;
   // face[1].point[3].y = -1.0;
	//face[1].point[3].z =  0.0;
	//face[1].point[3].w =  1.0;

	face[2].point[0].x = 0.0;  // Right Side 
    face[2].point[0].y =  0.0;
	face[2].point[0].z =  2.0;
	face[2].point[0].w =  1.0;
	face[2].point[1].x = -1.0;
    face[2].point[1].y =  1.0;
	face[2].point[1].z =  1.0;
	face[2].point[1].w =  1.0;
	face[2].point[2].x =  -1.0;
    face[2].point[2].y =  -1.0;
	face[2].point[2].z =  1.0;
	face[2].point[2].w =  1.0;
//	face[2].point[3].x =  1.0;
  //  face[2].point[3].y =  1.0;
	//face[2].point[3].z =  0.0;
//	face[2].point[3].w =  1.0;
	
	face[3].point[0].x = 0.0;  // Back Side 
    face[3].point[0].y = 0.0;
	face[3].point[0].z =  2.0;
	face[3].point[0].w =  1.0;
	face[3].point[1].x = 1.0;
    face[3].point[1].y = -1.0;
	face[3].point[1].z =  1.0;
	face[3].point[1].w =  1.0;
	face[3].point[2].x = 1.0;
    face[3].point[2].y =  1.0;
	face[3].point[2].z =  1.0;
	face[3].point[2].w =  1.0;
//	face[3].point[3].x = -1.0;
  //  face[3].point[3].y =  1.0;
//	face[3].point[3].z =  0.0;
	///face[3].point[3].w =  1.0;

     face[0].color.red   = 1.0;
	face[0].color.green = 0.7;
	face[0].color.blue  = 1.0;
	
	face[1].color.red   = 0.4;
	face[1].color.green = 0.8;
	face[1].color.blue  = 0.8;
	
	face[2].color.red   = 1.0;
	face[2].color.green = 0.0;
	face[2].color.blue  = 0.0;

	face[3].color.red   = 0.0;
	face[3].color.green = 1.0;
	face[3].color.blue  = 0.0;
                              //Pyramid
}

void drawText()
{
    if(!SIGNSWITCH)
    {
        return;
    }
    string txt = "Hello World!";

    glColor3f(0.0,0.0,1.0);
    void *font =  GLUT_STROKE_ROMAN;

    glPushMatrix();
    glTranslatef(-0.7,-0.9,1.3);
    glRotatef(2.0,0.0,1.0,0.0);
    //glRotatef(1.0,0.0,0.0,1.0);
    glRotatef(15.0,1.0,0.0,0.0);
    glScalef(0.002,0.002,0.002);
    for(char ch:txt)
    {
        glutStrokeCharacter(font,ch);
    }
    glPopMatrix();
}

void drawBox( struct box *face ,struct pmid *pyrm)
{
    int i, j;
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
     glPushMatrix();
   glTranslatef(0.0,0.0,0.0);
   glRotatef(spinX,1.0,0.0,0.0);
   glRotatef(spinY,0.0,1.0,0.0);
   glRotatef(spinZ,0.0,0.0,1.0);
 if(DRAWFILL)
 {
   // glPolygonMode(GL_FRONT, GL_FILL);
    //glPolygonMode(GL_BACK, GL_FILL);

    //glPushMatrix();
   // glTranslatef(0.0,0.0,0.0);
   //glRotatef(spinX,1.0,0.0,0.0);
   //glRotatef(spinY,0.0,1.0,0.0);
   //glRotatef(spinZ,0.0,0.0,1.0);

    for(j=0;j<5;j++)
    {

        glColor3f(face[j].color.red,
                  face[j].color.green,
                  face[j].color.blue);

        glBegin(GL_POLYGON);
        for (i=0;i<4;i++)
        {
            glVertex3f(face[j].point[i].x,
                       face[j].point[i].y,
                       face[j].point[i].z);
        }
        glEnd();
    }

   // glPopMatrix();

    //glPushMatrix();
        for(j=0;j<4;j++)
    {

        glColor3f(pyrm[j].color.red,
                  pyrm[j].color.green,
                  pyrm[j].color.blue);

        glBegin(GL_POLYGON);
        for (i=0;i<3;i++)
        {
            glVertex3f(pyrm[j].point[i].x,
                       pyrm[j].point[i].y,
                       pyrm[j].point[i].z);
        }
        glEnd();
    }
    //glPopMatrix();

 } 
 if(DRAWWIRE)
 {
   // glPolygonMode(GL_FRONT, GL_FILL);
   // glPolygonMode(GL_BACK, GL_FILL);

    //glPushMatrix();
    //glTranslatef(0.0,0.0,0.0);
   // glRotatef(spinX,1.0,0.0,0.0);
   //glRotatef(spinY,0.0,1.0,0.0);
   //glRotatef(spinZ,0.0,0.0,1.0);

    for(j=0;j<5;j++)
    {

        glColor3f(face[j].color.red,
                  face[j].color.green,
                  face[j].color.blue);

        glBegin(GL_LINE_LOOP);
        for (i=0;i<4;i++)
        {
            glVertex3f(face[j].point[i].x,
                       face[j].point[i].y,
                       face[j].point[i].z);
        }
        glEnd();
    }

   // glPopMatrix();

    //glPushMatrix();
        for(j=0;j<4;j++)
    {

        glColor3f(pyrm[j].color.red,
                  pyrm[j].color.green,
                  pyrm[j].color.blue);

        glBegin(GL_LINE_LOOP);
        for (i=0;i<3;i++)
        {
            glVertex3f(pyrm[j].point[i].x,
                       pyrm[j].point[i].y,
                       pyrm[j].point[i].z);
        }
        glEnd();
    }
    //glPopMatrix();
 }


   //glPushMatrix();
    drawText();
   glPopMatrix();
   glutPostRedisplay();
}

void drawTopScreenText()
{
    string txt;
    void* font = GLUT_BITMAP_HELVETICA_18;
    int txtHeight = 18;
    int txtWidth = 0;
    //char ch;
    glMatrixMode(GL_MODELVIEW); // general matrix and graphics setup
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,WINDOW_WIDTH,0,WINDOW_HEIGHT);

    if(PERSPECTIVEVIEW == 1)
    {
        txt = "Perspective";
    }
    if(ORTHOVIEW == 1)
    {
        txt = "Orthographic";
    }
    if(CUSTOMVIEW == 1)
    {
        txt = "Custom";
    }

    for(char ch : txt) // used to calculate text thickness
    {
        txtWidth =  txtWidth + glutBitmapWidth(font,ch);
    }
    glColor3f(1.0,1.0,1.0);

    glRasterPos2i(WINDOW_WIDTH - txtWidth, WINDOW_HEIGHT - txtHeight);

    for(char ch : txt) // used to write the string to the screen
    {
        glutBitmapCharacter(font,ch);
    }

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
     glutPostRedisplay();
}

void onAxis(int msg)
{
    switch (msg) {
        case 1:
            DRAWAXIS = 1; 
            break;
        case 2:
            DRAWAXIS = 0; 
            break;
    }
    glutPostRedisplay();
}

void wireOrFill(int msg)
{
    switch (msg) {
        case 1:
            DRAWWIRE = 1;
            DRAWFILL = 0; 
            break;
        case 2:
            DRAWWIRE = 0;
            DRAWFILL = 1;
            break;
    }
    glutPostRedisplay();
}

void perspectiveView()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(67.0,1.0,1.5,20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camPosX, camPosY, camPosZ, 0.0,0.0,0.0,0.0,0.0,1.0);
}
void orthographicView()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10.0,10.0,-10.0,10.0,4.0,20.0);
    //gluPerspective()
    //glOrtho()
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camPosX, camPosY, camPosZ, 0.0,0.0,0.0,0.0,0.0,1.0);
}

void customView()
{
    float left,right,bottom,top,near,far;
    cout << "Enter the Left Parameter." << endl;
    cin >> left;
     cout << "Enter the Right Parameter." << endl;
    cin >> right;
     cout << "Enter the Bottom Parameter." << endl;
    cin >> bottom;
     cout << "Enter the Top Parameter." << endl;
    cin >> top;
     cout << "Enter the Near Parameter." << endl;
    cin >> near;
     cout << "Enter the Far Parameter." << endl;
    cin >> far;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(left,right,bottom,top,near,far);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camPosX, camPosY, camPosZ, 0.0,0.0,0.0,0.0,0.0,1.0);
    //CUSTOMVIEW = 0;
}

void viewChange(int msg)
{
    switch (msg) {
        case 1:
            PERSPECTIVEVIEW = 1;
            ORTHOVIEW = 0;
            CUSTOMVIEW = 0;
            perspectiveView();
            //drawTopScreenText();
            break;
        case 2:
             PERSPECTIVEVIEW = 0;
            ORTHOVIEW = 1;
            CUSTOMVIEW = 0;
            orthographicView();
            //drawTopScreenText();
            break;
        case  3:
            PERSPECTIVEVIEW = 0;
            ORTHOVIEW = 0;
            CUSTOMVIEW = 1;
            customView();
            //drawTopScreenText();
            break;
    }
   // drawTopScreenText();
    glutPostRedisplay();
}

void toggleSign(int msg)
{
    switch (msg) {
        case 1:
            SIGNSWITCH = 1;
            break;
        case 2:
             SIGNSWITCH = 0;
            break;
    }
    glutPostRedisplay();
}
void createMenu()
{
    int menu, axis, W_or_F, views,sign;
    axis = glutCreateMenu(onAxis);
    glutAddMenuEntry("Axis On", 1);
    glutAddMenuEntry("Axis Off", 2);


    W_or_F = glutCreateMenu(wireOrFill);
    glutAddMenuEntry("Wire Shape", 1);
    glutAddMenuEntry("Normal Fill", 2);
    
    
    views= glutCreateMenu(viewChange);
    glutAddMenuEntry("Perspective", 1);
    glutAddMenuEntry("Orthographic", 2);
    glutAddMenuEntry("Custom", 3);

    sign = glutCreateMenu(toggleSign);
    glutAddMenuEntry("Sign On", 1);
    glutAddMenuEntry("Sign Off", 2);

    // And now create the menu 
    menu = glutCreateMenu(0);
    glutAddSubMenu("Axes", axis);
    glutAddSubMenu("Shape Fill", W_or_F);
    glutAddSubMenu("View Change", views);
     glutAddSubMenu("Sign Switch", sign);

    glutAttachMenu(GLUT_MIDDLE_BUTTON);
}
//void SpinDisplay(void) // may need to cap the adding of degrees at 10 degrees
//{
   // SPIN = SPIN + DELTA_SPIN;
    //if (SPIN > 360.0) SPIN = SPIN - 360.0;
    //glutPostRedisplay();
//}

int within(int x, int y) // determines if the mouse is in the viewport , 0 = true , 1 = false
{
   /* if (x > VIEWPORT_MIN && x < VIEWPORT_MAX && y > VIEWPORT_MIN && y < VIEWPORT_MAX)
    {
        return 0;
    }
    else
    {
        return 1;
    } */
    int res = -1;

    if( ( x > y && x < ( WINDOW_WIDTH / 2) && y < ( WINDOW_HEIGHT / 2) ) || ( ( (-1) * x + WINDOW_WIDTH ) + WINDOW_WIDTH > y && y < (WINDOW_HEIGHT / 2) && x > (WINDOW_WIDTH / 2) ) )
    {
        res = 2;
    }
    if(x < y && x < (WINDOW_WIDTH / 2) && y > (WINDOW_HEIGHT / 2))
    {
        res = 1;
    }
    if( x > (WINDOW_WIDTH / 2) && y  > (WINDOW_HEIGHT / 2) && ( ( (-1) * x + WINDOW_WIDTH ) < y ) )
    {
        res = 0;
    }
    return res;

    
}
/*void changeVelocity(float x , float y, bool increment) // initial positions plus whether it is an increment or decrement in speed which is happening
{

}
*/
void SpinDisplay(void)
{
    spinX = spinX + angleX;
    spinY = spinY + angleY;
    spinZ = spinZ + angleZ;

    if(spinX > 360)
    {
        spinX = spinX - 360;
    }
    if(spinY > 360)
    {
        spinY = spinY - 360;
    }
    if(spinZ > 360)
    {
        spinZ = spinZ - 360;
    }
    glutPostRedisplay();
}


void display(void)
{
   
  struct box faces[6];

  struct pmid sides[4];
   
   float *M;
   //box face;
   int i, j;
   

  defineBox(&faces[0]); // point defining function
  definePyramid(&sides[0]);

   //glClear (GL_COLOR_BUFFER_BIT);
   glEnable(GL_DEPTH_TEST);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);

/* ONLY MODIFY CODE BELOW */

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity ();             /* clear the matrix */

   
    //camPosX = -10;
    //camPosY = 5;
    //camPosZ = 10;

   gluLookAt( camPosX,   camPosY, camPosZ,  // Eye
                0.0,   0.0, 0.0,  // Center
                0.0,   0.0, 1.0); // Up
   //camPosXSave = -10;
    //camPosYSave = 5;
    //camPosZSave = 10;
   //             /* DO NOT MODIFY ANYTHING ELSE */
   
   //                /* Draw a coordinate axis */
   
                      glEnable(GL_DEPTH_TEST);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   if (DRAWAXIS)
   {
    drawAxes(5);
   }
   //drawAxes(5);



   drawBox(&faces[0],&sides[0]);
   //char *charString = (char*) malloc(40 * sizeof(char));
    //glColor3f(1.0, 0.0, 0.0);
    //sprintf(charString, "ORTHOGRAPHIC");
    //drawString(15,10,GLUT_BITMAP_HELVETICA_12,charString);
    //free(charString);
     //if(SPIN != 0) // make sure the rotating keeps happening
    //{
        //rotating(SPIN);
    //}
   drawTopScreenText(); // adds the view mode text at the top right of the screen

   glFlush();
   glutSwapBuffers();
   
   }
   
   
//glutSpecialFunc

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
}

void mouse(int button, int state, int x, int y) 
{
    y = WINDOW_HEIGHT-y; //accurate coordinates for y
    //float scaler = 0;
    int det = -1;
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        det = within(x,y);
        if(det == 0)
        {
            angleX = angleX + 0.05;
        }
        if(det == 1)
        {
            angleY = angleY + 0.05;
        }
        if(det == 2)
        {
            angleZ = angleZ + 0.05;
        }
    }
    glutPostRedisplay();
    glutIdleFunc(SpinDisplay);
}

void keyboard( unsigned char key, int x, int y )
{ 
    if ( key == 'q' || key == 'Q') exit(0);
    if(key == 's' || key == 'S')
    {
        angleX = 0.0;
        angleY = 0.0;
        angleZ = 0.0;
        cout << "Animation Stopped" << endl;
    }
    if(key == 'r')
    {
         angleX = 0.0;
        angleY = 0.0;
        angleZ = 0.0;
        spinX = 0.0;
        spinY = 0.0;
        spinZ = 0.0;
        cout << "Animation Stopped, House back to Original Position & Size" << endl;

    }
    if(key == 'R')
    {
        angleX = 0.0;
        angleY = 0.0;
        angleZ = 0.0;
        spinX = 0.0;
        spinY = 0.0;
        spinZ = 0.0;
        camPosX = camPosXSave;
        camPosY = camPosYSave;
        camPosZ = camPosZSave;
        PERSPECTIVEVIEW = 0;
        ORTHOVIEW = 0;
        CUSTOMVIEW = 0;
        cout << "House & Camera Reset" << endl;
    }
    glutPostRedisplay();
}
void specialKeyboard(int key, int x, int y)
{
   // if(!PERSPECTIVEVIEW)
  //  {
     //   return;
    //}
    switch(key)
    {
        case GLUT_KEY_PAGE_UP: //10% camera change
        camPosX = camPosX * 0.9;
        camPosY = camPosY * 0.9;
        camPosZ = camPosZ * 0.9;
        break;
        case GLUT_KEY_PAGE_DOWN:
        camPosX = camPosX * 1.1;
        camPosY = camPosY * 1.1;
        camPosZ = camPosZ * 1.1;
        break;
    }
    glutPostRedisplay();
}

 int main(int argc, char** argv)
 {
    glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DEPTH | GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (WINDOW_HEIGHT, WINDOW_WIDTH);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutReshapeFunc(reshape); 
   glutMouseFunc(mouse);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(specialKeyboard);                                                                              
   createMenu();
   glutDisplayFunc(display);                                                                                    
   glutMainLoop();
	return 0;
}
   
/*

/*glPushMatrix();
    glTranslatef(0.0,-2.0,0.0);

    for(j=0;j<6;j++)
    {

        glColor3f(face[j].color.red,
                  face[j].color.green,
                  face[j].color.blue);

        glBegin(GL_POLYGON);
        for (i=0;i<4;i++)
        {
            glVertex3f(face[j].point[i].x,
                       face[j].point[i].y,
                       face[j].point[i].z);
        }
        glEnd();
    }

    glPopMatrix(); 
    
    // This should be to draw a cylinder 
    /*glPushMatrix();
    glTranslated(2.0,0.0,0.0);    
    glColor3f (0.0,0.0,1.0);
    gluCylinder(gluNewQuadric(), 
            (GLdouble) 0.1,
            (GLdouble) 0.1,
            (GLdouble) 4.0,
            (GLint)    20,
            (GLint)    20 );
    glPopMatrix();

    // This should draw a green doughnut 
    glPushMatrix();
    glRotated(90.0,0,1,0);
    glTranslatef(2.0,0.0,0.0);
    glColor3f (0.0,1.0,0.0);
    gluDisk(gluNewQuadric(), 
            (GLdouble) 0.5,
            (GLdouble) 1.0,
            (GLint)     10,
            (GLint)     20 );
    glPopMatrix();

    // This should draw a red sphere 
    glPushMatrix();
    glTranslatef(-3.0,1.0,1.0);
    glColor3f (1.0,0.0,0.0);
    gluSphere(gluNewQuadric(), 
             (GLdouble) 1.5,
             (GLint)     10,
             (GLint)     10 );
    glPopMatrix();

    // This should draw a purple ellipsoid 
    glPushMatrix();
    glTranslatef(-3.0,1.0,-4.0);
    glScalef(0.5,0.5,1.0);
    glColor3f (1.0,0.0,1.0);
    gluSphere(gluNewQuadric(), 
             (GLdouble) 1.5,
             (GLint)     10,
             (GLint)     10 );
    glPopMatrix(); */
/* face[6].point[0].x = 0.0;  // Bottom
    face[6].point[0].y = 0.0;
	face[6].point[0].z =  3.0;
	face[6].point[0].w =  1.0;
	face[6].point[1].x = -1.0;
    face[6].point[1].y =  -1.0;
	face[6].point[1].z =  2.0;
	face[6].point[1].w =  1.0;
	face[6].point[2].x =  1.0;
    face[6].point[2].y =  -1.0;
	face[6].point[2].z =  2.0;
	face[6].point[2].w =  1.0;
	//face[0].point[3].x =  1.0;
   // face[0].point[3].y = -1.0;
	//face[0].point[3].z =  0.0;
	//face[0].point[3].w =  1.0;

	face[7].point[0].x = 0.0;  // Left Side
    face[7].point[0].y = 0.0;
	face[7].point[0].z =  3.0;
	face[7].point[0].w =  1.0;
	face[7].point[1].x = -1.0;
    face[7].point[1].y = 1.0;
	face[7].point[1].z =  2.0;
	face[7].point[1].w =  1.0;
	face[7].point[2].x =  1.0;
    face[7].point[2].y =  1.0;
	face[7].point[2].z =  2.0;
	face[7].point[2].w =  1.0;
	//face[1].point[3].x =  1.0;
   // face[1].point[3].y = -1.0;
	//face[1].point[3].z =  0.0;
	//face[1].point[3].w =  1.0;

	face[8].point[0].x = 0.0;  // Right Side 
    face[8].point[0].y =  0.0;
	face[8].point[0].z =  3.0;
	face[8].point[0].w =  1.0;
	face[8].point[1].x = -1.0;
    face[8].point[1].y =  1.0;
	face[8].point[1].z =  2.0;
	face[8].point[1].w =  1.0;
	face[8].point[2].x =  -1.0;
    face[8].point[2].y =  -1.0;
	face[8].point[2].z =  2.0;
	face[8].point[2].w =  1.0;
//	face[2].point[3].x =  1.0;
  //  face[2].point[3].y =  1.0;
	//face[2].point[3].z =  0.0;
//	face[2].point[3].w =  1.0;
	
	face[9].point[0].x = 0.0;  // Back Side 
    face[9].point[0].y = 0.0;
	face[9].point[0].z =  3.0;
	face[9].point[0].w =  1.0;
	face[9].point[1].x = 1.0;
    face[9].point[1].y = -1.0;
	face[9].point[1].z =  2.0;
	face[9].point[1].w =  1.0;
	face[9].point[2].x = 1.0;
    face[9].point[2].y =  1.0;
	face[9].point[2].z =  2.0;
	face[9].point[2].w =  1.0;
//	face[3].point[3].x = -1.0;
  //  face[3].point[3].y =  1.0;
//	face[3].point[3].z =  0.0;
	///face[3].point[3].w =  1.0;
                              //Pyramid
*/
// Let's manually move the camera back
   
   //glTranslatef(0.0, 0.0, -10.0);
   
             // Now explicitly place the camera
                
    //gluLookAt (6.0, 5.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
// Let's add another menu item
    //sphere = glutCreateMenu(drawBox);
    //glutAddMenuEntry("On", 1);
    //glutAddMenuEntry("Off", 2);
/* face[6].color.red   = 1.0;
	face[6].color.green = 0.7;
	face[6].color.blue  = 1.0;
	
	face[7].color.red   = 0.4;
	face[7].color.green = 0.8;
	face[7].color.blue  = 0.8;
	
	face[8].color.red   = 1.0;
	face[8].color.green = 0.0;
	face[8].color.blue  = 0.0;

	face[9].color.red   = 0.0;
	face[9].color.green = 1.0;
	face[9].color.blue  = 0.0;
    */