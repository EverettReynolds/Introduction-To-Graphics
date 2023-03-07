
//Everett Reynolds


#include <math.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <vector>
#include<algorithm>
#include <cmath>
#include <stdio.h>

using namespace std;

#define WINDOW_MAX 1000
#define WINDOW_MIN 0
#define VIEWPORT_MAX 900
#define VIEWPORT_MIN 100
#define MAX 100

/* Define the vertex data type */

typedef struct vertex { 
    float x;
    float y;
    float z = 1;
    float w = 0;
} vertex;

typedef struct polygon { 
   vector <vertex> tree;
} poly;

typedef struct triangles
{
	vertex t1;
    vertex t2;
    vertex t3;

}tri;

/* Define these two variables to have a global scope */
float DELTA_SPIN = 0.0;
float SPIN  = 0.0;
vector <vertex> pointStore;
vector <vertex> trunkStore;
vector <vertex> triStore;
vector <vertex> masterPointStore;
vector <vertex> masterTrunkStore;
vector <polygon> funk;
vertex masterTreeCenter;
vertex treeCenter;
int firstProcess = 1;

void drawBox( int x, int y )
{
    typedef GLfloat point[2];     
    point p;
 
    glColor3ub( drand48()*255, drand48()*255, drand48()*255 );
   
    p[0] = x;
    p[1] = WINDOW_MAX-y; // Subtract Max Window Y Coordinate by the mouse coordinate 
 //    xStore[clickCount] = p[0];
  //   yStore[clickCount] = p[1];
   //  clickCount++;
 	//add checks here please :) 
	vertex pt;
	pt.x = p[0];
	pt.y = p[1];
	//cout << "x:" << pt.x << endl;
    //cout << "y:" << pt.y << endl;
    
        glBegin(GL_POINTS);
            glVertex2fv(p); 
        glEnd();
        glFlush();
}
void vmatm (int SIZE, float *pA, float *pB)

// Matrix-vector multiplication
// pA is a pointer to the first element of matrix A
// pB is a pointer to the first element of vector B
// On return, B will contain transformed coordinates
{
   int i, j;
   float temp[4];

   for (i=0; i<SIZE; i++)
             temp[i] = 0.0;

   for (i=0; i < SIZE; i++)
     for (j = 0; j < SIZE; j++)
           temp[i] += *(pA+(i*SIZE+j)) * *(pB+j);
   
   for (i=0; i<SIZE; i++)
         *(pB+i) = temp[i];

}


void buildTranslate( float x, float y, float z, float *pA )
// Constructs tranlation matrix to translate along x, y, and z axes
{
     pA[ 0] = 1.0; pA[ 1] = 0.0; pA[ 2] = 0.0; pA[ 3] =   x;
     pA[ 4] = 0.0; pA[ 5] = 1.0; pA[ 6] = 0.0; pA[ 7] =   y;
     pA[ 8] = 0.0; pA[ 9] = 0.0; pA[10] = 1.0; pA[11] =   z;
     pA[12] = 0.0; pA[13] = 0.0; pA[14] = 0.0; pA[15] = 1.0;
}

void buildRotateZ( float theta, float *pA )
{
// Constructs rotation matrix about Z axis

     float phi;

     // Convert degrees to radians

     phi = theta * M_PI / 180.0;

     pA[ 0] =  cos(phi); pA[ 1] = sin(phi); pA[ 2] = 0.0; pA[ 3] = 0.0;
     pA[ 4] = -sin(phi); pA[ 5] = cos(phi); pA[ 6] = 0.0; pA[ 7] = 0.0;
     pA[ 8] = 0.0;       pA[ 9] = 0.0;      pA[10] = 1.0; pA[11] = 0.0;
     pA[12] = 0.0;       pA[13] = 0.0;      pA[14] = 0.0; pA[15] = 1.0;
}      

