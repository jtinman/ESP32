#include <FastLED.h>
#include "B_Ledgfx.h"
#include "A_Globals.h"
void DrawMarquee()
{
    static float scroll = 0.0f;     //This is the scrolling effect of the marquee
    scroll += 0.1f;
    if (scroll > 5.0)
      scroll -= 5.0;

    for (float i=scroll; i<NUM_LEDS; i+=5) //Repeat every 5.0 pixels up to the middle
    {
        DrawPixels(i, 3, CRGB(r, g, b));          //i is starting point, length is 3
    }
}

void DrawComet()
{
  const byte fadeAmt = 64;          //Fade on each pass
  const int cometSize = 3;          //Pixel width of comet
  const int deltaHue = 4;           //How far to step the cycling hue each frame
  const double cometSpeed = 0.5;    //How far to advance the comet each frame

  static byte hue = HUE_RED;        //Current (& starting) colour
  static int iDirection = 1;        //Direction modifier
  static double iPos = 0.0;         //Current position on strip

  hue += deltaHue;                  //Update current colour
  iPos += iDirection * cometSpeed;  //Update Comet position

//Switch direction at the end fo the strip
  if (iPos ==(NUM_LEDS - cometSize) || iPos ==0)
    iDirection *= -1;

//Fill the comet pixels
  for (int i=0; i<cometSize; i++)
    Leds[(int)iPos + i].setHue(hue);

//Fade out the tail in a random twinkle
  for (int j=0; j<NUM_LEDS; j++)
    if (random(2) == 1)
      Leds[j] = Leds[j].fadeToBlackBy(fadeAmt);
}
