
//gcc tess.cpp -lGL -lGLU -lglut
//g++ tess.cpp -lGL -lGLU -lglut

// An OpenGL Keyboard and Mouse Interaction Program

#include <GL/glut.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include<algorithm>
#include <cmath>

using namespace std;
// These are defined in a global scope

GLubyte red, green, blue;
int COLORS_DEFINED;
int STOP_MOUSE;
int STOP_KEYBOARD;
int clickCount;
int FINAL_LINE;

// Specity the values to place and size the window on the screen

const int WINDOW_POSITION_X = 100;
const int WINDOW_POSITION_Y = 100;
const int WINDOW_MAX_X = 800;
const int WINDOW_MAX_Y = 800;

// Specify the coordinate ranges for the world coordinates in the 2D Frame

const float WORLD_COORDINATE_MIN_X = 0.0;
const float WORLD_COORDINATE_MAX_X = 800.0;
const float WORLD_COORDINATE_MIN_Y = 0.0;
const float WORLD_COORDINATE_MAX_Y = 800.0;

typedef struct dots
{
	int x;
	int y;
}dot;

typedef struct lines
{
	dot p1;
	dot p2;

}line;

typedef struct triangles
{
	dot t1;
	dot t2;
	dot t3;

}tri;



//struct dot pointStore[1];
vector <dot> pointStore(1);
vector <dot> copyStore(1);
vector <line> lineStore(1);
vector <tri> triStore(1);


void myglutInit( int argc, char** argv )
{
    glutInit(&argc,argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); /* default, not needed */
    glutInitWindowSize(WINDOW_MAX_X,WINDOW_MAX_Y); /* set pixel window */
    glutInitWindowPosition(WINDOW_POSITION_X, WINDOW_POSITION_Y); /* place window top left on display */
    glutCreateWindow("Tessalation Simulation"); /* window title */
}


void myInit(void)
{
 
/* standard OpenGL attributes */

      glClearColor(1.0, 1.0, 1.0, 1.0); /* white background */
      glColor3f(1.0, 0.0, 0.0); /* draw in red */
      glPointSize(10.0);

      COLORS_DEFINED = 0;
      STOP_MOUSE = 0;
      STOP_KEYBOARD = 0;
      FINAL_LINE = 0;

/* set up viewing window with origin lower left */

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluOrtho2D(WORLD_COORDINATE_MIN_X, WORLD_COORDINATE_MAX_X,
                 WORLD_COORDINATE_MIN_Y, WORLD_COORDINATE_MAX_Y);
      glMatrixMode(GL_MODELVIEW);
}


void display( void )
{

/* define a point data type */

    typedef GLfloat point[2];     

    point p; /* A point in 2-D space */

    glClear(GL_COLOR_BUFFER_BIT);  /*clear the window */

    if (!COLORS_DEFINED) {
       red   = 255;
       green = 0; 
       blue  = 0;
    }

    glColor3ub( red, green, blue ); 

    /* define point */

    p[0] = 100; 
    p[1] = 100;
   
    /* plot new point */

        /*glBegin(GL_POINTS);
            glVertex2fv(p); 
        glEnd(); */
  
     
     glFlush(); /* clear buffers */

 }