void applyTransformation( float *vp, int vpts, float *TM ) 
// Applies the given transformation matrix TM to the vector vp containing
// all of the homegenous coordinates to define the object
{
	float temp[4];
	float *tmp;
        int i;

	tmp = &temp[0];

	for (i=0;i<vpts;i++)
	{
		*(tmp+0)= *(vp+(i*4)+0);
		*(tmp+1)= *(vp+(i*4)+1);
		*(tmp+2)= *(vp+(i*4)+2);
		*(tmp+3)= *(vp+(i*4)+3);
		vmatm( 4, TM, tmp );
		*(vp+(i*4)+0) = *(tmp+0); 
		*(vp+(i*4)+1) = *(tmp+1); 
		*(vp+(i*4)+2) = *(tmp+2); 
		*(vp+(i*4)+3) = *(tmp+3); 
        }
}

	
void PipeLine( float *vp, int vpts )
{
    /*  This routine will run the graphics transformation pipeline. 
     *  It is operating on x,y,z homogenous coordinates defined in a linear
     *  array.  These are converted to vertex points later for possible
     *  clipping before final display.
     */ 

    /* Set up useful temporary variables */
    float TransformationMatrix[16];
    float *TM;

    /* Don't forget to initialize the ponter! */
    TM = &TransformationMatrix[0];

    // Translate to origin  
    buildTranslate( -WINDOW_MAX/2, -WINDOW_MAX/2, 0.0,  TM );
    applyTransformation( vp, vpts, TM );   	
    // Perform the rotation operation
    buildRotateZ( SPIN, TM );	
    applyTransformation( vp, vpts, TM );
    // Translate back to point
    buildTranslate( WINDOW_MAX/2, WINDOW_MAX/2, 0.0,  TM );
    applyTransformation( vp, vpts, TM );   	

}


void defineArrow( float *apts )
{
   apts[ 0] = 250.0;  apts[ 1] = 300.0; apts[ 2] = 0.0; apts[ 3] = 1.0;
   apts[ 4] = 450.0;  apts[ 5] = 300.0; apts[ 6] = 0.0; apts[ 7] = 1.0;
   apts[ 8] = 550.0;  apts[ 9] = 350.0; apts[10] = 0.0; apts[11] = 1.0;
   apts[12] = 450.0;  apts[13] = 400.0; apts[14] = 0.0; apts[15] = 1.0;
   apts[16] = 250.0;  apts[17] = 400.0; apts[18] = 0.0; apts[19] = 1.0;
}

void toVertex ( float *apts, struct vertex *vp, int pts )
{
	int i;

	for (i=0;i<pts;i++)
	{
	   (vp+i)->x = *(apts+(i*4)+0);
	   (vp+i)->y = *(apts+(i*4)+1);
	   (vp+i)->z = *(apts+(i*4)+2);
	   (vp+i)->w = *(apts+(i+4)+3);
	}
}


