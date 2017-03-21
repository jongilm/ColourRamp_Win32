/*****************************************************************************
 * FILE NAME     : ColourRamp.cpp
 * MODULE        : ColourRamp
 * AUTHOR        : Jonathan Gilmore
 * CREATION DATE : 13/11/2011
 *
 *****************************************************************************
 *
 * DESCRIPTION : Defines the entry point for the console application.
 *
 *
 *****************************************************************************
 */

#include "stdafx.h"
#include <iostream>
#include <string>
#include "conio.h"

#include "display.h"
#include "colour_gradient.h"

using namespace std;

#define UNUSED_PARAMETER(a) ((void)a)

int _tmain(int argc, _TCHAR* argv[])
{
   Display *pMyDisplay;
   colour_gradient *pGradient;
   string NameOfDisplayDevice;
   unsigned short TopLeft = 0;
   unsigned short TopRight = 0;
   unsigned short BottomRight = 0; // BottomRight is optional, defaults to TopRight
   unsigned short BottomLeft = 0;  // BottomLeft is optional, defaults to TopLeft

   // Parse the command line args

   // Check that we have at least the minimum number or parameters
   if (argc < 4)
   {
       // Inform the user of how to use the program
       // TODO: Give more details about acceptable device name and colour values.
       std::cout << "Parameters: <DisplayName> <TopLeft> <TopRight> [BottomRight] [BottomLeft]\n"
                    "            All colour values must be positive decimal numbers.";
       std::cin.get();
       exit(0);
   }

   // First 3 parameters are mandatory
   // TODO: One could validate these parameters further...
   //       e.g. Check that the device name meets certain requirements and
   //            user friendly range/sign checking of the colour values
   //            instead of simply casting to unsigned short.
   //       Also, one could permit entry of hex values.
   NameOfDisplayDevice = (char*)(argv[1]);
   TopLeft     = (unsigned short)atol((char*)argv[2]);
   TopRight    = (unsigned short)atol((char*)argv[3]);
   BottomRight = TopRight; // BottomRight is optional, defaults to TopRight
   BottomLeft  = TopLeft;  // BottomLeft is optional, defaults to TopLeft

   // Look for optional paramerters
   if (argc > 4)
   {
      BottomRight = (unsigned short)atol((char*)argv[4]);
      if (argc > 5)
      {
         BottomLeft = (unsigned short)atol((char*)argv[5]);
      }
   }

#ifdef TESTING
   // Inform the user of his choices...
   std::cout << "Gradient will be generated with the following parameters:" << endl;
   std::cout << "  DisplayDevice = " << NameOfDisplayDevice << endl;
   std::cout << "  TopLeft       = " << TopLeft     << endl;
   std::cout << "  TopRight      = " << TopRight    << endl;
   std::cout << "  BottomRight   = " << BottomRight << endl;
   std::cout << "  BottomLeft    = " << BottomLeft  << endl;
   std::cin.get();
#endif

   pMyDisplay = new Display();
   if (pMyDisplay == NULL)
   {
       std::cout << "ERROR: Insufficient memory for Display\n";
       std::cin.get();
       exit(0);
   }

   pMyDisplay->connect(NameOfDisplayDevice.c_str());

   pGradient = new colour_gradient(pMyDisplay);
   if (pGradient == NULL)
   {
       std::cout << "ERROR: Insufficient memory for colour_gradient\n";
       std::cin.get();
       exit(0);
   }

#ifdef TESTING
   pGradient->set_colours ( RGB565(0x00,0x00,0x00),   // 0x0000 = 0
                            RGB565(0x1F,0x00,0x00),   // 0xF800 = 63488
                            RGB565(0x1F,0x3F,0x1F),   // 0xFFFF = 65535
                            RGB565(0x00,0x00,0x1F) ); // 0x001F = 31
#else
   pGradient->set_colours ( TopLeft,
                            TopRight,
                            BottomRight,
                            BottomLeft );
#endif

   pGradient->draw_colour_gradient();

#ifdef DISPLAY_EXTENSION
   pMyDisplay->create_bmp("ColourRamp.bmp");
#endif

   delete pMyDisplay;
   //std::cin.get();

   return 0;
}