dot lineIntersect(dot p1, dot p2, dot p3, dot p4)
{
  dot intersection; // set to false by default
  float product,denom,scalar1,scalar2;
  float a1,a2,b1,b2,c1,c2;
  float coorX,coorY;
 /* a1 = p2.y - p1.y;
  b1 = p1.x - p2.x;
  c1 = a1*(p1.x) + b1*(p1.y);

  a2 = p4.y - p3.y;
  b2 = p3.x - p4.x;
  c2 = a2*(p3.x) + b2*(p3.y);

  product = (a1*b2) - (a2*b1);

  if(product == 0) // indicates parallel lines
  {
    intersection.x = -1;
    intersection.y = -1;
    return intersection;
  }
  else // indicates intersecting lines
  {
    coorX = ( (b2*c1) - (b1*c2) )/product;
    coorY = ( (a1*c2) - (a2*c1) )/product;
    intersection.x = coorX;
    intersection.y = coorY;
    return intersection;
  } */

  denom = ( ( (p2.x - p1.x) * (p4.y - p3.y) ) - ( (p2.y - p1.y) * (p4.x - p3.x) ) );
  if(denom == 0) // indicates parallel lines
  {
    intersection.x = -1;
    intersection.y = -1;
    return intersection;
  }
  else
  {
    scalar1 = ( (p1.y - p3.y) * (p4.x - p3.x) ) - (  (p1.x - p3.x) * (p4.y - p3.y));
    scalar2 = ( (p1.y - p3.y) * (p2.x - p1.x) ) - (  (p1.x - p3.x) * (p2.y - p1.y));
    scalar1 = scalar1/denom;
    scalar2 = scalar2/denom;
    if( 0 < scalar1 < 1 &&  0 < scalar2 < 1)
    {
      intersection.x = 1;
      intersection.y = 1;
      return intersection;
    }
    else
    {
      intersection.x = -1;
      intersection.y = -1;
      return intersection;
    } 
  }
  
}
  

int diagIntersection(vector<dot> pts, int index)
{
  dot d,ex; 
  d.x = -1;
  d.y = -1;
  ex.x = 0;
  ex.y = 0;
  for(int i  = 0; i < pts.size(); i++)
  {
    if( i == index || i == (index + 2) % pts.size() || index == (i+1) % pts.size() || (index + 2) % pts.size() == (i+1) % pts.size() ) // checks for unique points within our set of lines
    {
        ex = lineIntersect(pts[index], pts[(index+2) % pts.size()] , pts[i], pts[ (i+1) % pts.size() ] );
        continue;
    }
    if(ex.x !=  d.x &&  ex.y != d.y)
    {
      return 1; // returns true if diagonal intersection of line during tessalation is found
    }
  }
  return 0;
}
int clockWise(vector<dot> dot)
{
  int sum,num;
  sum = 0;
  num = dot.size();
  for(int  i = 0; i < num; i++)
  {
    sum = sum + (dot[(i+1) % num].x + dot[i].x) * (dot[(i+1) % num].y + dot[i].y);
  }
  if(sum  > 0)
  {
    return 1; // says polygon is clockwise
  }
  else
  {
    return 0; // says polygon is not clockwise
  }
}
void drawBox( int x, int y )
{
    typedef GLfloat point[2];     
    point p;
    
    glColor3ub( drand48()*255, drand48()*255, drand48()*255 );
   
    p[0] = x;
    p[1] = (WINDOW_MAX_Y-y); // Subtract Max Window Y Coordinate by the mouse coordinate 
 	//add checks here please :) 
	dot pt,checking;
	pt.x = p[0];
	pt.y = p[1];
  if(pointStore.size() > 3)
  {
    for(int i = 0; i < pointStore.size(); i++)
    {
      checking = lineIntersect(pt,pointStore[pointStore.size()-1],pointStore[i],pointStore[i-1]);
      cout << "\tx is equal to " << checking.x;
      cout << "\ty is equal to " << checking.y;
      if(checking.x != -1 && checking.y != -1)
      {
        printf("Intersecting Point Detected");
        //pointStore.erase(i);
        break;
      }
      else
      {
        pointStore[clickCount].x = x;
        pointStore[clickCount].y = (WINDOW_MAX_Y-y);
	      clickCount++;
        cout << "\tDrawing " << x;
        cout << "\tDrawing " << pointStore[clickCount].y;
        glBegin(GL_POINTS);
            glVertex2fv(p); 
        glEnd(); 
        glFlush();
      }
    }
  }
	else
  {
        pointStore[clickCount].x = x;
        pointStore[clickCount].y = (WINDOW_MAX_Y-y);
	      clickCount++;
        cout << "\tDrawing " << x;
        cout << "\tDrawing " << pointStore[clickCount].y;
        glBegin(GL_POINTS);
            glVertex2fv(p); 
        glEnd(); 
        glFlush();
  }
}