void drawArrow( vertex *vp, int points )
{
    int i;

    glBegin(GL_LINE_LOOP);
    for (i=0;i<points;i++)
        glVertex2f( (vp+i)->x, (vp+i)->y );
    glEnd();

}
void symmetryHelp(float iniX, float iniY, float x, float y) // Every combo of inital x/y, + or -, x or y is put into point storage
{
        vertex dot;

        dot.x = iniX + x;
        dot.y = iniY + y;
        pointStore.push_back(dot);
        //cout << " Dot's X: " << (dot.x) << endl;
       // cout << " Dot's Y: " << (dot.y) << endl;
        dot.x = iniX + y;
        dot.y = iniY + x;
        pointStore.push_back(dot);
        //cout << " Dot's X: " << (dot.x) << endl;
       // cout << " Dot's Y: " << (dot.y) << endl;
        dot.x = iniX - y;
        dot.y = iniY + x;
        //pointStore.push_back(dot); // pacman dot
        //cout << " Dot's X: " << (dot.x) << endl;
        //cout << " Dot's Y: " << (dot.y) << endl;
        dot.x = iniX - x;
        dot.y = iniY + y;
       pointStore.push_back(dot);
       // cout << " Dot's X: " << (dot.x) << endl;
        //cout << " Dot's Y: " << (dot.y) << endl;
        dot.x = iniX - x;
        dot.y = iniY - y;
        pointStore.push_back(dot);
        //cout << " Dot's X: " << (dot.x) << endl;
        //cout << " Dot's Y: " << (dot.y) << endl;
        dot.x = iniX - y;
        dot.y = iniY - x;
       // pointStore.push_back(dot);  // pacman dot
        //cout << " Dot's X: " << (dot.x) << endl;
        //cout << " Dot's Y: " << (dot.y) << endl;
        dot.x = iniX + y;
        dot.y = iniY - x;
        pointStore.push_back(dot);
       //cout << " Dot's X: " << (dot.x) << endl;
       // cout << " Dot's Y: " << (dot.y) << endl;
        dot.x = iniX + x;
        dot.y = iniY - y;
        pointStore.push_back(dot);
       //cout << " Dot's X: " << (dot.x) << endl;
       //cout << " Dot's Y: " << (dot.y) << endl;
}
// Textbook Midpoint Circle Algorithm
void midpointCircle() //int radius
{
    float iniX = 730;
    float iniY = 550;
    int radius = 130;
    float x = 0;
    float y = (float) radius;
    float error = 5.0/4.0 - radius;
    vertex dot;
    //save points here
    dot.x = x;
    dot.y = y;
    pointStore.push_back(dot);

    while (y > x)
    {
        x = x + 0.5;
        //y = y + 1;
        if(error <= 0)
        {
           
            error = error + x *2.0 + 3;
            //cout << "x Value: " << x << endl;
            //cout << " Error Value " << error << endl;
        }
        else
        {
            error = error + (x-y)*2.0 - 0.5;
            x++;
            y--;
            //cout << "y Value: " << y<< endl;
            //cout << " Error Value " << error << endl;
        }
        //cout << "x Value: " << x << endl;
        dot.x = x;
        //cout << "y Value: " << y<< endl;
        dot.y = y;
        pointStore.push_back(dot);
        symmetryHelp(iniX,iniY,x,y);
    }
}
void treeLines(float x1,float y1,float x2,float y2)
{
    vertex point1, point2;

    point1.x = x1;
    point1.y = y1;
    point2.x = x2;
    point2.y = y2;

    trunkStore.push_back(point1);
    trunkStore.push_back(point2);
    //cout << point1.x << endl;
   // cout << point1.y << endl;
    //cout << point2.x << endl;
    //cout << point2.y << endl;

}
void treeBounds()
{
    float circleX, circleY;
    circleX = 600.0; // calculated from coords in Midpoint Circle Algorithm
    circleY = 420.0;
   

    //treeLines(630,605,620,615);
    //treeLines(circleX+20,circleY-15,circleX-42,circleY-42);
   // treeLines(circleX+20,circleY+15,VIEWPORT_MIN+100,VIEWPORT_MAX-175);
    //treeLines(circleX+20,circleY-15,VIEWPORT_MIN+100,VIEWPORT_MIN+160);
    //treeLines(VIEWPORT_MAX+100,VIEWPORT_MAX-175,VIEWPORT_MIN+100,VIEWPORT_MIN+160);
   // treeLines(732,450,637,355);
   // treeLines(637,355,730,456);
   // treeLines(730,456,636,541);
    //treeLines(636,541,205,402);
    //treeLines(205,402,206,494);
    //treeLines(206,494,730,456);
    treeLines(635,643,760,574);
    treeLines(635,457,755,541);
    treeLines(760,574,216,617);
    treeLines(755,541,216,476);
    treeLines(216,617,216,476);
    treeCenter.x = 462;
    treeCenter.y = 548;
}
void defineTree()
{
    if(firstProcess == 1)
    {
        midpointCircle();
        treeBounds();
    }

    int size = pointStore.size();

    

    glBegin(GL_POINTS);
    for (int i=0;i<size;i++)
    {
        glColor3f(0.0, 0.0, 0.0);
        
        glVertex2f( pointStore[i].x,pointStore[i].y);
        glVertex2f(treeCenter.x,treeCenter.y); // The point of rotation , make variable
        
    }
    /*for (int i=0;i<size;i++)
    {
        glColor3f(0.0, 0.0, 0.0);
        glVertex2f( pointStore[i].x,pointStore[i].y);
        glVertex2f(pointStore[(i+1)%size].x,pointStore[(i+1)%size].y);
    }*/
    glEnd();

    glBegin(GL_LINES);
    for(int i = 0; i < trunkStore.size(); i++)
    {
        //if(trunkStore[i].x == 0 && trunkStore[i].y == 0)
        //{
           // continue;
       // }
        //else
        //{
            glVertex2f(trunkStore[i].x,trunkStore[i].y);
           // cout << trunkStore[i].x << endl;
           // cout << trunkStore[i].y << endl;
        //}
    }
    glEnd();
}
void reset()
{
    SPIN = 0;
    pointStore.clear();
    pointStore = masterPointStore;
    trunkStore.clear();
    trunkStore = masterTrunkStore;
    treeCenter.x = masterTreeCenter.x;
    treeCenter.y = masterTreeCenter.y;
    
}
vertex translation(float x, float y, vertex matrixVert)
{
    vertex transMat;
    transMat.x = matrixVert.x + (x * matrixVert.z);
    transMat.y = matrixVert.y + (y * matrixVert.z); // when in 3D, add z translation bc default value never changes
    return transMat;
}

