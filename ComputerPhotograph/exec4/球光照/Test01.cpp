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
	glClearColor(0.0f,0.0f,0.0f,0.0f); 
	glClear(GL_COLOR_BUFFER_BIT);


	//光照
	//设置第1个球光照
	GLfloat light0_specular[] = { 1.0f, 0.0f, 0.0f, 1.0f };  
	GLfloat light0_ambient[]  = { 1.0f, 0.0f, 0.0f, 1.0f };  
	GLfloat light0_diffuse[]  = { 1.0f, 0.0f, 0.0f, 1.0f }; 
	GLfloat light0_position[] = { -3.0f, -3.0f, 0.0f, 0.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);


   //设置第二个球光照
	GLfloat light1_specular[] = { 0.0f, 1.0f, 0.0f, 1.0f }; 
	GLfloat light1_ambient[]  = { 0.0f, 1.0f, 0.0f, 1.0f };  
	GLfloat light1_diffuse[]  = { 0.0f, 1.0f, 0.0f, 1.0f }; 
	GLfloat light1_position[] = { 1.0f, 0.0f, 0.0f, 1.0f };

	glLightfv(GL_LIGHT1, GL_AMBIENT,  light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

   //设置第三个球光照
	GLfloat light2_specular[] = { 0.0f, 1.0f, 0.0f,  1.0f };  
	GLfloat light2_ambient[]  = { 0.0f,  1.0f,  0.0f, 1.0f }; 
	GLfloat light2_diffuse[]  = { 0.0f, 1.0f, 0.0f, 1.0f }; 
	GLfloat light2_position[] = { -2.0f, -2.0f, 0.0f, 1.0f };

	glLightfv(GL_LIGHT2, GL_AMBIENT,  light2_ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE,  light2_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
	glLightfv(GL_LIGHT2, GL_POSITION, light2_position);



	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);



	//材质
	GLfloat mat_ambient[]={0.2,0.2,0.2,1.0};  
	GLfloat mat_diffuse[]={1.0,0.0,0.0,1.0}; 
	GLfloat mat_specular[]={1.0,0.0,0.0,1.0}; 
	GLfloat mat_emission[]={1.0,0.0,0.0,0.0}; 
	GLfloat shininess[] = {10};

	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,mat_diffuse); 
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,mat_specular); 
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,mat_ambient); 
//	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,mat_emission); 
	glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shininess); 

	glPushMatrix();
		//glRotatef(-60.0, 1.0,0.0,0.0);
	//	glColor4f(1.0,1.0,1.0,1.0);
		glutSolidTeapot(0.4);
    glPopMatrix();

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

