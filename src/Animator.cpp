#include <FastLED.h>
#include <cstring>
#include <stdlib.h>

#include "Animator.h"

//Store the brightness of the LEDs
int brightness;

//Start index of the motion:
int startIndex = 0;

//Arrays to hold data on moving lines
int lineHeads[8][8] = { 0 };
CRGB lineColours[8][8];

//Array to hold game of life simulation:
int world[8][8] = { 0 };

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

CRGB Animator::getMovingLines(int pos)
{
  //Get the x and y coordinate of pos:
  int x = pos % 8;
  int y = pos / 8;

  return lineColours[x][y];
}

CRGB Animator::getGameOfLife(int pos)
{
  //Get the x and y coordinate of pos:
  int x = pos % 8;
  int y = pos / 8;

  //If the cell is alive:
  if(world[x][y]==1)
  {
    //Return a yellow colour:
    return CRGB::Yellow;
  }
  //Else the cell is dead:
  else
  {
    return CRGB::Black;
  }
}

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

void updateLife()
{
  Serial.println();

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

  //If there are no live cells:
  if(liveCells == 0)
  {
    //Repopulate:
    repopulate();

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

  memcpy(world, newWorld, sizeof(world));
}

void Animator::update()
{
  startIndex++;

  updateLines();
  updateLife();

  return;
}