void translationApp(float x, float y)
{
    vertex transMat;
    vector<vertex> transVec;

    for(int i  = 0; i < pointStore.size(); i++)
    {
        transMat = translation(x,y,pointStore[i]); //translation process
        transVec.push_back(transMat);   
    }

    pointStore.clear();
    pointStore = transVec; // copying process
    transVec.clear();

    for(int i  = 0; i < trunkStore.size(); i++)
    {
        transMat = translation(x,y,trunkStore[i]); //translation process
        transVec.push_back(transMat);   
    }
    trunkStore.clear();
    trunkStore = transVec; // copying process
    transVec.clear();

}

vertex scaling(vertex matrixVert, float scaler)
{
    vertex transMat;
    scaler = scaler/100;
    transMat.x = (scaler * matrixVert.x) + matrixVert.x;
    transMat.y = (scaler * matrixVert.y) + matrixVert.y; // when in 3D, add z scaling bc default value never changes
    return transMat;

}

void scalingApp(float scaler)
{
    vertex transMat;
    vector<vertex> transVec;

    for(int i  = 0; i < pointStore.size(); i++)
    {
        transMat = scaling(pointStore[i],scaler); //scaling process
        transVec.push_back(transMat);   
    }

    pointStore.clear();
    pointStore = transVec; // copying process
    transVec.clear();

    for(int i  = 0; i < trunkStore.size(); i++)
    {
        transMat = scaling(trunkStore[i],scaler); //scaling process
        transVec.push_back(transMat);   
    }
    trunkStore.clear();
    trunkStore = transVec; // copying process
    transVec.clear();
}

void changeScale(float scaler) // move to origin, scale, then move to original position - scaling transformation pipeline
{
    translationApp( treeCenter.x*-1, treeCenter.y*-1); 
    scalingApp(scaler);
    translationApp(treeCenter.x, treeCenter.y);
    //defineTree();
}

vertex rotation(float theta, vertex angleVert)
{
     float phi;
     vertex transMat;

     // Convert degrees to radians

     phi = theta * M_PI / 180.0;
     transMat.x = (angleVert.x * cos(phi)) - (angleVert.y * sin(phi));
     transMat.y = (angleVert.x * sin(phi)) + (angleVert.y * cos(phi)); //ROTATING
    // Need to add Z dimension if in 3D, it is set to 1 by default
     return transMat;

}

void rotationApp(float theta) // may need to change order
{
    vertex transMat;
    vector<vertex> transVec;

    for(int i  = 0; i < pointStore.size(); i++)
    {
        transMat = rotation(theta , pointStore[i]); // circle rotation process
        transVec.push_back(transMat);   
    }

    pointStore.clear();
    pointStore = transVec; // copying process
    transVec.clear();

    for(int i  = 0; i < trunkStore.size(); i++)
    {
        transMat = rotation(theta , trunkStore[i]); //trunk rotation process
        transVec.push_back(transMat);   
    }
    trunkStore.clear();
    trunkStore = transVec; // copying process
    transVec.clear();
}

void rotating(float factor) //rotation transformation pipeline
{
    translationApp( treeCenter.x * -1, treeCenter.y * -1); // not sure if -1 or 0 is needed
    rotationApp(factor); // put into rotation application the factor with which you are rotating by
    translationApp(treeCenter.x, treeCenter.y);
}

void reflection() //reflect over vertical axis , if > central point x, current - central = new displacement, then central - displacement = new point. if < central point x, central - current = new. central + displacement = new point.
{

}

