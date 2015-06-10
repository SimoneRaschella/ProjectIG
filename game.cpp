#include <GL/glut.h>
#include <cmath>
#include <cassert>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "maze.h"
#include "glm.h"
#include "imageloader.h"
using namespace std;

# define M_PI 3.14159265358979323846

GLuint texturePavimento;
GLuint textureMuro;
GLuint textureOro;

Image* pavimento;
Image* muro;
Image* oro;

GLint time_value = 25;
GLint cubeRotation = 0;
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
GLint targetPosition [2] = {0,0};
bool view = false;
const GLdouble DIMENSION_SQUARE = 1.0;
int NUMBER_CELL = 30;
const GLdouble DIMENSION_MAZE = DIMENSION_SQUARE * NUMBER_CELL; 
Maze maze(NUMBER_CELL);
bool inGame = true;
int life = 3;


void timer(int value)
{
  if (inGame){
      cubeRotation = (cubeRotation + 10) % 360;
      glutTimerFunc(time_value, timer, time_value);
  }
  else {
     view = true;
  }
  
  glutPostRedisplay();
}

GLuint loadTexture(Image* image)
{
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
                 //as unsigned numbers
				 image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

void init(){
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	GLfloat  ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f};
	GLfloat  specular[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat  specref[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	
	glEnable(GL_LIGHTING);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,ambientLight);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	glEnable(GL_LIGHT0);
	
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
 	glMaterialfv(GL_FRONT, GL_SPECULAR,specref);
  	glMateriali(GL_FRONT, GL_SHININESS,100);

	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_FOG);
	glFogf(GL_FOG_START, 1.0);
	glFogf(GL_FOG_END, 5.0);
	glFogf(GL_FOG_DENSITY, 1.0);
	
	glEnable(GL_TEXTURE_2D);
	
	pavimento=loadBMP("img/erba.bmp");
	texturePavimento=loadTexture(pavimento);
	
	muro = loadBMP("img/stones.bmp");
	textureMuro = loadTexture(muro);
	
 	oro = loadBMP("img/gold.bmp");
	textureOro = loadTexture(oro);
	
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
    if (inGame){
	fin.open("maze.txt");
	assert (fin.is_open());
    } else {
	fin.open("youWin.txt");
	assert(fin.is_open());
    }
	if (view){
	    glDisable(GL_FOG);
	} else {
	  glEnable(GL_FOG);
	}
	
	GLfloat fog_color[]= {0, 0, 0, 1.0};
	glFogfv(GL_FOG_COLOR, fog_color);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	
	
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	gluLookAt(gamePosition[0], gamePosition[1], gamePosition[2],
		  gameForward[0], gameForward[1], gameForward[2], 
		  gameNormal[0], gameNormal[1], gameNormal[2]);
	
	if (view && inGame){
	  glPushMatrix();
	    glTranslatef(savePosition[0], savePosition[1], savePosition[2]);
	    glColor3f(1, 0, 0);
	    glutSolidSphere(0.4, 15, 15);
	  glPopMatrix();
	  
	  glPushMatrix();
	    glColor3f(0.0, 1.0, 0.0);
	    for (int i = 0; i < life; i++){
	    glPushMatrix();
	    glTranslatef(4.0 + i*2, 2.0, 10.0);
	    glutSolidSphere(0.5, 10, 10);
	    glPopMatrix();
	    }
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
			    glBindTexture(GL_TEXTURE_2D, textureMuro);
			    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			    
			    for (int k = 0; k < 3; k++){
			      glPushMatrix();
			      glTranslatef(0.0, 0.0, DIMENSION_SQUARE*k);
			    //EST-FACE
 			    glBegin(GL_POLYGON);
			    glNormal3f(-1.0, 0.0, 0.0);
 			    glTexCoord2f(1.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
 			    glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 0.0, DIMENSION_SQUARE);
			    glTexCoord2f(0.0, 1.0); glVertex3f(0.0, DIMENSION_SQUARE, DIMENSION_SQUARE);
			    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, DIMENSION_SQUARE, 0.0);
			    glEnd();
			    //NORTH-FACE
 			    glBegin(GL_POLYGON);
			    glNormal3f(0.0, 1.0, 0.0);
 			    glTexCoord2f(1.0, 0.0); glVertex3f(0.0, DIMENSION_SQUARE, 0.0);
			    glTexCoord2f(1.0, 1.0); glVertex3f(0.0, DIMENSION_SQUARE, DIMENSION_SQUARE);
			    glTexCoord2f(0.0, 1.0); glVertex3f(DIMENSION_SQUARE, DIMENSION_SQUARE, DIMENSION_SQUARE);
 			    glTexCoord2f(0.0, 0.0); glVertex3f(DIMENSION_SQUARE, DIMENSION_SQUARE, 0.0);
 			    glEnd();
			    //WEST-FACE
 			    glBegin(GL_POLYGON);
			    glNormal3f(1.0, 0.0, 0.0);
  			    glTexCoord2f(0.0, 0.0); glVertex3f(DIMENSION_SQUARE, 0.0, 0.0);
  			    glTexCoord2f(1.0, 0.0); glVertex3f(DIMENSION_SQUARE, DIMENSION_SQUARE, 0.0);
  			    glTexCoord2f(1.0, 1.0); glVertex3f(DIMENSION_SQUARE, DIMENSION_SQUARE, DIMENSION_SQUARE);
  			    glTexCoord2f(0.0, 1.0); glVertex3f(DIMENSION_SQUARE, 0.0, DIMENSION_SQUARE);
  			    glEnd();
			    //SOUTH-FACE
			    glBegin(GL_POLYGON);
			    glNormal3f(0.0, -1.0, 0.0);
 			    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
 			    glTexCoord2f(1.0, 0.0); glVertex3f(DIMENSION_SQUARE, 0.0, 0.0);
 			    glTexCoord2f(1.0, 1.0); glVertex3f(DIMENSION_SQUARE, 0.0, DIMENSION_SQUARE);
 			    glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 0.0, DIMENSION_SQUARE);
 			    glEnd();
			    //UP-FACE
			    glBegin(GL_POLYGON);
 			    glVertex3f(0.0, 0.0, DIMENSION_SQUARE);
 			    glVertex3f(0.0, DIMENSION_SQUARE, DIMENSION_SQUARE);
 			    glVertex3f(DIMENSION_SQUARE, DIMENSION_SQUARE, DIMENSION_SQUARE);
 			    glVertex3f(DIMENSION_SQUARE, 0.0, DIMENSION_SQUARE);
 			    glEnd();
			    glPopMatrix();
			    }
			  glPopMatrix();
			} else {
			  
			  if (number == '2'){
			    targetPosition[0] = j;
			    targetPosition[1] = i;
			   
			    glPushMatrix();
			    glBindTexture(GL_TEXTURE_2D, textureOro);
			    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			      glTranslatef(0.5, 0.5, 0.1);
			      glRotatef ((GLfloat) cubeRotation, 1.0, 1.0, 1.0);
			      glPushMatrix();
 			      glDisable(GL_LIGHTING);
			      //EST-FACE
			      glBegin(GL_POLYGON);
			    glNormal3f(-1.0, 0.0, 0.0);
 			    glTexCoord2f(1.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
 			    glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 0.0, DIMENSION_SQUARE/3);
			    glTexCoord2f(0.0, 1.0); glVertex3f(0.0, DIMENSION_SQUARE/3, DIMENSION_SQUARE/3);
			    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, DIMENSION_SQUARE/3, 0.0);
			    glEnd();
			    //NORTH-FACE
 			    glBegin(GL_POLYGON);
			    glNormal3f(0.0, 1.0, 0.0);
 			    glTexCoord2f(1.0, 0.0); glVertex3f(0.0, DIMENSION_SQUARE/3, 0.0);
			    glTexCoord2f(1.0, 1.0); glVertex3f(0.0, DIMENSION_SQUARE/3, DIMENSION_SQUARE/3);
			    glTexCoord2f(0.0, 1.0); glVertex3f(DIMENSION_SQUARE/3, DIMENSION_SQUARE/3, DIMENSION_SQUARE/3);
 			    glTexCoord2f(0.0, 0.0); glVertex3f(DIMENSION_SQUARE/3, DIMENSION_SQUARE/3, 0.0);
 			    glEnd();
			    //WEST-FACE
 			    glBegin(GL_POLYGON);
			    glNormal3f(1.0, 0.0, 0.0);
  			    glTexCoord2f(0.0, 0.0); glVertex3f(DIMENSION_SQUARE/3, 0.0, 0.0);
  			    glTexCoord2f(1.0, 0.0); glVertex3f(DIMENSION_SQUARE/3, DIMENSION_SQUARE/3, 0.0);
  			    glTexCoord2f(1.0, 1.0); glVertex3f(DIMENSION_SQUARE/3, DIMENSION_SQUARE/3, DIMENSION_SQUARE/3);
  			    glTexCoord2f(0.0, 1.0); glVertex3f(DIMENSION_SQUARE/3, 0.0, DIMENSION_SQUARE/3);
  			    glEnd();
			    //SOUTH-FACE
			    glBegin(GL_POLYGON);
			    glNormal3f(0.0, -1.0, 0.0);
 			    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
 			    glTexCoord2f(1.0, 0.0); glVertex3f(DIMENSION_SQUARE/3, 0.0, 0.0);
 			    glTexCoord2f(1.0, 1.0); glVertex3f(DIMENSION_SQUARE/3, 0.0, DIMENSION_SQUARE/3);
 			    glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 0.0, DIMENSION_SQUARE/3);
 			    glEnd();
			    //UP-FACE
			    glBegin(GL_POLYGON);
 			    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, DIMENSION_SQUARE/3);
			    glTexCoord2f(1.0, 0.0); glVertex3f(DIMENSION_SQUARE/3, 0.0, DIMENSION_SQUARE/3);
			    glTexCoord2f(1.0, 1.0); glVertex3f(DIMENSION_SQUARE/3, DIMENSION_SQUARE/3, DIMENSION_SQUARE/3);
 			    glTexCoord2f(0.0, 1.0); glVertex3f(0.0, DIMENSION_SQUARE/3, DIMENSION_SQUARE/3);
 			    glEnd();
			    //DOWN-FACE
			    glBegin(GL_POLYGON);
 			    glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 0.0, 0.0);
			    glTexCoord2f(0.0, 0.0); glVertex3f(0.0, DIMENSION_SQUARE/3, 0.0);
			    glTexCoord2f(1.0, 0.0); glVertex3f(DIMENSION_SQUARE/3, DIMENSION_SQUARE/3, 0.0);
 			    glTexCoord2f(1.0, 1.0); glVertex3f(DIMENSION_SQUARE/3, 0.0, 0.0);
 			    glEnd();
			    glPopMatrix();
 			      glEnable(GL_LIGHTING);
 			    glPopMatrix();
			  }
			  glBindTexture(GL_TEXTURE_2D, texturePavimento);
			  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			  maze.setAt(j,i,false);
			  
			  glBegin(GL_QUADS);
			    glNormal3f(0.0, 0.0, 1.0);
			    glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0, 0.0);
			    glTexCoord2f(1.0f, 0.0f); glVertex2f(DIMENSION_SQUARE, 0.0);
			    glTexCoord2f(1.0f, 1.0f); glVertex2f(DIMENSION_SQUARE, DIMENSION_SQUARE);
			    glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0, DIMENSION_SQUARE);
			  glEnd();
			}
			glTranslatef(DIMENSION_SQUARE,0.0, 0.0);
		}
		glTranslatef(0.0, DIMENSION_SQUARE, 0.0);
		glTranslatef((-1)*DIMENSION_MAZE, 0.0, 0.0);
	}
	glPopMatrix();
	
	glPopMatrix();
    
	if (!inGame && view == false){
	  glPushMatrix();
	  view = true;
	  for (int i = 0; i < 3; i++){
	      gamePosition[i] = viewPosition[i];
	      gameForward[i] = viewForward[i];
	      gameNormal[i] = viewNormal[i];
	  }
	  gluLookAt(gamePosition[0], gamePosition[1], gamePosition[2],
		  gameForward[0], gameForward[1], gameForward[2], 
		  gameNormal[0], gameNormal[1], gameNormal[2]);
	  glPopMatrix();
	}
      
      glutSwapBuffers();
}




