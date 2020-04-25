#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/ioctl.h>

char *aal[4] = {
  "  __    ",
  "<(o )___",
  " ( ._> /",
  "  `---' "
};

char *aar[4] = {
  "     __  ",
  " ___( o)>",
  " \\ <_. ) ",
  "  `---'  "
};

int width, height;
int xlim, ylim;
char *frame = NULL;

// systems servers don't have ncurses so we have to use this trick
static void reset_term(void)
{
  printf("\e[1;1H\e[2J");
}

static void * next_frame(void)
{
  static int x=0, y=0; // top-left
  static int xd = 1, yd = 1; // move right
  // clear screen
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < (width-1); j++)
      frame[i*width+j] = ' ';
    frame[i*width+width-1] = '\n'; // not necessary
  }
  // show duck
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 9; j++)
      frame[(i+y)*width+x+j] = (xd == 1) ? aar[i][j] : aal[i][j];

  // move
  x += xd;
  y += yd;
  if (x == 0 || x == xlim)
    xd = -xd;
  if (y == 0 || y == ylim)
    yd = -yd;
}

// lock and cond-var
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
volatile bool consumed = false;

static void * fake_decoder(void * ptr)
{
  do {
    // lock & wait until the last frame has been consumed
    // Your code here:


    next_frame();

    // mark frame as newly rendered
    // Your code here:


  } while(1);
}

static void * renderer(void * ptr)
{
  int nbytes = width * height;
  while (1) {
    usleep(500000); // update every 0.5s

    // just lock
    // Your code here:


    // print on screen don't change these
    reset_term();
    for (int i = 0; i < nbytes; i++)
      putchar(frame[i]);
    fflush(stdout);

    // mark frame as consumed and wake up the waiting thread
    // Your code here:


  }
}

int main()
{
  struct winsize xy;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &xy);
  width = xy.ws_col+1; // +1 for \n
  height = xy.ws_row-1;
  xlim = width - 9;
  ylim = height - 4;
  if (height < 8 || width < 20) {
    printf("please use larger terminal\n");
    return 0;
  }

  frame = malloc(height * width);
  memset(frame, ' ', height * width);
  pthread_t pt1, pt2;
  pthread_create(&pt1, NULL, fake_decoder, NULL);
  pthread_create(&pt2, NULL, renderer, NULL);
  pthread_join(pt1, NULL);
  pthread_join(pt2, NULL);
  return 0;
}