vertex lineIntersect(vertex p1, vertex p2, vertex p3, vertex p4)
{
  vertex intersection; // set to false by default
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

int diagIntersection(vector<vertex> pts, int index)
{
  vertex d,ex; 
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
int clockWise(vector<vertex> dot)
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
int vertexCheck(vector<vertex>dots) // is it CCW or is it not?
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
  vector<vertex>copy;
  vector<vertex>triCheck(3); // do not destroy your original points
  int count,ccw, triCount;
  vertex intersection;
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
        triStore.push_back(triCheck[0]);
        triStore.push_back(triCheck[1]);
        triStore.push_back(triCheck[2]);
        //triStore.push_back(triangles);
        copy.erase(copy.begin()+(count+1)%copy.size());
        count = 0;
      }
      }
      
    }
    else if(ccw == -1)
    {
        cout << "what" << endl;
    }
  }
  if(copy.size() == 3)
  {
        triStore.push_back(triCheck[0]);
        triStore.push_back(triCheck[1]);
        triStore.push_back(triCheck[2]);
  }


  
}

void drawShape(void)
{ 
    int lastPos = 0;
	int length = pointStore.size(); // may change
       glClear (GL_COLOR_BUFFER_BIT);  // Clear display window.
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColor3f (0.0, 0.4, 0.2);  
        glBegin(GL_TRIANGLES);    // Set line segment color to green.
        for(int i = 0; i < length; i++)
        {       			
            glVertex2f( pointStore[i].x,pointStore[i].y);
           // glColor3f (0.0, 0.4, 0.2);
        }
         for(int i = 0; i < trunkStore.size(); i++)
        {       			
            glVertex2f( trunkStore[i].x,trunkStore[i].y);
            //glColor3f (0.0, 0.4, 0.2);
        }
        glEnd();
        glBegin(GL_LINES);    // Set line segment color to green.
       for(int i = 0; i < length; i++)
      {       			
           glVertex2f( pointStore[i].x,pointStore[i].y);
           glColor3f (0.0, 0.4, 0.2);
       }
        for(int i = 0; i < trunkStore.size(); i++)
        {       			
            glVertex2f( trunkStore[i].x,trunkStore[i].y);
            glColor3f (0.0, 0.4, 0.2);
       } 

      /*  for(int i = 0; i < pointStore.size(); i++)
        {
            glBegin(GL_TRIANGLES);
                glVertex2f(pointStore[i].x,pointStore[i].y);
                glColor3f (0.0, 0.4, 0.2);
            glEnd();
        }
       for(int i = 0; i < trunkStore.size(); i++)
        {
            glBegin(GL_);
                glVertex2f(trunkStore[i].x,trunkStore[i].y);
                glColor3f (0.0, 0.4, 0.2);
            glEnd();
        } */
        //glEnd();
      // glFlush();
    glutSwapBuffers ();     // Process all OpenGL routines as quickly as possible.*/

}

