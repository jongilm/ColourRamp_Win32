/*****************************************************************************
 * FILE NAME     : display.cpp
 * MODULE        : Display
 * AUTHOR        : unknown employer???
 * CREATION DATE : 13/11/2011
 *
 *****************************************************************************
 *
 * DESCRIPTION : Dummy Implementation of the Simple Display class
 *
 *****************************************************************************
 */

#include <cstdio>
#include <cassert>
#include <memory.h>
#include <stdio.h>

#include "display.h"

#define UNUSED_PARAMETER(a) ((void)a)
#define NUM_ELEM(x) (sizeof(x)/sizeof(x[0]))

#define WIDTH  (16)
#define HEIGHT (9)

static unsigned short frame_buffer[WIDTH*HEIGHT];

Display::Display()
{
  memset(frame_buffer, 0, sizeof(frame_buffer));
}

Display::~Display()
{
   unsigned short *pPix = frame_buffer;

   for (int y = 0; y < HEIGHT; y++)
   {
      for (int x = 0; x < WIDTH; x++)
      {
         if (x > 0)
         {
            printf(" ");
         }
         printf("%04X", *pPix++);
      }
      printf("\n");
   }
}

bool Display::connect(const char *display_name)
{
  UNUSED_PARAMETER(display_name);
  return true;
}

void Display::get_size(int &width, int &height)
{
   width = WIDTH;
   height = HEIGHT;
}

void Display::draw_raster(int x, int y, const unsigned short *pixels, int width)
{
   memcpy(&frame_buffer[y*WIDTH + x], pixels, width * sizeof(unsigned short));
}

#ifdef DISPLAY_EXTENSION
bool Display::create_bmp(const char *filename)
{
   // Ref: http://msdn.microsoft.com/en-us/library/aa446563.aspx#scibf_topic3
   // BMP Version 3

#pragma pack(2)
   typedef unsigned long DWORD;
   typedef signed long   LONG;
   typedef unsigned short WORD;

   typedef struct tagBITMAPFILEHEADER
   {
     WORD  bfType;
     DWORD bfSize;
     WORD  bfReserved1;
     WORD  bfReserved2;
     DWORD bfOffBits;
   } BITMAPFILEHEADER;

   typedef struct _WinNtBitmapHeader
   {
      DWORD Size;            /* Size of this header in bytes */
      LONG  Width;           /* Image width in pixels */
      LONG  Height;          /* Image height in pixels */
      WORD  Planes;          /* Number of color planes */
      WORD  BitsPerPixel;    /* Number of bits per pixel */
      DWORD Compression;     /* Compression methods used */
      DWORD SizeOfBitmap;    /* Size of bitmap in bytes */
      LONG  HorzResolution;  /* Horizontal resolution in pixels per meter */
      LONG  VertResolution;  /* Vertical resolution in pixels per meter */
      DWORD ColorsUsed;      /* Number of colors in the image */
      DWORD ColorsImportant; /* Minimum number of important colors */
   } WINNTBITMAPHEADER;

   FILE *f;
   BITMAPFILEHEADER file_hdr;
   WINNTBITMAPHEADER bmp_hdr;
#pragma pack()

   f = fopen(filename,"wb");
   if (!f)
   {
      /* Test code only. Just abort the mission */
      return false;
   }

   file_hdr.bfType      = 0x4D42; // "BM"
   file_hdr.bfSize      = sizeof(BITMAPFILEHEADER) + sizeof(WINNTBITMAPHEADER) + sizeof(frame_buffer);
   file_hdr.bfReserved1 = 0;
   file_hdr.bfReserved2 = 0;
   file_hdr.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(WINNTBITMAPHEADER);

   bmp_hdr.Size            = sizeof(WINNTBITMAPHEADER);
   bmp_hdr.Width           = WIDTH;
   bmp_hdr.Height          = HEIGHT;
   bmp_hdr.Planes          = 1;
   bmp_hdr.BitsPerPixel    = 16;
   bmp_hdr.Compression     = 0;
   bmp_hdr.SizeOfBitmap    = sizeof(frame_buffer);
   bmp_hdr.HorzResolution  = 0;
   bmp_hdr.VertResolution  = 0;
   bmp_hdr.ColorsUsed      = 0;
   bmp_hdr.ColorsImportant = 0;

   /* It seems that very few graphics viewers know how to display a 565 BMP properly. */
   fwrite(&file_hdr,sizeof(file_hdr),1,f);
   fwrite(&bmp_hdr,sizeof(bmp_hdr),1,f);
   fwrite(frame_buffer,sizeof(frame_buffer),1,f);

   fclose(f);

   return true;
}

#endif // DISPLAY_EXTENSION