void eraseBox( int x, int y )
{
    typedef GLfloat point[2];     
    point p;

    glColor3f( 1.0, 1.0, 1.0 );

    p[0] = x;
    p[1] = (WINDOW_MAX_Y-y);  

    glBegin(GL_POINTS);
      glVertex2fv(p); 
    glEnd();
    glFlush();
}


void clearBox()
{
       glClear(GL_COLOR_BUFFER_BIT); 
       glFlush();
}
float triArea(dot p1, dot p2, dot p3)
{
  int a,b,c,d,e,f;
  float distance1, distance2, distance3, sides, area;
  a = pow((p2.x - p1.x),2);
  b = pow((p2.y - p1.y),2);

  distance1 = sqrt(a+b);

  c = pow((p3.x - p2.x),2);
  d = pow((p3.y - p2.y),2);

  distance2 = sqrt(c+d);

  e = pow((p1.x - p3.x),2);
  f = pow((p1.y - p3.y),2);

  distance3 = sqrt(e+f);

  sides = (distance1+distance2+distance3)/2;

  area = sqrt( sides * (sides-distance1) * (sides-distance2) * (sides-distance3) );


  return area;
}

void drawShape(void)
{ 
  
	int lastPos = 0;
	int length = pointStore.size(); // may change
        glClear (GL_COLOR_BUFFER_BIT);  // Clear display window.

        glColor3f (0.0, 0.4, 0.2);      // Set line segment color to green.
        for(int i = 1; i < length; i++)
        {       						// Need to Pass in 2D Array of Points
			    lastPos = i-1;
				  if(pointStore[i].x == 0 || pointStore[i].y == 0)
				  {
					  continue;
				  }
          if(FINAL_LINE == 1)
				  {	
					  glBegin(GL_LINES);
              glVertex2i(pointStore[0].x,pointStore[0].y);
						  glVertex2i(pointStore[length-1].x,pointStore[length-1].y);
               lineStore[i].p1 = pointStore[length-1];
                lineStore[i].p2 = pointStore[0];
              glBegin(GL_LINE_LOOP);
                glVertex2i(pointStore[i].x,pointStore[i].y);
                glVertex2i(pointStore[lastPos].x,pointStore[lastPos].y);
                lineStore[i].p1 = pointStore[lastPos];
                lineStore[i].p2 = pointStore[i];
					    glEnd();
					  glEnd();
				  }
				  else
				  {
            continue;
				  }  
        }
        glFlush ();     // Process all OpenGL routines as quickly as possible.*/
}
int vertexCheck(vector<dot>dots) // is it CCW or is it not?
{
  int dotSum ,response;
  response = -1;
  for(int i  = 1; i < 4; i++)
  {
    dotSum = (dots[i].x-dots[i-1].x)*(dots[i].y + dots[i-1].y);
  }
  if(dotSum <= 0)
  {
    return response = 1; //interesting syntax i just discovered
  }
  if(dotSum > 1)
  {
    return response = 0;
  }
  if(dotSum == 1)
  {
    return response;
  }
  return response;
}

