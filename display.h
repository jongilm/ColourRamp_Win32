/*****************************************************************************
 * FILE NAME     : display.h
 * MODULE        : Display
 * AUTHOR        : unknown
 * CREATION DATE : 2011/11/12
 *
 *****************************************************************************
 *
 * DESCRIPTION : Interface of the simple Display class
 * 
 * REFERENCES  : 
 * 
 *****************************************************************************/

//#define DISPLAY_EXTENSION

class Display
{
   public:
      Display();
      ~Display();

      bool connect(const char *display_name);
      void get_size(int &width, int &height);
      void draw_raster(int x, int y, const unsigned short *pixels, int width);
#ifdef DISPLAY_EXTENSION
      bool Display::create_bmp(const char *filename);
#endif
};
