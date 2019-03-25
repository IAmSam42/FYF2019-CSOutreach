#include <FastLED.h>
#include "Animator.h"

//Store the cyrrent palette and blending:
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

//Store the brightness of the LEDs
int brightness;

//Start index of the motion:
int startIndex = 0;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

Animator::Animator(int i_brightness)
{
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;

  brightness = i_brightness;
}

CRGB Animator::getColour(int pos)
{
  int index = startIndex + (pos*3);

  CRGB col = ColorFromPalette(currentPalette, index, brightness, currentBlending);

  return col;
}


// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
     for( int i = 0; i < 16; i++) {
         currentPalette[i] = CHSV( random8(), 255, random8());
     }
}

void SetupBlackAndWhiteStripedPalette()
{
     // 'black out' all 16 palette entries...
     fill_solid( currentPalette, 16, CRGB::Black);
     // and set every fourth one to white.
     currentPalette[0] = CRGB::White;
     currentPalette[4] = CRGB::White;
     currentPalette[8] = CRGB::White;
     currentPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
     CRGB purple = CHSV( HUE_PURPLE, 255, 255);
     CRGB green  = CHSV( HUE_GREEN, 255, 255);
     CRGB black  = CRGB::Black;

     currentPalette = CRGBPalette16(
                                    green,  green,  black,  black,
                                    purple, purple, black,  black,
                                    green,  green,  black,  black,
                                    purple, purple, black,  black );
}

const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
     CRGB::Red,
     CRGB::Gray, // 'white' is too bright compared to red and blue
     CRGB::Blue,
     CRGB::Black,

     CRGB::Red,
     CRGB::Gray,
     CRGB::Blue,
     CRGB::Black,

     CRGB::Red,
     CRGB::Red,
     CRGB::Gray,
     CRGB::Gray,
     CRGB::Blue,
     CRGB::Blue,
     CRGB::Black,
     CRGB::Black
};

void ChangePalettePeriodically()
{
     uint8_t secondHand = (millis() / 1000) % 60;
     static uint8_t lastSecond = 99;

     if( lastSecond != secondHand)
     {
       lastSecond = secondHand;
       if( secondHand ==  0)
       {
         currentPalette = RainbowColors_p;
         currentBlending = LINEARBLEND;
       }
       if( secondHand == 10)
       {
         currentPalette = RainbowStripeColors_p;
         currentBlending = NOBLEND;
       }
       if( secondHand == 15)
       {
         currentPalette = RainbowStripeColors_p;
         currentBlending = LINEARBLEND;
       }
       if( secondHand == 20)
       {
         SetupPurpleAndGreenPalette();
         currentBlending = LINEARBLEND;
       }
       if( secondHand == 25)
       {
         SetupTotallyRandomPalette();
         currentBlending = LINEARBLEND;
       }
       if( secondHand == 30)
       {
         SetupBlackAndWhiteStripedPalette();
         currentBlending = NOBLEND;
       }
       if( secondHand == 35)
       {
         SetupBlackAndWhiteStripedPalette();
         currentBlending = LINEARBLEND;
       }
       if( secondHand == 40)
       {
         currentPalette = CloudColors_p;
         currentBlending = LINEARBLEND;
       }
       if( secondHand == 45)
       {
         currentPalette = PartyColors_p;
         currentBlending = LINEARBLEND;
       }
       if( secondHand == 50)
       {
         currentPalette = myRedWhiteBluePalette_p;
         currentBlending = NOBLEND;
       }
       if( secondHand == 55)
       {
         currentPalette = myRedWhiteBluePalette_p;
         currentBlending = LINEARBLEND;
       }
     }
}

void Animator::update()
{
  ChangePalettePeriodically();
  startIndex++;

  return;
}
