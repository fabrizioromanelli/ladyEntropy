// Making art with a heart.

#include <iostream>
#include "GL/freeglut.h"
#include "GL/gl.h"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp> 
#include <glm/gtc/matrix_transform.hpp> 

using namespace glm;

void drawCube(vec4 v0, vec4 v1, vec4 v2, vec4 v3) {
  glBegin(GL_LINE_STRIP);
  glVertex3f(v0[0], v0[1], v0[2]);
  glVertex3f(v1[0], v1[1], v1[2]);
  glVertex3f(v2[0], v2[1], v2[2]);
  glVertex3f(v3[0], v3[1], v3[2]);
  glVertex3f(v0[0], v0[1], v0[2]);
  glEnd();

  return;
}

void drawArt()
{
  glClearColor(1.0, 1.0, 1.0, .0);
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(0.0, 0.0, 0.0);
  glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

  float cubeSize = 0.05;

  vec3 rotationAxis(0.0, 0.0, 1.0); // Z-axis
  mat4 translMatrix, rotMatrix, scaleMatrix;
  vec4 _vertex0 = vec4(0.0, 0.0, 0.0, 1.0);
  vec4 _vertex1 = vec4(0.0, cubeSize, 0.0, 1.0);
  vec4 _vertex2 = vec4(cubeSize, cubeSize, 0.0, 1.0);
  vec4 _vertex3 = vec4(cubeSize, 0.0, 0.0, 1.0);

  translMatrix = translate(mat4(1.0f), vec3(-0.7, 0.7, 0.0));
  rotMatrix    = rotate( 1.0f, rotationAxis);

  vec4 vertex0 = translMatrix * rotMatrix * _vertex0;
  vec4 vertex1 = translMatrix * rotMatrix * _vertex1;
  vec4 vertex2 = translMatrix * rotMatrix * _vertex2;
  vec4 vertex3 = translMatrix * rotMatrix * _vertex3;

  glEnable( GL_LINE_SMOOTH );
  glLineWidth(0.5);
  glEnable(GL_BLEND);
  glDepthMask(false);
  glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
  
  drawCube(vertex0, vertex1, vertex2, vertex3);

  glFlush();
  return;
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE);
  glutInitWindowSize(1200, 1200);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Art");
  glutDisplayFunc(drawArt);
  glutMainLoop();
  return 0;
}