//based off Sutherland-Hodgman Algorithm w/ twoDclippers file & Rosetta Code guidance
vector<vertex> clippingProcess(vector<vertex>exStore) // Check Left -> Check Right -> Check Bottom -> Check Top 
//Works backwards and asks if the point is not outside the clipping boundary before proceeding
{
    vector<vertex> fin;
    vertex intersection;
    float slope = 0;

    for(int i = 1; i < exStore.size(); i++) // Left Side Check p1 = i-1 , p2 = i
    {
        if(exStore[i-1].x == exStore[i].x) // accounts for a case where 0 causes errors throughout clipping process
        {
            slope == 0;
        }
        else
        {
            slope = (exStore[i-1].y - exStore[i].y)/(exStore[i-1].x - exStore[i].x);
        }

        if(exStore[i-1].x > VIEWPORT_MIN)
        {
            if(exStore[i].x > VIEWPORT_MIN)
            {
                //fin.push_back(exStore[i]); // may cause issues of duplicate values
                fin.push_back(exStore[i]);
            }
            else
            {
                intersection.x = VIEWPORT_MIN;
                intersection.y = exStore[i].y + ( (VIEWPORT_MIN - exStore[i].x) * slope);
                fin.push_back(intersection);

            }
        }
        else
        {
            if(exStore[i-1].x > VIEWPORT_MIN)
            {
                intersection.x = VIEWPORT_MIN;
                intersection.y = exStore[i].y + ( (VIEWPORT_MIN - exStore[i].x) * slope);
                fin.push_back(intersection);
                fin.push_back(exStore[i]);
            }
        }
    }

    for(int i = 1; i < exStore.size(); i++) // Right Side Check
    {
        if(exStore[i].x == exStore[i-1].x) // accounts for a case where 0 causes errors throughout clipping process
        {
            slope == 0;
        }
        else
        {
            slope = (exStore[i-1].y - exStore[i].y)/(exStore[i-1].x - exStore[i].x);
        }

        if(exStore[i-1].x < VIEWPORT_MAX)
        {
            if(exStore[i].x < VIEWPORT_MAX)
            {
                //fin.push_back(exStore[i]); // may cause issues of duplicate values
                fin.push_back(exStore[i]);
            }
            else
            {
                intersection.x = VIEWPORT_MAX;
                intersection.y = exStore[i].y + ( (VIEWPORT_MAX - exStore[i].x) * slope);
                fin.push_back(intersection);

            }
        }
        else
        {
            if(exStore[i].x < VIEWPORT_MAX)
            {
                intersection.x = VIEWPORT_MAX;
                intersection.y = exStore[i].y + ( (VIEWPORT_MAX - exStore[i].x) * slope);
                fin.push_back(intersection);
                fin.push_back(exStore[i]);
            }
        }
    }

     for(int i = 1; i < exStore.size(); i++) // Bottom Side Check
    {
        if(exStore[i].x == exStore[i-1].x) // accounts for a case where 0 causes errors throughout clipping process
        {
            slope == 0;
        }
        else
        {
            slope = (exStore[i-1].y - exStore[i].y)/(exStore[i-1].x - exStore[i].x);
        }

        if(exStore[i-1].y > VIEWPORT_MIN)
        {
            if(exStore[i].y > VIEWPORT_MIN)
            {
                //fin.push_back(exStore[i]); // may cause issues of duplicate values
                fin.push_back(exStore[i]);
            }
            else
            {
                //intersection.x = VIEWPORT_MIN;
                //intersection.y = exStore[i-1].y + ( (VIEWPORT_MIN - exStore[i-1].x) * slope);
                intersection.y = VIEWPORT_MIN;
                if(exStore[i-1].x != exStore[i].x)
                {
                    intersection.x = exStore[i].x + ( (VIEWPORT_MIN - exStore[i].y) / slope );
                }
                else
                {
                    intersection.x = exStore[i].x;
                }
                fin.push_back(intersection);

            }
        }
        else
        {
            if(exStore[i].y > VIEWPORT_MIN)
            {
                intersection.y = VIEWPORT_MIN;
                if(exStore[i-1].x != exStore[i].x)
                {
                    intersection.x = exStore[i].x + ( (VIEWPORT_MIN - exStore[i].y) / slope );
                }
                else
                {
                    intersection.x = exStore[i].x;
                }
                fin.push_back(intersection);
                fin.push_back(exStore[i]);
            }
        }
    }
    

    for(int i = 1; i < exStore.size(); i++) // Top Side Check
    {
        if(exStore[i].x == exStore[i-1].x) // accounts for a case where 0 causes errors throughout clipping process
        {
            slope == 0;
        }
        else
        {
            slope = (exStore[i-1].y - exStore[i].y)/(exStore[i-1].x - exStore[i].x);
        }

        if(exStore[i-1].y < VIEWPORT_MAX)
        {
            if(exStore[i].y < VIEWPORT_MAX)
            {
               // fin.push_back(exStore[i]); // may cause issues of duplicate values
                fin.push_back(exStore[i]);
            }
            else
            {
                //intersection.x = VIEWPORT_MIN;
                //intersection.y = exStore[i-1].y + ( (VIEWPORT_MIN - exStore[i-1].x) * slope);
                intersection.y = VIEWPORT_MAX;
                if(exStore[i-1].y != exStore[i].y)
                {
                    intersection.x = exStore[i].x + ( (VIEWPORT_MAX - exStore[i].y) / slope );
                }
                else
                {
                    intersection.x = exStore[i].x;
                }
                fin.push_back(intersection);

            }
        }
        else
        {
            if(exStore[i].y < VIEWPORT_MAX)
            {
                intersection.y = VIEWPORT_MAX;
                if(exStore[i-1].x != exStore[i].x)
                {
                    intersection.x = exStore[i].x + ( (VIEWPORT_MAX - exStore[i].y) / slope );
                }
                else
                {
                    intersection.x = exStore[i].x;
                }
                fin.push_back(intersection);
                fin.push_back(exStore[i]);
            }
        }
    } 

    return fin;
}
//vector<vertex>
void clipper() //Is the driver for clipping above
{
    vector <vertex> tempVec1,tempVec2,finalVec; // may need final Vec and temp vecs if clipped object comes back into viewport so points arent lost
    tempVec1 = pointStore;
    tempVec2 = trunkStore;
  tempVec1 = clippingProcess(tempVec1);
  tempVec2 = clippingProcess(tempVec2);
  //pointStore = tempVec1;
  //trunkStore = tempVec2;
 //  pointStore = clippingProcess(pointStore);
  //trunkStore = clippingProcess(trunkStore);

  //  for(int i = 0; i < tempVec1.size(); i++)
  //  {
  //      finalVec.push_back(tempVec1[i]);
  //  }
  //  for(int i = 0; i < tempVec2.size(); i++)
  //  {
   //     finalVec.push_back(tempVec2[i]);
   // }
  
 // return finalVec;

}

