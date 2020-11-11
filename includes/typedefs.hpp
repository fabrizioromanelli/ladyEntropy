typedef int LE_LONG;
typedef unsigned short LE_WORD;
typedef unsigned int LE_DWORD;

typedef struct tagBITMAPFILEHEADER {
  LE_WORD  bfType;
  LE_DWORD bfSize;
  LE_WORD  bfReserved1;
  LE_WORD  bfReserved2;
  LE_DWORD bfOffBits;
} __attribute__((packed)) BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
  LE_DWORD biSize;
  LE_LONG  biWidth;
  LE_LONG  biHeight;
  LE_WORD  biPlanes;
  LE_WORD  biBitCount;
  LE_DWORD biCompression;
  LE_DWORD biSizeImage;
  LE_LONG  biXPelsPerMeter;
  LE_LONG  biYPelsPerMeter;
  LE_DWORD biClrUsed;
  LE_DWORD biClrImportant;
} __attribute__((packed)) BITMAPINFOHEADER;
