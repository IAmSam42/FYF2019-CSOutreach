#include <FastLED.h>

#define LED_PIN     13
#define NUM_LEDS    64
#define COLOR_ORDER GRB

#define BASE_BRIGHTNESS 64
#define RDCT_BRIGHTNESS 230

CRGB leds[NUM_LEDS];

int ar_8[64] = {0, 0, 0, 1, 1, 0, 0, 0,
                0, 0, 1, 0, 0, 1, 0, 0,
                0, 0, 1, 0, 0, 1, 0, 0,
                0, 0, 0, 1, 1, 0, 0, 0,
                0, 0, 1, 0, 0, 1, 0, 0,
                0, 0, 1, 0, 0, 1, 0, 0,
                0, 0, 0, 1, 1, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,};

/**
 * Set the LED at a given position
 */
void setLED(int pos, CRGB col)
{
  leds[pos] = col;
  leds[pos].fadeLightBy(ar_8[pos] * RDCT_BRIGHTNESS);
}

void setup()
{
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );

  //Set the base bightness
  FastLED.setBrightness(  BASE_BRIGHTNESS );

  Serial.begin(9600);
}

void loop()
{
  for(int i=0; i<NUM_LEDS; i++)
  {
    setLED(i, CRGB::Blue);

    FastLED.show();
  }
}
