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
GLdouble savePosition [3] = {0,0,0};
GLdouble saveForward [3] = {0,0,0};
GLdouble saveNormal [3] = {0,0,0};
GLdouble gamePosition[3] = {2 ,1.5 ,0.25};
GLdouble gameForward [3] = {cos(alphaRad) + 2, sin(alphaRad) + 1.5, 0.25};
GLdouble gameNormal [3] = {0.0, 0.0, 1.0};
GLdouble viewPosition [3] = {15, 15, 35};
GLdouble viewForward [3] = {15, 15, 34};
GLdouble viewNormal [3] = {0,1, 0};
GLdouble targetPosition [2] = {0,0};
bool view = false;
const GLdouble DIMENSION_SQUARE = 1.0;
int NUMBER_CELL = 30;
const GLdouble DIMENSION_MAZE = DIMENSION_SQUARE * NUMBER_CELL; 
Maze maze(NUMBER_CELL);


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
		  gameNormal[0], gameNormal[1], gameNormal[2]);
	
	if (view){
	  glPushMatrix();
	    glTranslatef(savePosition[0], savePosition[1], savePosition[2]);
	    glColor3f(1, 1, 1);
	    glutSolidSphere(0.5, 15, 15);
	  glPopMatrix();
	}
	
	glPushMatrix();
	for (int i = 0; i < NUMBER_CELL; i++){
		for (int j = 0; j < NUMBER_CELL; j++){
			char number;
			fin >> number;
			if (number == '1'){
			  maze.setAt(j,i,true);
			  glPushMatrix();
			    glColor3f(1.0, 1.0, 1.0);
			    glTranslatef(DIMENSION_SQUARE/2, DIMENSION_SQUARE/2, 0.5);
			    glutSolidCube (1.0);
			  glPopMatrix();
			} else {
			  if (number == '2'){
			    targetPosition[0] = j;
			    targetPosition[1] = i;
			    cout << targetPosition[0] << " " << targetPosition[1] << endl;
			    glPushMatrix();
			      glTranslatef(DIMENSION_SQUARE/2, DIMENSION_SQUARE/2, 0.35);
			      glColor3f(0.0, 0.0, 0.7);
			      glutSolidCube(0.5);
			    glPopMatrix();
			  }
			  maze.setAt(j,i,false);
			  glColor3f(0.0, 0.0, 0.1);
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
	      if (maze.getAt(j,i)){
		glPushMatrix();
		  glColor3f(1.0, 0.0, 0.0);
		  glTranslatef(DIMENSION_SQUARE/2, DIMENSION_SQUARE/2, 0.5);
		  glutSolidCube (1.0);
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
      GLint tmpX;
      GLint tmpY;
	switch (key){
	case GLUT_KEY_DOWN:
		if (!view){
		  gamePosition[0] -= cos(alphaRad);
		  gamePosition[1] -= sin(alphaRad);
		  if (maze.getAt((GLint)gamePosition[0], (GLint)gamePosition[1]) == true){
		    gamePosition[0] += cos(alphaRad);
		    gamePosition[1] += sin(alphaRad);
		  }
		  gameForward[0] = gamePosition[0] + cos(alphaRad);
		  gameForward[1] = gamePosition[1] + sin(alphaRad);
		}
		break;
	case GLUT_KEY_UP:
		if (!view){
		  gamePosition[0] += cos(alphaRad);
		  gamePosition[1] += sin(alphaRad);
		  if (maze.getAt((GLint)gamePosition[0], (GLint)gamePosition[1]) == true){
		    gamePosition[0] -= cos(alphaRad);
		    gamePosition[1] -= sin(alphaRad);
		  }
		  gameForward[0] = gamePosition[0] + cos(alphaRad);
		  gameForward[1] = gamePosition[1] + sin(alphaRad);
		}
		break;
	case GLUT_KEY_LEFT:
		if(!view){
		  alpha = (alpha + 5) % 360;
		  alphaRad = alpha * M_PI / 180;
		  gameForward[0] = gamePosition[0] + cos(alphaRad);
		  gameForward[1] = gamePosition[1] + sin(alphaRad);
		}
		break;
	case GLUT_KEY_RIGHT:
		if (!view){
		  alpha = (alpha - 5) % 360;
		  alphaRad = alpha * M_PI / 180;
		  gameForward[0] = gamePosition[0] + cos(alphaRad);
		  gameForward[1] = gamePosition[1] + sin(alphaRad);
		}
		break;
	case GLUT_KEY_PAGE_DOWN:
		if (view){
		  view = false;
		  for (int i = 0; i < 3; i++){
		    gamePosition[i] = savePosition[i];
		    gameForward[i] = saveForward[i];
		    gameNormal[i] = saveNormal[i];
		  }
		}
		break;
	case GLUT_KEY_PAGE_UP:
		if (!view){
		  view = true;
		  for (int i = 0; i < 3; i++){
		    savePosition[i] = gamePosition[i];
		    saveForward[i] = gameForward[i];
		    saveNormal[i] = gameNormal[i];
		  }
		  for (int i = 0; i < 3; i++){
		    gamePosition[i] = viewPosition[i];
		    gameForward[i] = viewForward[i];
		    gameNormal[i] = viewNormal[i];
		  }
		}
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