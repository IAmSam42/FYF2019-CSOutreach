/*
  Animator.h - Library for animating LEDS
*/
#ifndef Animator_h
#define Animator_h

#include <FastLED.h>
#include "char_arrays.h"

class Animator
{
  public:
    Animator(int brightness);
    CRGB getPanelRainbow(int pos);
    CRGB getMovingRainbow(int pos);
    CRGB getMovingStripes(int pos);
    CRGB getMovingLines(int pos);
    CRGB getRandomChar(int pos);
    CRGB getGameOfLife(int pos);
    void update();
};

#endif
