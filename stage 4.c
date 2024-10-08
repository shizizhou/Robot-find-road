#include "stdio.h"
#include "graphics.h"
#include "math.h"
#include "stdlib.h"
#include "stdbool.h"
const int width = 800;
const int height = 800;
const int waitTime = 50;
int homex = 100;
int homey = 100;
int temp = 0;
int counter = 0;
int markersize = 2;
int blocksize = 3;
int steps = 0;
// record the movement of robot
int movex[100];
int movey[100];
int movedir[100];

// define three main structs for the game
struct robot
{
  int x;
  int y;
  int dir;
};

struct marker
{
  int x;
  int y;
};

struct block
{
  int x;
  int y;
};

// initialize background
void drawBackground()
{
  background();
  setColour(black);
  for (int a = 100; a < 700; a += 50)
  {
    for (int b = 100; b < 700; b += 50)
    {
      drawRect(a, b, 50, 50);
    }
  }
}

// update the position each frame
void update(int x, int y, int dir)
{
  int a[3];
  int b[3];
  clear();
  switch (dir)
  {
  case 0:
    a[0] = x;
    a[1] = x + 25;
    a[2] = x + 50;
    b[0] = y;
    b[1] = y + 50;
    b[2] = y;
    break;
  case 1:
    a[0] = x;
    a[1] = x + 50;
    a[2] = x;
    b[0] = y;
    b[1] = y + 25;
    b[2] = y + 50;
    break;
  case 2:
    a[0] = x;
    a[1] = x + 25;
    a[2] = x + 50;
    b[0] = y + 50;
    b[1] = y;
    b[2] = y + 50;
    break;
  case 3:
    a[0] = x + 50;
    a[1] = x;
    a[2] = x + 50;
    b[0] = y;
    b[1] = y + 25;
    b[2] = y + 50;
    break;
  }
  setColour(black);
  fillPolygon(3, a, b);
}

void updateRec(struct marker mark[], struct block blo[])
{
  setColour(black);
  // int size1=sizeof(blo)/sizeof(blo[0]);
  for (int i = 0; i < blocksize; i++)
  {
    fillRect(blo[i].x, blo[i].y, 50, 50);
  }
  setColour(gray);
  // int size2=sizeof(mark)/sizeof(mark[0]);
  for (int i = 0; i < markersize; i++)
  {
    fillRect(mark[i].x, mark[i].y, 50, 50);
  }
  setColour(blue);
  fillRect(homex, homey, 50, 50);
}

// core movement function
void forward(struct robot *rob)
{
  foreground();
  movex[steps] = rob->x;
  movey[steps] = rob->y;
  movedir[steps] = rob->dir;
  steps++;
  switch (rob->dir)
  {
  case 0:
    rob->y += 50;
    break;
  case 1:
    rob->x += 50;
    break;
  case 2:
    rob->y -= 50;
    break;
  case 3:
    rob->x -= 50;
    break;
  }
  update(rob->x, rob->y, rob->dir);
}

// functions about changing direction
void check_dir(struct robot *rob)
{
  if (rob->dir < 0)
  {
    rob->dir = 3;
  }
  if (rob->dir > 3)
  {
    rob->dir = 0;
  }
}

void left(struct robot *rob)
{
  rob->dir--;
  check_dir(rob);
  update(rob->x, rob->y, rob->dir);
}

void right(struct robot *rob)
{
  rob->dir++;
  check_dir(rob);
  update(rob->x, rob->y, rob->dir);
}

bool canMoveForward(struct robot *rob, struct block blo[])
{
  int x = rob->x;
  int y = rob->y;
  switch (rob->dir)
  {
  case 0:
    y += 50;
    break;
  case 1:
    x += 50;
    break;
  case 2:
    y -= 50;
    break;
  case 3:
    x -= 50;
    break;
  }
  // int size=sizeof(blo)/sizeof(blo[0]);
  for (int i = 0; i < steps; i++)
  {
    if (x == movex[i] && y == movey[i])
    {
      return false;
    }
  }
  if (50 < x && x < 700 && 50 < y && y < 700)
  {
    for (int i = 0; i < blocksize; i++)
    {
      if (blo[i].x == x && blo[i].y == y)
      {
        return false;
      }
    }
    return true;
  }
  return false;
}

