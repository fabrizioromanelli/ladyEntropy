/**
    lady_entropy.c++
    Purpose: Making art with a ♥. This program uses OpenGL to display and save to a file a matrix of 2D cubes.
    The cubes are organized per-row and per-column and at each column it adds increasing random noise.
    Options:
      -type display|file
      -row  <positive integer>
      -col  <positive integer>
      -size <positive float>

    @author Fabrizio Romanelli
    @version 1.0 03/01/20 
*/

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <bits/stdc++.h>
#include <iostream>
#include <string>
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

// Global variables
bool isFile = false;
int ROW     = 0;
int COL     = 0;
float SIZE  = 0.0;

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

void saveImage(int r, int c, float s)
{
  char filename[80];
  unsigned char* image = (unsigned char*)malloc(sizeof(unsigned char) * 3 * IMG_SIZE_X * IMG_SIZE_Y);
  glReadPixels(0, 0, IMG_SIZE_X - 1, IMG_SIZE_Y - 1, GL_RGB, GL_UNSIGNED_BYTE, image);

  strcpy(filename, "image-R");
  strcat(filename, std::to_string(r).c_str());
  strcat(filename, "-C");
  strcat(filename, std::to_string(c).c_str());
  strcat(filename, "-S");
  strcat(filename, std::to_string(s).c_str());
  strcat(filename, ".bmp");

  // PPMWriter(image, "image.ppm", IMG_SIZE_X, IMG_SIZE_Y);
  BMPWriter(filename, IMG_SIZE_X, IMG_SIZE_Y, image);
  return;
}