void myinit( void )
{
/* attributes */

      glClearColor(0.0, 0.0, 0.0, 1.0); /* black background */
      glPointSize(1.0);
      

/* set up viewing */

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluOrtho2D(0.0, (float) WINDOW_MAX, 
                 0.0, (float) WINDOW_MAX);
      glMatrixMode(GL_MODELVIEW);
}



void display( void )
{

    
    
    glClear(GL_COLOR_BUFFER_BIT);  /*clear the window */

    /* Define Viewport */
    glColor3f(1.0, 1.0, 1.0);
    glRecti(VIEWPORT_MIN,VIEWPORT_MIN,VIEWPORT_MAX,VIEWPORT_MAX);

    /* Define the arrow points */
   //defineArrow( apts );
   if(firstProcess == 1) //set to false by default
   {
    defineTree();
    masterPointStore = pointStore;
    masterTrunkStore = trunkStore;
    masterTreeCenter = treeCenter;
    firstProcess = 0;
   }
   else
   {
        clipper();
   }
    
    if(SPIN != 0) // make sure the rotating keeps happening
    {
        rotating(SPIN);
    }
    /* Now start the process of rotating */
   //PipeLine( apts, inPoints );
   //toVertex( apts, invp, inPoints );

    
    defineTree(); // redefine the new tree, change here after clipping
    glColor3f(1.0, 0.0, 0.0);

    /* Draw Scaled and Rotated Arrow */
    //drawArrow( invp, inPoints );
    glutSwapBuffers(); // swap your buffers

    

 }


void SpinDisplay(void) // may need to cap the adding of degrees at 10 degrees
{
   // SPIN = SPIN + DELTA_SPIN;
    if (SPIN > 360.0) SPIN = SPIN - 360.0;
    glutPostRedisplay();
}

int within(int x, int y) // determines if the mouse is in the viewport , 0 = true , 1 = false
{
    if (x > VIEWPORT_MIN && x < VIEWPORT_MAX && y > VIEWPORT_MIN && y < VIEWPORT_MAX)
    {
        return 0;
    }
    else
    {
        return 1;
    }
    
}

void mouse(int button, int state, int x, int y) 
{
   
    y = WINDOW_MAX-y; //accurate coordinates for y
    float scaler = 0;

    if(within(x,y) == 0)
    {
        if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
        {
            SPIN = SPIN + 1.0;
                glutIdleFunc(SpinDisplay);
        }
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            SPIN = SPIN - 1.0;
                glutIdleFunc(SpinDisplay);
        }
        rotating(SPIN);
    }
    if(within(x,y) == 1)
    {
        if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
        {
            scaler = -1.05;
            changeScale(scaler);
        }
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
             scaler = 1.05;
            changeScale(scaler);
        }
    }
 glutPostRedisplay();

}
void keyboard( unsigned char key, int x, int y )
{ 
    if ( key == 'q' || key == 'Q') exit(0);
    if ( key == 'i' || key == 'I') { // stop animation & return to initial tree position
        SPIN = 0.0;
        reset();
        glutIdleFunc(display);
    }
    if(key == 'r' || key == 'R') // reflect image along vertical axis thru central point, continue any animations ongoing in same direction
    {
        rotating(180.0); //maybe this will work?
        glutPostRedisplay();
	//exit(0);
    }
    if ( key == 's' || key == 'S') { // stops any animations ongoing
        SPIN = 0;
        glutIdleFunc(NULL);
    }
    if(key == 'f' || key == 'F') //fills the tree, must be tesselated first
    {
        drawShape();
        glutIdleFunc(display);
	//exit(0);
    }
    if(key == 't' || key == 'T') // tesselates & reveals unfilled tesselation triangles
    {
	exit(0);	
    }
    if(key == 'l' || key == 'L') // returns to an unfilled original tree state after filling it
    {
        //reset();
    }
    glutPostRedisplay();
}