void tessalate(void)
{
  vector<dot>copy;
  vector<dot>triCheck; // do not destroy your original points
  int count,ccw, triCount;
  dot intersection;
  tri triangles;
  copy = pointStore; 
  count = 0; // oh my bad
  triCount = 0;
  cout << "Checkpoint 1" << endl;
  if(clockWise(copy) != 0) // checks modeled after class slides 
  { 
    cout << "Checkpoint 2" << endl;
    reverse(copy.begin(),copy.end());
  }
  cout << "Checkpoint 3" << endl;
  while (copy.size() > 3)
  {
      cout << "Checkpoint 4" << endl;
    triCheck.push_back(copy[count]); //why is this not possible
    triCheck.push_back(copy[count+1]);
    triCheck.push_back(copy[count+2]);
    cout << "Checkpoint 5" << endl;
    ccw = vertexCheck(copy);
    cout << "CCW Check: " << ccw << endl;
    if(ccw == 0) // is not ccw
    {
      cout << copy[0+count+1].x << endl;
      copy.erase(copy.begin()+(count+1)%copy.size());
      count = 0;
    }
    else if(ccw == 1) // is ccw
    {
      triCount = triCount + count;
      for(int i = 0; i < copy.size(); i++)
      {
        cout << copy[0+count+1].x << endl;
        intersection = lineIntersect(copy[count],copy[count+1],copy[count+2],copy[count+3]);
        cout << intersection.x << endl;
        cout << intersection.y << endl;
        count++;
        if(intersection.x == 1 && intersection.y == 1)
        {
          //count++;
          break;
        }
        if(intersection.x == -1 && intersection.y == -1)
      {
        cout << triCheck[0].x << endl;
        cout << triCheck[1].x << endl;
        cout << triCheck[2].x << endl;
        triangles.t1 = triCheck[0];
        triangles.t2 = triCheck[1];
        triangles.t3 = triCheck[2];
        triStore.push_back(triangles);
        copy.erase(copy.begin()+(count+1)%copy.size());
        count = 0;
      }
      }
      /*if(intersection.x == -1 && intersection.y == -1)
      {
        triangles.t1 = triCheck[0];
        triangles.t2 = triCheck[1];
        triangles.t3 = triCheck[2];
        triStore.push_back(triangles);
        copy.erase(copy.begin()+count+1);
        count = 0;
      } */
    }
    else if(ccw == -1)
    {
        cout << "what" << endl;
    }
  }
  if(copy.size() == 3)
  {
        triangles.t1 = triCheck[0]; // WHY NOT
        triangles.t2 = triCheck[1];
        triangles.t3 = triCheck[2];
        triStore.push_back(triangles);
  }


  
}
void fillTesselation(void)
{
   for(int i = 0; i < triStore.size(); i++) // drawing tesselation
  {
    glBegin(GL_POLYGON);
      glVertex2i(triStore[i].t1.x,triStore[i].t1.y);
			glVertex2i(triStore[i].t2.x,triStore[i].t2.y);
      glVertex2i(triStore[i].t3.x,triStore[i].t3.y);
    glEnd();

    glFlush();
  }
}
void fillShape(void) 
{
  int lastPos = 0;
	int length = pointStore.size();
  glClear (GL_COLOR_BUFFER_BIT);
  glColor3f (0.0, 0.0, 0.0);
  for(int i = 1; i < length; i++)
        {       						// Need to Pass in 2D Array of Points
			    lastPos = i-1;
				  if(pointStore[i].x == 0 || pointStore[i].y == 0)
				  {
					  continue;
				  }
          if(FINAL_LINE == 1)
				  {	
					  glBegin(GL_POLYGON);
              glVertex2i(pointStore[0].x,pointStore[0].y);
						  glVertex2i(pointStore[length-1].x,pointStore[length-1].y);
              glBegin(GL_LINE_LOOP);
                glVertex2i(pointStore[i].x,pointStore[i].y);
                glVertex2i(pointStore[lastPos].x,pointStore[lastPos].y);
					    glEnd();
					  glEnd();
				  }
				  else
				  {
            continue;
				  }  
        }
        glFlush ();     // Process all OpenGL routines as quickly as possible.*/


}
void mouse( int button, int state, int x, int y )
{ 
  dot checker;
  int checkmark;
  checker.x = -1;
  checker.y = -1;
  checkmark = 0;
  if ( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && STOP_MOUSE != 1)
     {
        if(checker.x == -1 && checker.y == -1 && checkmark == 0)
        {
          printf ("%d   %d\n", x, y); //add checks here

          if(clickCount == pointStore.size())
          {
            pointStore.resize(pointStore.size()+1);
          }
          drawBox( x, y );
        }
     }

  if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
     {
        STOP_MOUSE = 1;
        STOP_KEYBOARD = 1;
        FINAL_LINE = 1;
	printf("All Done!");
	
	   for(int i = 0; i < pointStore.size(); i++)
        {
                printf("%d\t%d",pointStore[i].x,pointStore[i].y);
                printf("\n");
        }
	//drawShape();

     }
  
  if ( button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN )
     {
        printf ("%d   %d\n", x, y);
        clearBox();
        for(int i = 0; i < pointStore.size(); i++)
        {
                pointStore[i].x = 0;
                pointStore[i].y = 0;
                pointStore.resize(clickCount-1);
                printf("%d\t%d",pointStore[i].x,pointStore[i].y);
                printf("\n");
        }
        clickCount = 0;
     }
}


