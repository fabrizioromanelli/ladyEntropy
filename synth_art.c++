// Making art with a heart.

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
#include "GL/freeglut.h"
#include "GL/gl.h"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp> 
#include <glm/gtc/matrix_transform.hpp> 

#define X              0
#define Y              1
#define ROW_SIZE       10
#define COL_SIZE       30
#define CUBE_SIZE      0.05
#define MIN_TRAS_CHAOS -CUBE_SIZE/10
#define MAX_TRAS_CHAOS CUBE_SIZE/2
#define MIN_ROTO_CHAOS 0.0
#define MAX_ROTO_CHAOS 0.785
#define MIN_SKEW_CHAOS 0.0
#define MAX_SKEW_CHAOS CUBE_SIZE/5

// #define MAX_ROTO_CHAOS 0.0
// #define MIN_TRAS_CHAOS 0.0
// #define MAX_TRAS_CHAOS 0.0
// #define MAX_SKEW_CHAOS 0.0

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

float RandomFloat(float a, float b) {
  float random = ((float) rand()) / (float) RAND_MAX;
  float diff = b - a;
  float r = random * diff;
  return a + r;
}

int randomSign() {
  int randomSign = rand() % 3 + (-1);
  if (randomSign == 0) {
    randomSign = 1;
  }
  return randomSign;
}

void drawArt()
{
  srand(time(NULL));
  glClearColor(1.0, 1.0, 1.0, .0);
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(0.0, 0.0, 0.0);
  glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA_SATURATE, GL_ONE);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glLineWidth(0.5);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

  size_t matSize = 0;
  float trasChaos = 0.0, rotoChaos = 0.0, skewChaos = 0.0;

  vec3 rotationAxis(0.0, 0.0, 1.0); // Z-axis
  mat4 translMatrix, rotMatrix, scaleMatrix;
  vec4 _vertex0 = vec4(0.0, 0.0, 0.0, 1.0);
  vec4 _vertex1 = vec4(0.0, CUBE_SIZE, 0.0, 1.0);
  vec4 _vertex2 = vec4(CUBE_SIZE, CUBE_SIZE, 0.0, 1.0);
  vec4 _vertex3 = vec4(CUBE_SIZE, 0.0, 0.0, 1.0);
  vec4 v0[ROW_SIZE * COL_SIZE], v1[ROW_SIZE * COL_SIZE], v2[ROW_SIZE * COL_SIZE], v3[ROW_SIZE * COL_SIZE];

  for (size_t i = 0; i < ROW_SIZE; i++)
  {
    for (size_t j = 0; j < COL_SIZE; j++)
    {
      translMatrix = translate(mat4(1.0f), vec3(-0.7 + CUBE_SIZE * j + trasChaos, 0.7 - CUBE_SIZE * i, 0.0));
      rotMatrix    = rotate(rotoChaos, rotationAxis); // radians - range for random rotoChaos [MIN_ROTO_CHAOS .. MAX_ROTO_CHAOS]

      v0[matSize] = translMatrix * rotMatrix * _vertex0;
      v1[matSize] = translMatrix * rotMatrix * _vertex1;
      v2[matSize] = translMatrix * rotMatrix * _vertex2;
      v3[matSize] = translMatrix * rotMatrix * _vertex3;

      // Apply skew chaos to roto-translated cubes
      if (j != 0) {
        v0[matSize][X] += RandomFloat(MIN_SKEW_CHAOS, MAX_SKEW_CHAOS * (j + 1) / COL_SIZE ) * randomSign();
        v0[matSize][Y] += RandomFloat(MIN_SKEW_CHAOS, MAX_SKEW_CHAOS * (j + 1) / COL_SIZE ) * randomSign();
        v1[matSize][X] += RandomFloat(MIN_SKEW_CHAOS, MAX_SKEW_CHAOS * (j + 1) / COL_SIZE ) * randomSign();
        v1[matSize][Y] += RandomFloat(MIN_SKEW_CHAOS, MAX_SKEW_CHAOS * (j + 1) / COL_SIZE ) * randomSign();
        v2[matSize][X] += RandomFloat(MIN_SKEW_CHAOS, MAX_SKEW_CHAOS * (j + 1) / COL_SIZE ) * randomSign();
        v2[matSize][Y] += RandomFloat(MIN_SKEW_CHAOS, MAX_SKEW_CHAOS * (j + 1) / COL_SIZE ) * randomSign();
        v3[matSize][X] += RandomFloat(MIN_SKEW_CHAOS, MAX_SKEW_CHAOS * (j + 1) / COL_SIZE ) * randomSign();
        v3[matSize][Y] += RandomFloat(MIN_SKEW_CHAOS, MAX_SKEW_CHAOS * (j + 1) / COL_SIZE ) * randomSign();
      }

      drawCube(v0[matSize], v1[matSize], v2[matSize], v3[matSize]);
      matSize++;
      // let's add some chaos!
      trasChaos = RandomFloat(MIN_TRAS_CHAOS * (j + 1) / COL_SIZE, MAX_TRAS_CHAOS * (j + 1) / COL_SIZE );
      rotoChaos = RandomFloat(MIN_ROTO_CHAOS, MAX_ROTO_CHAOS * (j + 1) / COL_SIZE ) * randomSign();
    }
    // reset chaos
    trasChaos = 0.0;
    rotoChaos = 0.0;
  }

  glFlush();
  return;
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE);
  glutInitWindowSize(1200, 1200);
  glutInitWindowPosition(1000, 100);
  glutCreateWindow("Art");
  glutDisplayFunc(drawArt);
  glutMainLoop();
  return 0;
}