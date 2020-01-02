// Making art with a heart.

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <bits/stdc++.h>
#include <iostream>
#include "GL/freeglut.h"
#include "GL/gl.h"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp> 
#include <glm/gtc/matrix_transform.hpp> 

// Local includes
#include "includes/constants.hpp"
#include "includes/typedefs.hpp"

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

void PPMWriter(unsigned char *in, char *name, int dimx, int dimy)
{
  int i, j;
  FILE *fp = fopen(name, "wb"); /* b - binary mode */
  (void) fprintf(fp, "P6 %d %d 255", dimx, dimy);
  for (j = 0; j < dimy; ++j)
  {
    for (i = 0; i < dimx; ++i)
    {
      static unsigned char color[3];
      color[0] = in[3*i+3*j*dimy];  /* red */
      color[1] = in[3*i+3*j*dimy+1];  /* green */
      color[2] = in[3*i+3*j*dimy+2];  /* blue */
      (void) fwrite(color, 1, 3, fp);
    }
  }
  (void) fclose(fp);
}

int BMPWriter(const char *filename, int width, int height, unsigned char *imageData) {
  FILE *fPtr;
  BITMAPFILEHEADER bitmapFileHeader;
  BITMAPINFOHEADER bitmapInfoHeader;
  int imageIdx;
  int size = width * height * 3;
  unsigned char tempRGB;

  fPtr = fopen(filename, "wb");

  if(!fPtr) return 0;

  bitmapFileHeader.bfType = 0x4D42;
  bitmapFileHeader.bfSize = size + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
  bitmapFileHeader.bfReserved1 = 0;
  bitmapFileHeader.bfReserved2 = 0;
  bitmapFileHeader.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

  bitmapInfoHeader.biSize      = 40;
  bitmapInfoHeader.biPlanes    = 1;
  bitmapInfoHeader.biBitCount  = 24;
  bitmapInfoHeader.biCompression = 0x0000; // no compression
  bitmapInfoHeader.biSizeImage  = size;
  bitmapInfoHeader.biXPelsPerMeter = 0;
  bitmapInfoHeader.biYPelsPerMeter = 0;
  bitmapInfoHeader.biClrUsed       = 0;
  bitmapInfoHeader.biClrImportant  = 0;
  bitmapInfoHeader.biWidth         = width;
  bitmapInfoHeader.biHeight        = height;

  for (imageIdx = 0; imageIdx < bitmapInfoHeader.biSizeImage; imageIdx+=3) {
    tempRGB = imageData[imageIdx];
    imageData[imageIdx] = imageData[imageIdx + 2];
    imageData[imageIdx + 2] = tempRGB;
  }

  fwrite(&bitmapFileHeader, 1, sizeof(BITMAPFILEHEADER), fPtr);
  fwrite(&bitmapInfoHeader, 1, sizeof(BITMAPINFOHEADER), fPtr);
  fwrite(imageData, 1, size, fPtr);
  fclose(fPtr);

  return 1;
}

void saveImage()
{
  unsigned char* image = (unsigned char*)malloc(sizeof(unsigned char) * 3 * IMG_SIZE_X * IMG_SIZE_Y);
  glReadPixels(0, 0, IMG_SIZE_X - 1, IMG_SIZE_Y - 1, GL_RGB, GL_UNSIGNED_BYTE, image);
  // PPMWriter(image, "image.ppm", IMG_SIZE_X, IMG_SIZE_Y);
  BMPWriter("image.bmp", IMG_SIZE_X, IMG_SIZE_Y, image);
  return;
}

void drawArt()
{
  srand(time(NULL));
  glClearColor(1.0, 1.0, 1.0, .0);
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(0.0, 0.0, 0.0);
  glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

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
      translMatrix = translate(mat4(1.0f), vec3(-0.98 + CUBE_SIZE * j + trasChaos, 0.8 - CUBE_SIZE * i, 0.0));
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

  // We just save the image over here
  // saveImage();

  glFlush();
  return;
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_MULTISAMPLE);
  glutInitWindowSize(IMG_SIZE_X, IMG_SIZE_Y);
  glutInitWindowPosition(1000, 100);
  glutCreateWindow("Art");

  // Manage here the command line
  for (int i = 0; i < argc; i++)
  {
    std::cout << argv[i] << std::endl;
  }

  // Choose here based on the input argument

  // Set Multisample anti-aliasing for physical display :0
  // glutSetOption(GLUT_MULTISAMPLE, 4);
  // glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
  // glEnable(GL_MULTISAMPLE);

  // Set anti-aliasing for virtual display :1
  // glEnable(GL_LINE_SMOOTH);
  // glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  // glLineWidth(0.5);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // glDisable(GL_MULTISAMPLE);

  glutDisplayFunc(drawArt);
  glutMainLoop();
  return 0;
}