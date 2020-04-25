#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/ioctl.h>

char value = 'A';
int width = 0;
int height = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
volatile bool consumed = false;

char *frame = NULL;

static void reset_term(void)
{
  printf("\e[1;1H\e[2J");
}

static void * fake_decoder(void * ptr)
{
  do {
    if ((++value) > 'Z')
      value = 'A';

    // lock & wait until the last frame has been consumed
    // Your code here:

    // don't change the for-loop
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < (width-1); j++) {
        frame[(i*width)+j] = value;
      }
      frame[i*width+width-1] = '\n'; // not necessary
    }

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

    // don't change these
    reset_term();
    for (int i = 0; i < nbytes; i++) {
      putchar(frame[i]);
    }
    fflush(stdout);

    // mark frame as consumed and wake up the waiting thread
    // Your code here:

  }
}

int main()
{
  struct winsize xy;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &xy);
  width = xy.ws_col;
  height = xy.ws_row-2;
  frame = malloc(height * width);
  memset(frame, value, height * width);
  pthread_t pt1, pt2;
  pthread_create(&pt1, NULL, fake_decoder, NULL);
  pthread_create(&pt2, NULL, renderer, NULL);
  pthread_join(pt1, NULL);
  pthread_join(pt2, NULL);
  return 0;
}
