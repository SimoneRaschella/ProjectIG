#include <GL/glut.h>
#include <cmath>
#include <iostream>
using namespace std;

# define M_PI 3.14159265358979323846

//GLfloat posRedCube[3] = { 50, 50, 2.5 };
//GLfloat posBlueSphere[3] = { 70, -20, 2.5 };
GLint alpha = 90;
GLdouble alphaRad = alpha * M_PI / 180;
GLdouble gamePosition[3] = {0.0 ,0.0 ,1.0};
GLdouble gameForward [3] = {cos(alphaRad), sin(alphaRad), 1.0};


void init(){
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
}

void reshape(int w, int h){
	GLfloat fAspect;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	fAspect = (GLfloat)w / (GLfloat)h;
	gluPerspective(60.0f, fAspect, 0.1f, 500.0f);  

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	gluLookAt(gamePosition[0], gamePosition[1], gamePosition[2],
		  gameForward[0], gameForward[1], gameForward[2], 
		  0.0, 0.0, 1.0);
	
	glPushMatrix();
	//for (int i = 0; i < 50; i++){
	//	for (int j = 0; j < 50; j++){
	//		if ((i+j) % 2 == 0)
	//			glColor3f(0.0, 0.0, 0.7);
	//		else
				glColor3f(0.7, 0.7, 0.7);

			glBegin(GL_POLYGON);
			glVertex2f(0.0, 0.0);
			glVertex2f(100.0, 0.0);
			glVertex2f(100.0, 100.0);
			glVertex2f(0.0, 100.0);
			glEnd();

	//		glTranslatef(2.0, 0.0, 0.0);
	/*	}
		glTranslatef(0.0, 2.0, 0.0);
		glTranslatef(-100.0, 0.0, 0.0);
	}*/
	//glPopMatrix();
	
	/*glPushMatrix();
		glTranslatef(posRedCube[0], posRedCube[1], posRedCube[2]);
		glColor3f(1.0, 0.0, 0.0);
		glutSolidCube(5);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(posBlueSphere[0], posBlueSphere[1], posBlueSphere[2]);
		glColor3f(0.0, 0.0, 1.0);
		glutSolidSphere(5, 50, 50);
	glPopMatrix();*/

	glPopMatrix();

	

	glutSwapBuffers();
}

void SpecialKeys(int key, int x, int y){
	switch (key){
	case GLUT_KEY_DOWN:
		gamePosition[0] -= cos(alphaRad);
		gamePosition[1] -= sin(alphaRad);
		gameForward[0] = gamePosition[0] + cos(alphaRad);
		gameForward[1] = gamePosition[1] + sin(alphaRad);
		break;
	case GLUT_KEY_UP:
		gamePosition[0] += cos(alphaRad);
		gamePosition[1] += sin(alphaRad);
		gameForward[0] = gamePosition[0] + cos(alphaRad);
		gameForward[1] = gamePosition[1] + sin(alphaRad);
		break;
	case GLUT_KEY_LEFT:
		alpha = (alpha + 5) % 360;
		alphaRad = alpha * M_PI / 180;
		gameForward[0] = gamePosition[0] + cos(alphaRad);
		gameForward[1] = gamePosition[1] + sin(alphaRad);
		break;
	case GLUT_KEY_RIGHT:
		alpha = (alpha - 5) % 360;
		alphaRad = alpha * M_PI / 180;
		gameForward[0] = gamePosition[0] + cos(alphaRad);
		gameForward[1] = gamePosition[1] + sin(alphaRad);
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1350, 690);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Floor");
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutSpecialFunc(SpecialKeys);
	glutMainLoop();
	return 0; 
}