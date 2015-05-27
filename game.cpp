#include <GL/glut.h>
#include <cmath>
#include <cassert>
#include <iostream>
#include <fstream>
#include "maze.h"
using namespace std;

# define M_PI 3.14159265358979323846

GLint alpha = 90;
GLdouble alphaRad = alpha * M_PI / 180;
GLdouble gamePosition[3] = {3.0 ,3.0 ,1.0};
GLdouble gameForward [3] = {cos(alphaRad) + 3.0, sin(alphaRad) + 3.0, 1.0};
const GLdouble DIMENSION_SQUARE = 2.0;
int NUMBER_CELL = 50;
const GLdouble DIMENSION_MAZE = DIMENSION_SQUARE * NUMBER_CELL; 
Maze maze(50);


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
	ifstream fin;
	fin.open("maze.txt");
	assert (fin.is_open());
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	gluLookAt(gamePosition[0], gamePosition[1], gamePosition[2],
		  gameForward[0], gameForward[1], gameForward[2], 
		  0.0, 0.0, 1.0);
	
	glPushMatrix();
	for (int i = 0; i < NUMBER_CELL; i++){
		for (int j = 0; j < NUMBER_CELL; j++){
			char number;
			fin >> number;
			if (number == '1'){
			  maze.setAt(i,j,true);
			  glPushMatrix();
			    glColor3f(1.0, 1.0, 1.0);
			    glTranslatef(DIMENSION_SQUARE/2, DIMENSION_SQUARE/2, 1.0);
			    glutSolidCube (2.0);
			  glPopMatrix();
			} else {
			  maze.setAt(i,j,false);
			if ((i+j) % 2 == 0)
				glColor3f(0.0, 0.0, 0.7);
			else
				glColor3f(0.7, 0.7, 0.7);

			glBegin(GL_POLYGON);
			glVertex2f(0.0, 0.0);
			glVertex2f(DIMENSION_SQUARE, 0.0);
			glVertex2f(DIMENSION_SQUARE, DIMENSION_SQUARE);
			glVertex2f(0.0, DIMENSION_SQUARE);
			glEnd();
			}
			glTranslatef(DIMENSION_SQUARE,0.0, 0.0);
		}
		glTranslatef(0.0, DIMENSION_SQUARE, 0.0);
		glTranslatef((-1)*DIMENSION_MAZE, 0.0, 0.0);
	}
	glPopMatrix();
	
	glPushMatrix();
	  glTranslatef(0.0,0.0, DIMENSION_SQUARE);
	  for (int i = 0; i < NUMBER_CELL; i++){
	    for (int j = 0; j < NUMBER_CELL; j++){
	      if (maze.getAt(i,j)){
		glPushMatrix();
		  glColor3f(1.0, 0.0, 0.0);
		  glTranslatef(DIMENSION_SQUARE/2, DIMENSION_SQUARE/2, 1.0);
		  glutSolidCube (2.0);
		glPopMatrix();
	      }
	      glTranslatef(DIMENSION_SQUARE,0.0, 0.0);
	    }
	    glTranslatef(0.0, DIMENSION_SQUARE, 0.0);
	    glTranslatef((-1)*DIMENSION_MAZE, 0.0, 0.0);
	  }
	glPopMatrix();

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