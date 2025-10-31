// Test01.cpp : Defines the entry point for the console application.
//



#include "stdafx.h"
#include "GL/glut.h"


void reshape(int w, int h){

	//glViewport (0,0,w/2,h/2);   
	//glOrtho(0,1,-1,1,-1,1);    

}


void disPlay()
{
	glClearColor(0.0f,1.0f,0.0f,0.0f); 
	glClear(GL_COLOR_BUFFER_BIT);


	glColor4f(1.0,0.0,0.0,1.0);


	glRotatef(-60.0, 1.0,0.0,0.0);
	glutWireSphere(0.6, 100, 100);



	glFlush();
}


void mouse(int button, int state, int x, int y){
	

	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		
		glClearColor(0.0f,0.0f,1.0f,0.0f); 
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1.0,0.0,0.0);
		glBegin(GL_LINES);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.1,0.5,0.0);
		glVertex3f(-0.5,0.0,0.0);
		glVertex3f(-0.5,-0.5,0.0);
		glEnd();
	}

	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		glClearColor(0.0f,0.0f,1.0f,0.0f); 
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(1.0,0.0,0.0);
		glRotatef(40.0, 1.0,0.0,0.0);
		glutWireTeapot(0.35);
	}

	glFlush();
}

void keys(unsigned char key,int x,int y){
	if(key=='1')
	{ 
		glClearColor(0.0f,0.0f,1.0f,0.0f); 
		glClear(GL_COLOR_BUFFER_BIT);
		glRectf(-0.5,-0.5,0.5,0.5);
	}

	if(key=='2')
	{
		glClearColor(0.0f,0.0f,1.0f,0.0f); 
		glClear(GL_COLOR_BUFFER_BIT);
		glutWireTorus(0.1, 0.6, 10, 30);	
	}
	if(key=='3')
	{
		glClearColor(0.0f,0.0f,1.0f,0.0f); 
		glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_TRIANGLES);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.1,0.5,0.0);
		glVertex3f(-0.5,0.0,0.0);
		glEnd();	
	}
	glFlush();
}



int main(int argc, char* argv[])
{

	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(600,600);
	glutInitWindowPosition(300,0);
	  
	glutCreateWindow("Hello");   

	glutDisplayFunc(disPlay);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keys);
	glutReshapeFunc(reshape);	

	glutMainLoop();   


	return 0;
}

