#include <FastLED.h>
#include <cstring>
#include <stdlib.h>

#include "Animator.h"

//The maximum number of generations before the Game of Life world resets
#define MAX_GENS 50

//Store the brightness of the LEDs
int brightness;

//Start index of the motion:
int startIndex = 0;

//Arrays to hold data on moving lines
int lineHeads[8][8] = { 0 };
CRGB lineColours[8][8];

//Array to hold current char mask:
int charMask[64] = { 0 };
CRGB charColour = CRGB::Yellow; //Current colour of the character

//Array to hold game of life simulation:
int world[8][8] = { 0 };
int genNumber = 0; //Number of generations of the current world.

Animator::Animator(int i_brightness)
{
  //Save the brightness:
  brightness = i_brightness;

  //Fill the lineColours array with a colour:
  std::fill_n(*lineColours, 64, CRGB::Black);

  //Seed a random number generator:
  randomSeed(analogRead(0));
}

/**
 * The whole panel is a moving rainbow.
 */
CRGB Animator::getMovingRainbow(int pos)
{
  //Get the index of the pixel. Increasting the pos mutiplier shows more of the
  //spectrum
  int index = startIndex + (pos*3);


  //Return the colour from the rainbox palette
  return ColorFromPalette(RainbowColors_p, index, brightness, LINEARBLEND);
}

/**
 * The whole panel is a single colour, but that colour slowly cycles through
 * a rainbow pattern.
 */
CRGB Animator::getPanelRainbow(int pos)
{
  //Get the index, by not using the LED position we get a constant colours
  //accross the panel
  int index = startIndex;

  //Get the colour from the rainbox palette
  return ColorFromPalette(RainbowColors_p, index, brightness, LINEARBLEND);
}

/**
 * The whole panel is a set of moving stripes
 */
CRGB Animator::getMovingStripes(int pos)
{
  //Get the index of the pixel. Increasting the pos mutiplier shows more stripes
  int index = startIndex  + (pos*3);

  //Get the colour from the clour pallette
  return ColorFromPalette(CloudColors_p, index, brightness, LINEARBLEND);
}

/**
 * Display random lines dropping down over the screen. The lines are green and
 * have a tail that slowly fades out
 */
CRGB Animator::getMovingLines(int pos)
{
  //Get the x and y coordinate of pos:
  int x = pos % 8;
  int y = pos / 8;

  //return the colour from the line colour array
  return lineColours[x][y];
}

/**
 * Display a random character from 0-9 on the panel.
 * The character has a random colour.
 */
CRGB Animator::getRandomChar(int pos)
{
  //Check if this pixel should be active
  if(charMask[pos] == 1)
  {
    //Return the current char colour:
    return charColour;
  }
  else //Else, return black
  {
    return CRGB::Black;
  }
}

/**
 * Display Conways Game of Life. When there are no live cells left the grid will
 * repolulate, but a restart will be needed if the grid enters a stable state
 */
CRGB Animator::getGameOfLife(int pos)
{
  //Get the x and y coordinate of pos:
  int x = pos % 8;
  int y = pos / 8;

  //If the cell is alive:
  if(world[x][y]==1)
  {
    CRGB col = CRGB::Yellow;

    return col%=brightness;
  }
  //Else the cell is dead:
  else
  {
    return CRGB::Black;
  }
}


/**
 *  Update the random line to create trailing patterns down the LED screen.
 */
void updateLines()
{
  //Don't update on every cycle:
  if((startIndex%50)!=0)
  {
    return;
  }


  // **Move old line heads**

  //Create a new lineHead array:
  int newHeads[8][8] = { 0 };

  //Go through all the elements in lineHead:
  for(int i=0; i<8; i++)
  {
    for(int j=0; j<8; j++)
    {
      //If a 1 is found, and it is not on the bottom row:
      if((lineHeads[i][j]==1) && (i!=7))
      {
        //Move it down one in the new array:
        newHeads[i+1][j] = 1;
      }
    }
  }

  //Replace the old line heads with the new ones:
  memcpy(lineHeads, newHeads, sizeof(lineHeads));


  // **Spawn new line heads**

  //Go through each column:
  for(int j=0; j<8; j++)
  {
    //Pick a random number, if it's 42 then:
    if(random(0,100) == 42)
    {
      //Spawn a new line head in this column
      lineHeads[0][j] = 1;
    }
  }


  // **Update the colours**

  //Go through all the elements in the updated lineHead:
  for(int i=0; i<8; i++)
  {
    for(int j=0; j<8; j++)
    {
      //First reduce the brightness of this LED:
      lineColours[i][j].subtractFromRGB(40);

      //If anything but 0 is found:
      if(lineHeads[i][j]!=0)
      {
        //Set this LED to be green
        lineColours[i][j] = CRGB::Green;
      }
    }
  }
}

