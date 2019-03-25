/*
  Animator.h - Library for animating LEDS
*/
#ifndef Animator_h
#define Animator_h

#include <FastLED.h>

class Animator
{
  public:
    Animator(int brightness);
    CRGB getColour(int pos);
    void update();

};

#endif