void drawArt()
{
  srand(time(NULL));
  glClearColor(1.0, 1.0, 1.0, .0);
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(0.0, 0.0, 0.0);
  glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

  int row_size, col_size;
  float cube_size;

  if (ROW > 0) {
    row_size = ROW;
  } else {
    row_size = ROW_SIZE;
  }

  if (COL > 0) {
    col_size = COL;
  } else {
    col_size = COL_SIZE;
  }

  if (SIZE > 0.0) {
    cube_size = SIZE;
  } else {
    cube_size = CUBE_SIZE;
  }

  size_t matSize = 0;
  float horizontalChaos = 0.0, rotoChaos = 0.0, skewChaos = 0.0, verticalChaos = 0.0;

  vec3 rotationAxis(0.0, 0.0, 1.0); // Z-axis
  mat4 translMatrix, rotMatrix, scaleMatrix;
  vec4 _vertex0 = vec4(0.0, 0.0, 0.0, 1.0);
  vec4 _vertex1 = vec4(0.0, cube_size, 0.0, 1.0);
  vec4 _vertex2 = vec4(cube_size, cube_size, 0.0, 1.0);
  vec4 _vertex3 = vec4(cube_size, 0.0, 0.0, 1.0);
  vec4 v0[row_size * col_size], v1[row_size * col_size], v2[row_size * col_size], v3[row_size * col_size];

  for (size_t i = 0; i < row_size; i++)
  {
    // Computing vertical chaos factor based on the row position
    float vChaosFactor = (float)MAX_VERT_CHAOS * (1 - (2 * (float)i / (float)row_size));

    for (size_t j = 0; j < col_size; j++)
    {
      verticalChaos = RandomFloat(0.0, vChaosFactor * (j + 1) / col_size);
      translMatrix  = translate(mat4(1.0f), vec3(-0.98 + cube_size * j + horizontalChaos, 0.8 - cube_size * i + verticalChaos, 0.0));
      rotMatrix     = rotate(rotoChaos, rotationAxis); // radians - range for random rotoChaos [MIN_ROTO_CHAOS .. MAX_ROTO_CHAOS]

      v0[matSize] = translMatrix * rotMatrix * _vertex0;
      v1[matSize] = translMatrix * rotMatrix * _vertex1;
      v2[matSize] = translMatrix * rotMatrix * _vertex2;
      v3[matSize] = translMatrix * rotMatrix * _vertex3;

      // Apply skew chaos to roto-translated cubes
      if (j != 0) {
        v0[matSize][X] += RandomFloat(MIN_SKEW_CHAOS, MAX_SKEW_CHAOS * (j + 1) / col_size ) * randomSign();
        v0[matSize][Y] += RandomFloat(MIN_SKEW_CHAOS, MAX_SKEW_CHAOS * (j + 1) / col_size ) * randomSign();
        v1[matSize][X] += RandomFloat(MIN_SKEW_CHAOS, MAX_SKEW_CHAOS * (j + 1) / col_size ) * randomSign();
        v1[matSize][Y] += RandomFloat(MIN_SKEW_CHAOS, MAX_SKEW_CHAOS * (j + 1) / col_size ) * randomSign();
        v2[matSize][X] += RandomFloat(MIN_SKEW_CHAOS, MAX_SKEW_CHAOS * (j + 1) / col_size ) * randomSign();
        v2[matSize][Y] += RandomFloat(MIN_SKEW_CHAOS, MAX_SKEW_CHAOS * (j + 1) / col_size ) * randomSign();
        v3[matSize][X] += RandomFloat(MIN_SKEW_CHAOS, MAX_SKEW_CHAOS * (j + 1) / col_size ) * randomSign();
        v3[matSize][Y] += RandomFloat(MIN_SKEW_CHAOS, MAX_SKEW_CHAOS * (j + 1) / col_size ) * randomSign();
      }

      drawCube(v0[matSize], v1[matSize], v2[matSize], v3[matSize]);
      matSize++;
      // let's add some chaos!
      horizontalChaos = RandomFloat(MIN_TRAS_CHAOS * (j + 1) / col_size, MAX_TRAS_CHAOS * (j + 1) / col_size );
      rotoChaos = RandomFloat(MIN_ROTO_CHAOS, MAX_ROTO_CHAOS * (j + 1) / col_size ) * randomSign();
    }
    // reset chaos
    horizontalChaos = 0.0;
    verticalChaos   = 0.0;
    rotoChaos       = 0.0;
  }

  // We just save the image over here
  if (isFile) {
    saveImage(row_size, col_size, cube_size);
    exit(0);
  }

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

  // Manage here the command line.
  for (int i = 1; i < argc; i+=2)
  {
    if (!strcmp(argv[i], "-type")) {
      if (!strcmp(argv[i+1], "display")) {
        isFile = false;
      } else {
        isFile = true;
      }
    } else if (!strcmp(argv[i], "-row")) {
      try
      {
        int v = std::stoi(argv[i+1]);
        ROW = v;
      }
      catch (std::invalid_argument const &e)
      {
        std::cout << "Bad input: std::invalid_argument thrown" << '\n';
      }
      catch (std::out_of_range const &e)
      {
        std::cout << "Integer overflow: std::out_of_range thrown" << '\n';
      }
    } else if (!strcmp(argv[i], "-col")) {
      try
      {
        int v = std::stoi(argv[i+1]);
        COL = v;
      }
      catch (std::invalid_argument const &e)
      {
        std::cout << "Bad input: std::invalid_argument thrown" << '\n';
      }
      catch (std::out_of_range const &e)
      {
        std::cout << "Integer overflow: std::out_of_range thrown" << '\n';
      }
    } else if (!strcmp(argv[i], "-size")) {
      try
      {
        float v = std::stof(argv[i+1]);
        SIZE = v;
      }
      catch (std::invalid_argument const &e)
      {
        std::cout << "Bad input: std::invalid_argument thrown" << '\n';
      }
      catch (std::out_of_range const &e)
      {
        std::cout << "Integer overflow: std::out_of_range thrown" << '\n';
      }
    }
  }

  if (isFile) {
    // Set anti-aliasing for virtual display :1
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glLineWidth(3.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_MULTISAMPLE);
  } else {
    // Set Multisample anti-aliasing for physical display :0
    glutSetOption(GLUT_MULTISAMPLE, 4);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glEnable(GL_MULTISAMPLE);
  }

  glutDisplayFunc(drawArt);
  glutMainLoop();
  return 0;
}