/**
 * Generate a new character and set the colour.
 */
void updateChar()
{
  //Don't update on every cycle:
  if((startIndex%50)!=0)
  {
    return;
  }

  //Pick a random number from 0 - 9, then set that as the current mask:
  switch (random(0, 9))
  {
    case 0 :  std::memcpy(charMask, ar_0, 64*sizeof(int));
              break;
    case 1 :  std::memcpy(charMask, ar_1, 64*sizeof(int));
              break;
    case 2 :  std::memcpy(charMask, ar_2, 64*sizeof(int));
              break;
    case 3 :  std::memcpy(charMask, ar_3, 64*sizeof(int));
              break;
    case 4 :  std::memcpy(charMask, ar_4, 64*sizeof(int));
              break;
    case 5 :  std::memcpy(charMask, ar_5, 64*sizeof(int));
              break;
    case 6 :  std::memcpy(charMask, ar_6, 64*sizeof(int));
              break;
    case 7 :  std::memcpy(charMask, ar_7, 64*sizeof(int));
              break;
    case 8 :  std::memcpy(charMask, ar_8, 64*sizeof(int));
              break;
    case 9 :  std::memcpy(charMask, ar_9, 64*sizeof(int));
    default:  break;
  }

  //Pick a random colour for the character:
  charColour = CHSV( random8(), 255, brightness);
}


/**
 *  Randomly repopulate the Game of Life array with living cells
 */
void repopulate()
{
  //Go through the whole world:
  for(int i=0; i<8; i++)
  {
    for(int j=0; j<8; j++)
    {
      //Pick a random number, if it's 0 then:
      if(random(0,5) == 0)
      {
        //Bring this cell to life!
        world[i][j] = 1;
      }
    }
  }
}

/**
 * For a given x,y position, calculate the number of neighbours of a cell in
 * the Game of Life world array.
 */
int lifeNeighbours(int x, int y)
{
  int neighbours = 0;

  //Go through all 9 adjacent cells:
  for(int i=(x-1); i<=(x+1); i++)
  {
    for(int j=(y-1); j<=(y+1); j++)
    {
      //If we are looking at cell (x,y) then ignore it:
      if((i==x) && (j==y))
      {
        continue;
      }
      //If i and j are in bounds:
      else if(i>=0 && i<8 && j>=0 && j<8)
      {
        neighbours += world[i][j];
      }
    }
  }

  //Return the number of neighbours
  return neighbours;
}


/**
 *  Update the game of life world, according to the rules of the game.
 */
void updateLife()
{
  //Don't update on every cycle:
  if((startIndex%50)!=0)
  {
    return;
  }

  //Count the number of live cells:
  int liveCells = 0;
  for(int i=0; i<8; i++)
  {
    for(int j=0; j<8; j++)
    {
      liveCells += world[i][j];
    }
  }

  //If there are no live cells, or the max number of generations have been
  //reached
  if((liveCells == 0) || (genNumber == MAX_GENS))
  {
    //Repopulate:
    repopulate();


    //Reset the number of generations
    genNumber = 0;

    return;
  }


  //Make a new world:
  int newWorld[8][8] = { 0 };

  //Go through every cell:
  for(int i=0; i<8; i++)
  {
    for(int j=0; j<8; j++)
    {
      //Get the number of neighbours
      int n = lifeNeighbours(i, j);

      //Live cell with less than 2: cell dies
      if(world[i][j] == 1 && n<2)
      {
        newWorld[i][j] = 0;
      }
      //Live cell with 2 or 3 neighbours: cell live
      else if(world[i][j] == 1 && (n==2 || n==3))
      {
        newWorld[i][j] = 1;
      }
      //Live cell with more than 4: cell updateLines
      else if(world[i][j] == 1 && n>3)
      {
        newWorld[i][j] = 0;
      }
      //Dead cell with 3 neighbours: cell liveCells//Live cell with more than 4: cell updateLines
      else if(world[i][j] == 0 && n==3)
      {
        newWorld[i][j] = 1;
      }
    }
  }

  //Replace the old world with the new one.
  memcpy(world, newWorld, sizeof(world));

  //Increment the generation count:
  genNumber++;
}

void Animator::update()
{
  //Increment the start index, used for animation
  startIndex++;

  //Update the the random lines, the random character and the game of life grid
  updateLines();
  updateChar();
  updateLife();
}
