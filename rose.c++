// A simple OpenGL program that draws a something.

#include <iostream>
#include "GL/freeglut.h"
#include "GL/gl.h"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp> 
#include <glm/gtc/matrix_transform.hpp> 

using namespace glm;

void drawLife()
{
  glClearColor(.0, .0, .0, .0);
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(0.1, 0.0, 0.0);
  glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

  vec3 rotationAxis(0.0, 0.0, 1.0); // Z-axis
  mat4 translMatrix, rotMatrix, scaleMatrix;
  vec4 _vertex0 = vec4(0.0, 0.0, 0.0, 1.0);
  vec4 _vertex1 = vec4(0.0, 0.18, 0.0, 1.0);
  vec4 _vertex2 = vec4(0.18, 0.18, 0.0, 1.0);
  vec4 _vertex3 = vec4(0.18, 0.0, 0.0, 1.0);

  translMatrix = translate(mat4(1.0f), vec3(-0.45, -0.98, 0.0));
  rotMatrix    = rotate( (3.14f / 16.0f), rotationAxis);
  scaleMatrix  = scale(vec3(1.0f, 1.0f, 0.0f));

  vec4 vertex0 = translMatrix * rotMatrix * scaleMatrix * _vertex0;
  vec4 vertex1 = translMatrix * rotMatrix * scaleMatrix * _vertex1;
  vec4 vertex2 = translMatrix * rotMatrix * scaleMatrix * _vertex2;
  vec4 vertex3 = translMatrix * rotMatrix * scaleMatrix * _vertex3;

  float R = 0.1;
  float G = 0.0;
  float B = 0.0;
  float step = 0.009;

  for (size_t i = 0; i < 500; i++)
  {
    R += step;
    if (R > 1.0) {
      R = 1.0;
      G += step;
      if (G > 1.0) {
        G = 1.0;
        B += step;
      }
    }

    glColor3f(R, G, B);
    translMatrix = translate(mat4(1.0f), vec3(vertex2[0], vertex2[1], vertex2[2]));
    rotMatrix    = rotate(-1.0f+i*0.3f, rotationAxis);
    scaleMatrix  = scale(vec3(1.0f-i*0.0001f, 1.0f-i*0.0001f, 0.0f));

    _vertex0 = scaleMatrix * _vertex0;
    _vertex1 = scaleMatrix * _vertex1;
    _vertex2 = scaleMatrix * _vertex2;
    _vertex3 = scaleMatrix * _vertex3;

    vertex0 = translMatrix * rotMatrix * _vertex0;
    vertex1 = translMatrix * rotMatrix * _vertex1;
    vertex2 = translMatrix * rotMatrix * _vertex2;
    vertex3 = translMatrix * rotMatrix * _vertex3;

    glBegin(GL_POLYGON);
    glVertex3f(vertex0[0], vertex0[1], vertex0[2]);
    glVertex3f(vertex1[0], vertex1[1], vertex1[2]);
    glVertex3f(vertex2[0], vertex2[1], vertex2[2]);
    glVertex3f(vertex3[0], vertex3[1], vertex3[2]);
    glEnd();
  }

  glFlush();
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE);
  glutInitWindowSize(1000, 1000);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Rose");
  glutDisplayFunc(drawLife);
  glutMainLoop();
  return 0;
}