void changeDir(struct robot *rob, struct block blo[])
{
  if (!canMoveForward(rob, blo))
  {
    right(rob);
  }
}

// functions about marker
bool atMarker(struct robot rob, struct marker mark[])
{
  // int size=sizeof(mark)/sizeof(mark[0]);
  for (int i = 0; i < markersize; i++)
  {
    if (mark[i].x == rob.x && mark[i].y == rob.y)
    {
      return true;
    }
  }
  return false;
}

void pickUpMarker(struct marker mark[], struct robot *rob)
{
  mark[temp].x = rob->x;
  mark[temp].y = rob->y;
}

void dropMarker(struct marker mark[], struct marker *mark1[])
{
  // int size=sizeof(mark)/sizeof(mark[0]);
  for (int i = 0; i < markersize; i++)
  {
    if (mark[i].x == homex && mark[i].y == homey)
    {
      mark[i].x = -1000;
      mark[i].y = -1000;
      counter++;
    }
  }
}

bool isCarryingMarker(struct marker mark[], struct robot rob)
{
  // int size=sizeof(mark)/sizeof(mark[0]);
  for (int i = 0; i < markersize; i++)
  {
    if (mark[i].x == rob.x && mark[i].y == rob.y)
    {
      temp = i;
      return true;
    }
  }
  return false;
}

// functions about home
bool atHome(struct robot rob, int x, int y)
{
  if (rob.x == x && rob.y == y)
  {
    return true;
  }
  return false;
}

void returnHome(struct robot *rob, struct marker *mark[], struct marker mark1[], struct block blo[])
{
  for (int i = steps - 1; i > -1; i--)
  {
    rob->x = movex[i];
    rob->y = movey[i];
    rob->dir = movedir[i];
    update(rob->x, rob->y, rob->dir);
    pickUpMarker(mark1, rob);
    updateRec(mark1, blo);
    dropMarker(mark1, mark);
    sleep(waitTime);
  }
  for (int i = 0; i < 100; i++)
  {
    movex[i] = 0;
    movey[i] = 0;
    movedir[i] = 0;
  }
  steps = 0;
}

struct robot move(struct robot rob, struct marker mark[], struct block blo[])
{
  while(steps<100)
  {
    forward(&rob);
    if (isCarryingMarker(mark, rob))
    {
      returnHome(&rob, &mark, mark, blo);
    }
    changeDir(&rob, blo);
    updateRec(mark, blo);
    printf("%d %d %d\n", rob.x, rob.y, rob.dir);
    sleep(waitTime);
    if (counter == markersize)
    {
      return rob;
    }
  }
  setColour(blue);
  forward(&rob);
  return rob;
}

int main(int argc, char **argv)
{
  // initialize each variable
  struct marker mark[markersize];
  struct block blo[blocksize];
  struct robot rob;
  blo[0].x = 600;
  blo[0].y = 400;
  blo[1].x = 300;
  blo[1].y = 400;
  blo[2].x = 500;
  blo[2].y = 500;
  mark[0].x = 500;
  mark[0].y = 100;
  mark[1].x = 100;
  mark[1].y = 200;
  rob.dir = 0;
  // read home position from terminal
  int direction = 0;
  if (argc == 4)
  {
    homex = atoi(argv[1]);
    homey = atoi(argv[2]);
    direction = atoi(argv[3]);
  }
  rob.x = homex;
  rob.y = homey;
  rob.dir = direction;
  // run the program
  setWindowSize(width, height);
  drawBackground();
  move(rob, mark, blo);
}