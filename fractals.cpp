
#include<stdio.h>   //standard C library
#include<stdlib.h>  //random function
#include<GL/glx.h>  //openGL library
#include<math.h>    //for sine cosine functions
#include<time.h>    //seed for random function
#include<unistd.h>

#define PI 3.14159265 //convert radian to degrees
int randomDepth();
//int depth;  //depth of triangle construction
int depth=9;
Display                 *dpy;
Window                  root;
GLint                   att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
XVisualInfo             *vi;
Colormap                cmap;
XSetWindowAttributes    swa;
Window                  win;
GLXContext              glc;
XWindowAttributes       gwa;
XEvent                  xev;

float rad(float x){
	return (x*(PI/180)); 
}

class triangle {
	public:
	float x1,y1,x2,y2,x3,y3;   //co ordinates of triangle

   float cx,cy,r;            //centroid co ordinate and radius
	
	triangle(float a,float b,float c){
		cx=a;
		cy=b;
		r=c;
	}
	 
};

float randomValue(){
	return (0+rand()%10)/10.0;    //random value between range 0 to 10, divided by 10 to make it from 0.0 to 1.0
}

int randomDepth(){
  return (1+rand()%8);
}
void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {
		float offsetX=0.8;
		float offsetY=-0.3;
	 	glColor3f(randomValue(), randomValue(), randomValue());
	 	glBegin(GL_TRIANGLES);
		glVertex3f(x1+offsetX,y1+offsetY,0.0);
        glVertex3f(x2+offsetX,y2+offsetY,-1.0);
        glVertex3f(x3+offsetX,y3+offsetY,1.0);
        glEnd();
}

   
void subTriangle(int n, float x1, float y1, float x2, float y2, float x3, float y3) //recursive function to draw triangle until depth reached
{
  usleep(100);
	drawTriangle(x1,y1,x2,y2,x3,y3);
  
  if(n < depth)
  {
  	
    subTriangle
    (
      n+1,
      (x1 + x2) / 2 + (x2 - x3) / 2, 
      (y1 + y2) / 2 + (y2 - y3) / 2, 
      (x1 + x2) / 2 + (x1 - x3) / 2, 
      (y1 + y2) / 2 + (y1 - y3) / 2, 
      (x1 + x2) / 2, 
      (y1 + y2) / 2  
    );
    
    subTriangle
    (
      n+1, 
      (x3 + x2) / 2 + (x2 - x1) / 2, 
      (y3 + y2) / 2 + (y2 - y1) / 2, 
      (x3 + x2) / 2 + (x3 - x1) / 2, 
      (y3 + y2) / 2 + (y3 - y1) / 2, 
      (x3 + x2) / 2, 
      (y3 + y2) / 2  
    );
    
    subTriangle
    (
      n+1, 
      (x1 + x3) / 2 + (x3 - x2) / 2,
      (y1 + y3) / 2 + (y3 - y2) / 2,
      (x1 + x3) / 2 + (x1 - x2) / 2,
      (y1 + y3) / 2 + (y1 - y2) / 2,
      (x1 + x3) / 2, 
      (y1 + y3) / 2);
  }

}


//Largest triangle construction is made by considering (cx,cy) is the origin and rotate point C by 60 degrees
// and by 120 degrees in order get a and b. This can be accomplished with the formula:
//b.x = c.x * cos( 120 degrees ) - ( c.y * sin( 120 degrees ) )
//b.y = c.x * sin( 120 degrees ) + ( c.y * cos( 120 degrees ) )
//a.x = c.x * cos( 240 degrees ) - ( c.y * sin( 240 degrees ) )
//a.y = c.x * sin( 240 degrees ) + ( c.y * cos( 240 degrees ) )

void myDraw() {

  triangle t (0.0,0.0,1.0); //first two arguments center of centroid and third is radius
	t.x1=t.cx;
	t.y1=t.cy+t.r;
	
	t.x2=t.x1*cos(rad(120.0)) - (t.y1)*sin(rad(120.0));
	t.y2=t.x1*sin(rad(120.0)) + (t.y1)*cos(rad(120.0));
	
	t.x3=t.x1*cos(rad(240.0)) - (t.y1)*sin(rad(240.0));
	t.y3=t.x1*sin(rad(240.0)) + (t.y1)*cos(rad(240.0));

	glClearColor(0.,0.,0.,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
		
    	subTriangle(1,(t.x1 + t.x2) / 2 + (t.x2 - t.x3) / 2,   //drawing first triangle
      (t.y1 + t.y2) / 2 + (t.y2 - t.y3) / 2,
      (t.x1 + t.x2) / 2 + (t.x1 - t.x3) / 2,
      (t.y1 + t.y2) / 2 + (t.y1 - t.y3) / 2,
      (t.x1 + t.x2) / 2,
      (t.y1 + t.y2) / 2);
} 
 int main() {
//int main(int argc, char *argv[]) {
  //depth=randomDepth();
 dpy = XOpenDisplay(NULL);
 
 srand(time(NULL));

 if(dpy == NULL) {
 	printf("\n\tcannot connect to X server\n\n");
        exit(0);
 }
        
 root = DefaultRootWindow(dpy);

 vi = glXChooseVisual(dpy, 0, att);

 if(vi == NULL) {
	printf("\n\tno appropriate visual found\n\n");
        exit(0);
 } 
 else {
	printf("\n\tvisual %p selected\n", (void *)vi->visualid); /* %p creates hexadecimal output like in glxinfo */
 }


 cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);

 swa.colormap = cmap;
 swa.event_mask = ExposureMask | KeyPressMask;
 
 win = XCreateWindow(dpy, root, 0, 0, 800, 800, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

 XMapWindow(dpy, win);
 XStoreName(dpy, win, "SIERPINKSI TRIANGLE");
 
 glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
 glXMakeCurrent(dpy, win, glc);

 
 while(1) {
 	XNextEvent(dpy, &xev);
        
        if(xev.type == Expose) {
        	XGetWindowAttributes(dpy, win, &gwa);
                glViewport(0, 0, gwa.width, gwa.height);
                myDraw();
                glXSwapBuffers(dpy, win);
        }
                
	else if ((xev.type == KeyPress)) {
    glXMakeCurrent(dpy, None, NULL);
 		glXDestroyContext(dpy, glc);
 		XDestroyWindow(dpy, win);
 		XCloseDisplay(dpy);
 		
    main();
    //exit(0);
        }
  else if (xev.type == ButtonPress) {
    //myDraw();
    //main();
  }
    } /* this closes while(1) { */
} 