// C program to illustrate OpenGL game
#include<stdio.h>
#include<GL/glut.h>
#include<math.h>
#include <time.h>
#include <string.h>
#define pi 3.142857

GLfloat angle = 0.0f; // Current rotational angle of the shapes


// Initialization function
void myInit (void)
{
	// Reset background color with white (since all three argument is 1.0)
	glClearColor(1.0, 1.0, 1.0, 0.0);

	// Set picture color to red (in RGB model)
	// as only argument corresponding to R (Red) is 1.0 and rest are 0.0
	glColor3f(1.0f, 0.0f, 0.0f);

	// Set width of point to one unit
	glPointSize(2.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set window size in X- and Y- direction
	gluOrtho2D(-620.0, 620.0, -340.0, 340.0);
}
/*
void reshape(){
	glViewport(0,0,1100, 600);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1100/2,600/2-1,-600/2,600/2-1,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
*/
void Timer(int ex)
{
 	glutPostRedisplay();
	glutTimerFunc(30,Timer,0);
}


 
int r=100,flag=0;


void myDisplay(void)
{

	// x and y keeps point on circumference of circle
	int k;
	// outer 'for loop' is to for making motion
	for (k = 0; k <= 400; k += 5)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_LINE_STRIP);
		
		
	
		// these four points draws outer rectangle which determines window
		glBegin(GL_LINE_LOOP);
			glVertex2i(-600, -320);
			glVertex2i(-600, 320);
			glVertex2i(600, 320);
			glVertex2i(600, -320);
			glVertex2i(-600, -320);
		glEnd();

		// these four points draws smaller rectangle which is Inner grouping area
		glBegin(GL_LINE_LOOP);
			glVertex2i(300, -160);
			glVertex2i(300, 210);
			glVertex2i(590, 210);
			glVertex2i(590, -160);
		glEnd();
		
		
		
		// these four points draws smaller rectangle which is teller 1
		glBegin(GL_LINE_LOOP);
			glVertex2i(500, 120);
			glVertex2i(500, 200);
			glVertex2i(580, 200);
			glVertex2i(580, 120);
			
		glEnd();
		
		// these four points draws smaller rectangle which is teller 2
		glBegin(GL_LINE_LOOP);
			glVertex2i(500, 30);
			glVertex2i(500, 110);
			glVertex2i(580, 110);
			glVertex2i(580, 30);
		glEnd();
		// these four points draws smaller rectangle which is teller 3
		glBegin(GL_LINE_LOOP);
			glVertex2i(500, -60);
			glVertex2i(500, 20);
			glVertex2i(580, 20);
			glVertex2i(580, -60);
		glEnd();
		// these four points draws smaller rectangle which is teller 4
		glBegin(GL_LINE_LOOP);
			glVertex2i(500, -150);
			glVertex2i(500, -70);
			glVertex2i(580, -70);
			glVertex2i(580, -150);
		glEnd();
		
		
		//rolling gate for males
		 GLfloat x, y, angle;  
		 glBegin( GL_POINTS);
		     for (angle = 0.0f; angle <= (2.0f * pi); angle += 0.01f) { 
		          x = -350.0f +(50.0f * sin(angle));
		          y = -70.0f +(50.0f * cos(angle));      
		          glVertex3f(x, y, 0.0f);   
		     } 
		  glEnd(); 
		 // these four points draws smaller rectangle which is body check for males
		glBegin(GL_LINE_LOOP);
			glVertex2i(0, 50);
			glVertex2i(0, 200);
			glVertex2i(60, 200);
			glVertex2i(60, 50);
		glEnd();
		 
		 //rolling gate for females
		 GLfloat x1, y1, angle1;  
		 glBegin( GL_POINTS);
		     for (angle1 = 0.0f; angle1 <= (2.0f * pi); angle1 += 0.01f) { 
		          x1 = -350.0f +(50.0f * sin(angle1));
		          y1 = 120.0f +(50.0f * cos(angle1));      
		          glVertex3f(x1, y1, 0.0f);   
		     } 
		  glEnd(); 
		  
		  // these four points draws smaller rectangle which is body check for females
		glBegin(GL_LINE_LOOP);
			glVertex2i(0, 0);
			glVertex2i(0, -150);
			glVertex2i(60, -150);
			glVertex2i(60, 0);
		glEnd();
		glPointSize(7.0);
		/*  
    		glPointSize(5.0);
   		glColor3f(1.0f, 0.0f, 0.0f);
    		glBegin(GL_POINTS);
    		glVertex2i(-500.0, 0.0);
    		glFlush();
    		//delay(500);
    		glEnd();
    		*/
    		/*
    		glBegin (GL_POINTS);
		for (int i = 0; i < 10000; i++)
		{
		
    			glVertex3f (-500.0, 120.0, 0.0);
    			glVertex3f (-510.0, 120.0, 0.0);
    			glVertex3f (-520.0, 120.0, 0.0);
    			glVertex3f (-530.0, 120.0, 0.0);
    			glVertex3f (-540.0, 120.0, 0.0);
    			
    			glVertex3f (-500.0, -90.0, 0.0);
    			glVertex3f (-510.0, -90.0, 0.0);
    			glVertex3f (-520.0, -90.0, 0.0);
    			glVertex3f (-530.0, -90.0, 0.0);
    			glVertex3f (-540.0, -90.0, 0.0);
    		
    		
    		      
    			
    			
		}
		glEnd ();
		*/
		
    			glPointSize(10);
   			glBegin(GL_POINTS);
    			glColor3f(1, 0, 0);
   			
    			glVertex3f (-500.0, 120.0, 0.0);
    			glVertex3f (-520.0, 120.0, 0.0);
    			glVertex3f (-540.0, 120.0, 0.0);
    			glVertex3f (-560.0, 120.0, 0.0);
    			glVertex3f (-580.0, 120.0, 0.0);
    			
    			glVertex3f (-500.0, -90.0, 0.0);
    			glVertex3f (-520.0, -90.0, 0.0);
    			glVertex3f (-540.0, -90.0, 0.0);
    			glVertex3f (-560.0, -90.0, 0.0);
    			glVertex3f (-580.0, -90.0, 0.0);
    			glEnd();

   			
   			glMatrixMode(GL_MODELVIEW); // To operate on Model-View matrix
   			glLoadIdentity(); // Reset the model-view matrix
   			glPushMatrix(); // Save model-view matrix setting
   			glTranslatef(-0.1f, 0.0f, 0.0f); // Translate
			glRotatef(angle, 0.0f, 1.0f, 0.0f);

    			glBegin(GL_POINTS);
    			glColor3f(0, 0, 0);
    			glVertex3f (200.0, 120.0, 0.0);
    			glEnd();
    			
    			glPopMatrix(); // Restore the model-view matrix
    			glPushMatrix();

    			glTranslatef(-0.1f, 0.0f, 0.0f); // Translate
   			glRotatef(angle, 0.0f, 1.0f, 0.0f);

  			glBegin(GL_POINTS);
   			glColor3f(0, 0, 0);
    			glVertex3f (100.0, -90.0, 0.0);
    			glEnd();
    			
    			glPopMatrix(); // Restore the model-view matrix
    			glPushMatrix();


    			// Change the rotational angle after each display()
   			 angle += 0.9f;
    		
    		
    		
    		
    		
    		
    		
    		
    		
    		
    		
   		glLineWidth(4.0); 
   		//to write text
                glRasterPos2f(-400, 230);
  		int len, i;
  		char *string = "Rolling Gates                                           Body Check                                                    Inner Grouping Area ";
  		len = (int)strlen(string);
  		for (i = 0; i < len; i++) {
    		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
  		}
  		
  		
  		//to write tellers labels
                glRasterPos2f(515, 140);
  		int len1, i1;
  		char *string1 = "Travel";
  		len1 = (int)strlen(string1);
  		for (i1 = 0; i1 < len1; i1++) {
    		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string1[i1]);
  		}
  		
  		glRasterPos2f(505, 70);
  		int len2, i2;
  		char *string2 = "Reunion";
  		len2 = (int)strlen(string2);
  		for (i2 = 0; i2 < len2; i2++) {
    		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string2[i2]);
  		}
  		glRasterPos2f(515, -15);
  		int len3, i3;
  		char *string3 = "ID";
  		len3 = (int)strlen(string3);
  		for (i3 = 0; i3 < len3; i3++) {
    		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string3[i3]);
  		}
  		glRasterPos2f(515, -100);
  		int len4, i4;
  		char *string4 = "Birth";
  		len4 = (int)strlen(string4);
  		for (i4 = 0; i4 < len4; i4++) {
    		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string4[i4]);
  		}
  		
  		//to draw the occupation officer
  		glBegin(GL_TRIANGLES);
		glVertex2f( -150, 200 );
		glColor3f( 0, 1, 0 ); // green
		glVertex2f( -100, 200 );
		glColor3f( 0, 0, 1 ); // blue
		glVertex2f( -125, 250 );
		glEnd();
		
		
		glutSwapBuffers();
	}
}


void idle(void){
/* do nothing */
}


// Driver Program
int main (int argc, char** argv)
{
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	// Declares window size
	glutInitWindowSize(1100, 600);
	
	// Declares window position which is (0, 0)
	// means lower left corner will indicate position (0, 0)
	glutInitWindowPosition(0, 0);

	// Name to window
	glutCreateWindow("OIM OFFICE");
	
	// Call to myInit()
	myInit();
	glutIdleFunc(idle);
        //glutReshapeFunc(reshape);
	glutDisplayFunc(myDisplay);
	glutTimerFunc(0,Timer,0);
	glutMainLoop();
	
}

