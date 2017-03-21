/*****************************************************************************
 * FILE NAME     : colour_gradient.cpp
 * MODULE        : ColourGradient
 * AUTHOR        : Jonathan Gilmore
 * CREATION DATE : 13/11/2011
 *
 *****************************************************************************
 *
 * DESCRIPTION : Fill the screen with a colour ramp as defined by the input
 *               values. Each corner of the screen should have the specified
 *               colour and the pixels in the middle should be the appropriate
 *               linear mix of those colours.
 *
 *****************************************************************************
 */

#include "stdafx.h"
#include <string>
#include <cmath>

#include "display.h"
#include "colour_gradient.h"

colour_gradient::colour_gradient()
{
   initialise();
}

colour_gradient::colour_gradient(Display *argDisplay)
{
   initialise();
   pDisplay = argDisplay;
}

colour_gradient::colour_gradient ( Display *argDisplay,
                                   unsigned short argTopLeft,
                                   unsigned short argTopRight,
                                   unsigned short argBottomRight,
                                   unsigned short argBottomLeft )
{
   initialise();
   set_display ( argDisplay );
   set_colours ( argTopLeft,
                 argTopRight,
                 argBottomRight,
                 argBottomLeft );
}

void colour_gradient::initialise(void)
{
   ErrNo          = ERR_OK;
   pDisplay       = NULL;
   rgbTopLeft     = 0;
   rgbTopRight    = 0;
   rgbBottomRight = 0;
   rgbBottomLeft  = 0;
}

void colour_gradient::set_display ( Display *argDisplay )
{
   pDisplay = argDisplay;
}

void colour_gradient::set_colours ( unsigned short argTopLeft,
                                    unsigned short argTopRight,
                                    unsigned short argBottomRight,
                                    unsigned short argBottomLeft )
{
   rgbTopLeft     = argTopLeft;     // 16bit, RGB565
   rgbTopRight    = argTopRight;    // 16bit, RGB565
   rgbBottomRight = argBottomRight; // optional, defaults to TopRight
   rgbBottomLeft  = argBottomLeft;  // optional, defaults to TopLeft
}

bool colour_gradient::draw_colour_gradient ( void )
{
   int width = 0;
   int height = 0;
   int x;
   int y;
   unsigned short *pPixels;
   unsigned short rgbLeft;
   unsigned short rgbRight;

   ErrNo = ERR_OK;

   if (pDisplay == NULL)
   {
      ErrNo = ERR_NO_DISPLAY;
      return false;
   }

   pDisplay->get_size(width, height);

   pPixels = new unsigned short[width];
   if (pPixels == NULL)
   {
      ErrNo = ERR_NO_MEMORY;
      return false;
   }

   for (y=0; y<height; y++)
   {
      // Sweep from rgbTopLeft to rgbBottomLeft;
      rgbLeft  = RGB565(point_in_gradient(y, height, RED565  (rgbTopLeft), RED565  (rgbBottomLeft)),
                        point_in_gradient(y, height, GREEN565(rgbTopLeft), GREEN565(rgbBottomLeft)),
                        point_in_gradient(y, height, BLUE565 (rgbTopLeft), BLUE565 (rgbBottomLeft)) );
      // Sweep from rgbTopRight to rgbBottomRight;
      rgbRight = RGB565(point_in_gradient(y, height, RED565  (rgbTopRight), RED565  (rgbBottomRight)),
                        point_in_gradient(y, height, GREEN565(rgbTopRight), GREEN565(rgbBottomRight)),
                        point_in_gradient(y, height, BLUE565 (rgbTopRight), BLUE565 (rgbBottomRight)) );

      for (x=0; x<width; x++)
      {
         // Sweep from rgbLeft to rgbRight;
         pPixels[x] = RGB565(point_in_gradient(x, width, RED565  (rgbLeft), RED565  (rgbRight)),
                             point_in_gradient(x, width, GREEN565(rgbLeft), GREEN565(rgbRight)),
                             point_in_gradient(x, width, BLUE565 (rgbLeft), BLUE565 (rgbRight)) );
      }
      pDisplay->draw_raster(0, y, pPixels, width);
   }
   delete pPixels;
   return true;
}

inline int round_to_int(double x)
{
   return int((x > 0.0) ? (x + 0.5) : (x - 0.5));
}

unsigned short colour_gradient::point_in_gradient(unsigned int current_point, unsigned int max_point, unsigned short first_value, unsigned short last_value)
{
   int            value_range;
   unsigned int   number_of_steps;
   float          delta_per_step;
   int            delta_from_origin;

   if (max_point <= 1)
   {
      return 0;
   }

   value_range       = last_value - first_value;
   number_of_steps   = max_point-1;
   delta_per_step    = (float)value_range/(float)number_of_steps;
   delta_from_origin = (int)round_to_int(current_point * delta_per_step);

#ifdef TESTING
   {
      int prev_delta = 0;

      if (current_point>0)
         prev_delta = (int)round_to_int((current_point-1) * delta_per_step);

      printf("x=%2d, average_delta=%f, thisdelta=%d, value=%4d\n",current_point,
                                                                  delta_per_step,
                                                                 delta_from_origin - prev_delta,
                                                                 (first_value + delta_from_origin));
   }
#endif

   return (unsigned short)(first_value + delta_from_origin);
}
