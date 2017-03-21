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

// BMP565 colour masks
// Pixels are specified as 16-bit numbers in RGB565 format.
// The bits for each colour are assigned as follows:
// Bit      15 14 13 12 11 10 9  8  7  6  5  4  3  2  1  0
// Colour   R4 R3 R2 R1 R0 G5 G4 G3 G2 G1 G0 B4 B3 B2 B1 B0
#define RED_MASK    0xF800 // 1111 1000 0000 0000
#define GREEN_MASK  0x07E0 // 0000 0111 1110 0000
#define BLUE_MASK   0x001F // 0000 0000 0001 1111

/* Number of bits that each colour is shifted to the left */
#define RED_SHIFT   11
#define GREEN_SHIFT 5
#define BLUE_SHIFT  0

/* Some useful macros to construct/disect colour value */
#define RGB565(r,g,b) (((r<<RED_SHIFT  )&RED_MASK  )| \
                       ((g<<GREEN_SHIFT)&GREEN_MASK)| \
                       ((b<<BLUE_SHIFT )&BLUE_MASK ))
#define RED565(colour)   ((colour & RED_MASK  )>>RED_SHIFT  )
#define GREEN565(colour) ((colour & GREEN_MASK)>>GREEN_SHIFT)
#define BLUE565(colour)  ((colour & BLUE_MASK )>>BLUE_SHIFT )

typedef enum tagERRNO
{
  ERR_OK = 0,
  ERR_NO_MEMORY,
  ERR_NO_DISPLAY,
  ERR_UNKNOWN
} tERRNO;

class colour_gradient
{
   public:
      colour_gradient();
      colour_gradient( Display *argDisplay );
      colour_gradient ( Display *argDisplay,
                        unsigned short rgbTopLeft,
                        unsigned short rgbTopRight,
                        unsigned short rgbBottomRight,
                        unsigned short rgbBottomLeft );

      void colour_gradient::initialise( void );
      void set_display ( Display *argDisplay );
      void set_colours ( unsigned short rgbTopLeft,
                         unsigned short rgbTopRight,
                         unsigned short rgbBottomRight,
                         unsigned short rgbBottomLeft );
      bool draw_colour_gradient( void );

   private:
      tERRNO   ErrNo;
      Display *pDisplay;
      unsigned short rgbTopLeft;       // 16bit, RGB565
      unsigned short rgbTopRight;      // 16bit, RGB565
      unsigned short rgbBottomRight;   // optional, defaults to TopRight
      unsigned short rgbBottomLeft;    // optional, defaults to TopLeft

      unsigned short point_in_gradient ( unsigned int current_point,
                                         unsigned int max_point,
                                         unsigned short first_value,
                                         unsigned short last_value );
};