int main(int argc, char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB); 
    glutInitWindowSize(WINDOW_MAX,WINDOW_MAX); 
    glutInitWindowPosition(0,0); 
    glutCreateWindow("The Funky Tree"); 
    myinit(); 
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display); 
    glutMainLoop();
}

// Graveyard of Code
/*
//exit(0);
/* switch (button) {
        case GLUT_LEFT_BUTTON: // every click left, faster rotation
            if (state == GLUT_DOWN) //start animations
            { // if outside viewport, increase scale by 5%
                //DELTA_SPIN = DELTA_SPIN - 1.0;
                //glutIdleFunc(SpinDisplay);
                //printf ("%d   %d\n", x, y);
                
                //drawBox( x, y );
            }
            break;
        case GLUT_RIGHT_BUTTON: //decrement angle of rotation by -10 degrees per click
            if (state == GLUT_DOWN) // if outside viewport, scale decreased by 5%
            {
                DELTA_SPIN = DELTA_SPIN + 1.0;
                glutIdleFunc(SpinDisplay);
            }
            break;
        default:
            break;
            //int inPoints;             // The number of points in the arrow 
    //float point[MAX];         // Array to hold homogenous coordinates for arrow
    //float *apts;              // Pointer for point array 
   // vector <vertex> cirPoints;
    //struct vertex inVertexArray[MAX/4];	/* array of vertices to hold points */
    //struct vertex *invp;                /* Pointer for inVertexArray  */  

    /* Make sure to initialize the pointers! */ 
    //apts = &point[0];         // the pointer to the array of points 
   // invp = &inVertexArray[0]; // the pointer to the array of vertices

    //inPoints = 5;             // the actual number of points in the arrow

    /*if(within(x,y) == 0 && button == GLUT_LEFT_BUTTON) // inside viewport
    {
        if (state == GLUT_DOWN)
        {
            DELTA_SPIN = DELTA_SPIN - 1.0;
                glutIdleFunc(SpinDisplay);
        }
        else
        {

        }
    }
    if(within(x,y) == 0 && button == GLUT_RIGHT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            DELTA_SPIN = DELTA_SPIN + 1.0;
                glutIdleFunc(SpinDisplay);
        }
        else
        {

        }
    }
    if(within(x,y) == 1 && button == GLUT_LEFT_BUTTON) // outside viewport
    {
        if (state == GLUT_DOWN)
        {
            scaler = 1.05;
            changeScale(scaler);
        }
    }
    if(within(x,y) == 1 && button == GLUT_RIGHT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            scaler = 0.95;
            changeScale(scaler);
        }
    } */
   // button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN
// glClear(GL_COLOR_BUFFER_BIT);
//glVertex2i(triStore[i].t2.x,triStore[i].t2.y);
      //glVertex2i(triStore[i].t3.x,triStore[i].t3.y);
//defineTree();
/*void fillTesselation(void)
{
     glBegin(GL_POLYGON);
   for(int i = 0; i < pointStore.size(); i++) // drawing tesselation
  {
   
      glVertex2f(pointStore[i].x,pointStore[i].y);
    

    //glFlush();
  }
  
  for(int i = 0; i < trunkStore.size(); i++) // drawing tesselation
  {
    
      glVertex2f(trunkStore[i].x,trunkStore[i].y);
   

   // glFlush();
  }
   glEnd();
   glFlush();
   /*if(intersection.x == -1 && intersection.y == -1)
      {
        triangles.t1 = triCheck[0];
        triangles.t2 = triCheck[1];
        triangles.t3 = triCheck[2];
        triStore.push_back(triangles);
        copy.erase(copy.begin()+count+1);
        count = 0;
      } */
