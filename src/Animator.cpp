#include <FastLED.h>
#include "Animator.h"

//Store the brightness of the LEDs
int brightness;

//Start index of the motion:
int startIndex = 0;

Animator::Animator(int i_brightness)
{
  brightness = i_brightness;
}

/**
 * The whole panel is a moving rainbow.
 */
CRGB Animator::getMovingRainbow(int pos)
{
  int index = startIndex + (pos*3);

  CRGB col = ColorFromPalette(RainbowColors_p, index, brightness, LINEARBLEND);

  return col;
}

/**
 * The whole panel is a single colour, but that colour slowly cycles through
 * a rainbow pattern.
 */
CRGB Animator::getPanelRainbow(int pos)
{
  int index = startIndex;

  CRGB col = ColorFromPalette(RainbowColors_p, index, brightness, LINEARBLEND);

  return col;
}

/**
 * The whole panel is a set of moving stripes
 */
CRGB Animator::getMovingStripes(int pos)
{
  int index = startIndex  + (pos*3);

  CRGB col = ColorFromPalette(CloudColors_p, index, brightness, LINEARBLEND);

  return col;
}

void Animator::update()
{
  startIndex++;

  return;
}
