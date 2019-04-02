#include <FastLED.h>
#include <cstring>
#include <stdlib.h>

#include "char_arrays.h"
#include "Animator.h"

//Circuit configuration, doesn't need to be modified
#define LED_PIN     13
#define NUM_PANELS  4
#define LEDS_PER_PANEL 64
#define NUM_LEDS    NUM_PANELS*LEDS_PER_PANEL
#define COLOR_ORDER GRB

//Set the base brightness of the LEDs
#define BASE_BRIGHTNESS 64
//How much the 'used' LEDs are reduced in brightness. Measured as a fraction
//out of 255  - adjust to change "difficulty"
#define REDUCE_BRIGHTNESS 230


//Set the code to be displayed on the panels
int code[NUM_PANELS] = {8, 7, 2, 6};

//Variable to store colour of every individual LED
CRGB leds[NUM_LEDS];

//Variable to store the "mask" that encodes the desired patterns
int mask[NUM_LEDS] = {0};

//Make an animator for each panel:
Animator animator(BASE_BRIGHTNESS);


/**
 *  SETUP FUNCTIONS
 */

 /**
  * Create the mask to change the LED brightness based on the current code
  */
void createMask()
{
  //For each connected panel:
  for(int i=0; i<NUM_PANELS; i++)
  {
    //Switch on the code numeral:
    switch (code[i])
    {
      case 0 :  std::memcpy(&mask[i*LEDS_PER_PANEL], ar_0, LEDS_PER_PANEL*sizeof(int));
                break;
      case 1 :  std::memcpy(&mask[i*LEDS_PER_PANEL], ar_1, LEDS_PER_PANEL*sizeof(int));
                break;
      case 2 :  std::memcpy(&mask[i*LEDS_PER_PANEL], ar_2, LEDS_PER_PANEL*sizeof(int));
                break;
      case 3 :  std::memcpy(&mask[i*LEDS_PER_PANEL], ar_3, LEDS_PER_PANEL*sizeof(int));
                break;
      case 4 :  std::memcpy(&mask[i*LEDS_PER_PANEL], ar_4, LEDS_PER_PANEL*sizeof(int));
                break;
      case 5 :  std::memcpy(&mask[i*LEDS_PER_PANEL], ar_5, LEDS_PER_PANEL*sizeof(int));
                break;
      case 6 :  std::memcpy(&mask[i*LEDS_PER_PANEL], ar_6, LEDS_PER_PANEL*sizeof(int));
                break;
      case 7 :  std::memcpy(&mask[i*LEDS_PER_PANEL], ar_7, LEDS_PER_PANEL*sizeof(int));
                break;
      case 8 :  std::memcpy(&mask[i*LEDS_PER_PANEL], ar_8, LEDS_PER_PANEL*sizeof(int));
                break;
      case 9 :  std::memcpy(&mask[i*LEDS_PER_PANEL], ar_9, LEDS_PER_PANEL*sizeof(int));
      default:  break;
    }
  }
}

void setup()
{
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);

  //Set the base bightness
  FastLED.setBrightness(  BASE_BRIGHTNESS );

  //Create the mask to show the code
  createMask();

  Serial.begin(9600);
}


/**
 * LOOP FUNCTIONS
 */

/**
 * Set the LED at a given position
 */
void setLED(int pos, CRGB col)
{
  //Set the colour
  leds[pos] = col;
  //Set the brightness
  leds[pos].fadeLightBy(mask[pos] * REDUCE_BRIGHTNESS);
}

void loop()
{
  //Go through every LED on a panel:
  for(int i=0; i<LEDS_PER_PANEL; i++)
  {
    //Each panel has it's own animator method:

    //Panel 1:
    setLED(i, animator.getPanelRainbow(i));

    //Panel 2:
    setLED(i + 64, animator.getRandomChar(i));

    //Panel 3:
    setLED(i + 128, animator.getMovingLines(i));

    //Panel 3:
    setLED(i + 192, animator.getGameOfLife(i));
  }


  //Update the panel
  FastLED.show();

  //Update the animation:
  animator.update();

  createMask();
}
