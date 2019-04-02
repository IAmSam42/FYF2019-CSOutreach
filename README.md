Forge You Future Cryptography project
Sam Holdcroft



Configuration


Setting a code:

Change the following line, and place the desired code in the brackets:

int code[NUM_PANELS] = {8, 7, 2, 6};


Setting the brightness:

There are two brightness', one is the base brightness for all the LEDs and the
other is how much the brightness is reduced on the 'worn out' LEDs. The two
can be set by changing the following lines:

#define BASE_BRIGHTNESS 64
#define REDUCE_BRIGHTNESS 230


Setting the pattern:

To customise the set of patterns on the individual panels, the loop() function
has to be edited. The first part of the loop function is a for loop that looks
like:

//Go through every LED on a panel:
for(int i=0; i<LEDS_PER_PANEL; i++)
{
  //Panel 1:
  setLED(i, animator.XX);

  //Panel 2:
  setLED(i + 64, animator);

  //Panel 3:
  setLED(i + 128, animator);

  //Panel 3:
  setLED(i + 192, animator);
}

XX can be swapped for any of the 6 animation functions, listed below. Be sure
to copy the (i) argument as well as the function name.

getPanelRainbow(i)    -- Sets the whole panel to be one cycling colour
getMovingRainbow(i);  -- Sets the panel to have a rainbow pattern move over it
getMovingStripes(i);  -- Displays a blue and white moving stripe over the panel
getMovingLines(i);    -- A random pattern of trailing green lines falls down
getRandomChar(i);     -- A random digit from 0-9 is displayed in a random colour
getGameOfLife(i);     -- Displays Conways Game of Life

IMPORTANT NOTE: using the same function on multiple panels will result in those
panels showing identical images, not unique versions of that pattern.