void keyboard( unsigned char key, int x, int y )
{ 
  if(STOP_KEYBOARD == 1)
  {
    if ( key == 'q' || key == 'Q') exit(0);
    else if ( key == 'f' || key == 'F') drawShape(); 
    else if ( key == 't' || key == 'T')
    { 
      clearBox();
      tessalate();
      fillTesselation();
    }
    else if ( key == 'p' || key == 'P') 
    {
      clearBox();
      fillShape();
    } 
    else if ( key == 'i' || key == 'I') 
    {
      drawShape();
    }
     // need to add a new method or change to copy of global here
  }
  else
  {
    
  }

}

void lineSegment (void)
{
    glClear (GL_COLOR_BUFFER_BIT);  // Clear display window.
    glColor3f (0.0, 0.4, 0.2);      // Set line segment color to green.
    glFlush ( );     // Process all OpenGL routines as quickly as possible.
}

int main(int argc, char** argv)
{

    myglutInit(argc,argv); /* Set up Window */
    myInit(); /* set attributes */

    // Now start the standard OpenGL glut callbacks //
    
    glutMouseFunc(mouse);  /* Define Mouse Handler */

    glutKeyboardFunc(keyboard); /* Define Keyboard Handler */

    glutDisplayFunc(display);

    glutMainLoop(); /* enter event loop */
}


// Graveyard of Dumb Stupid & Dumb Code
// Will Delete Later
//struct dot pointStore[1];
 	//	glBegin (GL_LINES);
//        glVertex2i (0, 15);       // Specify line-segment geometry.
  //      glVertex2i (200, 300);
   //     glVertex2i(0,50);
     //   glVertex2i(400,500);
   // glEnd ( );
       // glEnd ( );
       //int xStore[20];
  //int yStore[20];
  // glutDisplayFunc(display); /* Display callback invoked when window opened */
  // printf("%d ",pointStore[i]);
  //    xStore[clickCount] = p[0];
  //   yStore[clickCount] = p[1];
   //  clickCount++;
   //if(pointStore[i].x == pointStore[length-1].x ) //change condition & maybe make own method to close shape 
   //glVertex2i(pointStore[0].x,pointStore[0].y);
   //glBegin(GL_LINE_LOOP);
              //glVertex2i(pointStore[i].x,pointStore[i].y);
              //glVertex2i(pointStore[lastPos].x,pointStore[lastPos].y);
					  //glEnd();
            //glVertex2i(pointStore[0].x,pointStore[0].y);
            //  glBegin (GL_LINE_STRIP);
//      	glVertex2i(xStore[i],yStore[i]);
//	if(lastPos == -1)
		//	{
      //	int example = WINDOW_MAX_Y-y;
	    //printf("%d \n", example);
      /*int denom , scalar1, scalar2;
  dot d1 , d2, d3, d4;
  d1 = lineStore[num].p1;
  d2 = lineStore[num].p2;
  d3 = prevPoint;
  d4 = thisPoint;
  denom = ( ( (d2.x - d1.x) * (d4.y - d3.y) ) - ( (d2.y - d1.y) * (d4.x - d3.x) ) );*/
//fill(pointStore.begin(),pointStore.end(),0);
//  printf ("%d   %d\n", x, y);
       // eraseBox( x, y );
				  //printf("%d %d\n", pointStore[i].x,pointStore[length-1].y);
				  //printf("%d %d\n", pointStore[i].x,pointStore[length-1].y);