void SpecialKeys(int key, int x, int y){
      GLint tmpX;
      GLint tmpY;
      GLdouble tmpgamePosition0;
      GLdouble tmpgamePosition1;
	switch (key){
	case GLUT_KEY_DOWN:
		if (!view && inGame){
		  tmpgamePosition0 = gamePosition[0] - cos(alphaRad) * 0.5;
		  tmpgamePosition1 = gamePosition[1] - sin(alphaRad) * 0.5;
		  if (maze.getAt((GLint)tmpgamePosition0, (GLint)tmpgamePosition1) == false){
		    gamePosition[0] -= cos(alphaRad) * 0.1; 
		    gamePosition[1] -= sin(alphaRad) * 0.1;
		    gameForward[0] = gamePosition[0] + cos(alphaRad) * 0.1;
		    gameForward[1] = gamePosition[1] + sin(alphaRad) * 0.1;
		  } 
		}
		break;
	case GLUT_KEY_UP:
		if (!view && inGame){
		  tmpgamePosition0 = gamePosition[0] + cos(alphaRad) * 0.5;
		  tmpgamePosition1 = gamePosition[1] + sin(alphaRad) * 0.5;
		  if (maze.getAt((GLint)tmpgamePosition0, (GLint)tmpgamePosition1) == false){
		    gamePosition[0] += cos(alphaRad) * 0.1; 
		    gamePosition[1] += sin(alphaRad) * 0.1;
		    gameForward[0] = gamePosition[0] + cos(alphaRad) * 0.1;
		    gameForward[1] = gamePosition[1] + sin(alphaRad) * 0.1;
		  } 
		  if ((GLint)gamePosition[0]  == targetPosition[0] && (GLint)gamePosition[1] == targetPosition[1]){
		    inGame = false;
		  }
		}
		break;
	case GLUT_KEY_LEFT:
		if(!view && inGame){
		  alpha = (alpha + 2) % 360;
		  alphaRad = alpha * M_PI / 180;
		  gameForward[0] = gamePosition[0] + cos(alphaRad) * 0.1;
		  gameForward[1] = gamePosition[1] + sin(alphaRad) * 0.1;
		}
		break;
	case GLUT_KEY_RIGHT:
		if (!view && inGame){
		  alpha = (alpha - 2) % 360;
		  alphaRad = alpha * M_PI / 180;
		  gameForward[0] = gamePosition[0] + cos(alphaRad) * 0.1;
		  gameForward[1] = gamePosition[1] + sin(alphaRad) * 0.1;
		}
		break;
	case GLUT_KEY_PAGE_DOWN:
		if (view && inGame){
		  view = false;
		  for (int i = 0; i < 3; i++){
		    gamePosition[i] = savePosition[i];
		    gameForward[i] = saveForward[i];
		    gameNormal[i] = saveNormal[i];
		  }
		  life--;
		}
		
		break;
	case GLUT_KEY_PAGE_UP:
		if (!view && inGame && life > 0){
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
	glutTimerFunc(time_value, timer, time_value);
	glutMainLoop();
	return 0; 
}