/* csitest.c - CHIP program . */
/*
 * This code and its documentation is Copyright 2016 Steven Ford, http://geeky-boy.com
 * and licensed "public domain" style under Creative Commons "CC0": http://creativecommons.org/publicdomain/zero/1.0/
 * To the extent possible under law, the contributors to this project have
 * waived all copyright and related or neighboring rights to this work.
 * In other words, you can use this code for any purpose without any
 * restrictions.  This work is published from: United States. The project home
 * is https://github.com/fordsfords/gpiotest/tree/gh-pages
 */

/* Allow compilation under c99 and POSIX 1995 / 2004. */
#if __STDC_VERSION__ >= 199901L
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif /* __STDC_VERSION__ */

#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

/* See http://wiki.geeky-boy.com/w/index.php?title=Internal_error_handling */
#define WCHK(cond_expr) do { \
  if (!(cond_expr)) { \
    fprintf(stderr, "%s:%d, Warning, expected '%s' to be true (%s)\n", \
      __FILE__, __LINE__, #cond_expr, strerror(errno)); \
  }  \
} while (0)

#define ECHK(cond_expr) do { \
  if (!(cond_expr)) { \
    fprintf(stderr, "%s:%d, Error, expected '%s' to be true (%s)\n", \
      __FILE__, __LINE__, #cond_expr, strerror(errno)); \
    abort(); \
  }  \
} while (0)

 
double tv_diff(struct timespec *first, struct timespec *second)
{
  double rtn = (second->tv_sec - first->tv_sec) + (second->tv_nsec - first->tv_nsec) * 1e-9;
  return rtn;
}  /* tv_diff */

volatile int vol_int;

int main()
{
  struct timespec first, second, third, fourth;

  for (int i = 0; i < 5; ++i) {
    /* Enable and configure XIO-P0 as input. */
    int export_fd = open("/sys/class/gpio/export", O_WRONLY);  ECHK(export_fd != -1);
    /* If port 6 is already exported, the following will fail. That's OK, but warn. */
    int wlen = write(export_fd, "132", 4);  WCHK(wlen == 4);
    close(export_fd);
   
    int dir_fd = open("/sys/class/gpio/gpio132/direction", O_RDWR);  ECHK(dir_fd != -1);
    wlen = write(dir_fd, "in", 3);  ECHK(wlen == 3);
    close(dir_fd);

    int in_val_fd = open("/sys/class/gpio/gpio132/value", O_RDWR);  ECHK(in_val_fd != -1);

    /* Enable and configure XIO-P1 as output. */
    export_fd = open("/sys/class/gpio/export", O_WRONLY);  ECHK(export_fd != -1);
    /* If port 5 is already exported, the following will fail. That's OK, but warn. */
    wlen = write(export_fd, "133", 4);  WCHK(wlen == 4);
    close(export_fd);
   
    dir_fd = open("/sys/class/gpio/gpio133/direction", O_RDWR);  ECHK(dir_fd != -1);
    wlen = write(dir_fd, "out", 4);  ECHK(wlen == 4);
    close(dir_fd);

    int out_val_fd = open("/sys/class/gpio/gpio133/value", O_RDWR);  ECHK(out_val_fd != -1);

    sleep(1);

    /* Get chip out of low-power mode */
    for (int j = 0; j < 100000000; j++)
      ++vol_int;

    char readbuf[99] = "";
    int st;

    st = clock_gettime(CLOCK_MONOTONIC, &first);  ECHK(st == 0);

    for (int j = 0; j < 10000; ++j) {
      st = lseek(in_val_fd, 0, SEEK_SET);  ECHK(st == 0);
    }

    st = clock_gettime(CLOCK_MONOTONIC, &second);  ECHK(st == 0);

    for (int j = 0; j < 10000; ++j) {
      st = lseek(in_val_fd, 0, SEEK_SET);  ECHK(st == 0);
      int rlen = read(in_val_fd, readbuf, sizeof(readbuf));  ECHK(rlen > 0);
    }

    st = clock_gettime(CLOCK_MONOTONIC, &third);  ECHK(st == 0);

    for (int j = 0; j < 5000; ++j) {
      wlen = write(out_val_fd, "1", 2);  ECHK(wlen == 2);
      wlen = write(out_val_fd, "0", 2);  ECHK(wlen == 2);
    }
    st = clock_gettime(CLOCK_MONOTONIC, &fourth);  ECHK(st == 0);

    printf("lseek out: %lf us\n", 1000000/10000 * tv_diff(&first, &second));
    printf("lseek/read in: %lf us\n", 1000000/10000 * tv_diff(&second, &third));
    printf("write out: %lf us\n", 1000000/10000 * tv_diff(&third, &fourth));

    close(out_val_fd);
    close(in_val_fd);

    /* Release XIO-P0 */
    export_fd = open("/sys/class/gpio/unexport", O_WRONLY);  ECHK(export_fd != -1);
    wlen = write(export_fd, "132", 4);  ECHK(wlen == 4);
    close(export_fd);
   
    dir_fd = open("/sys/class/gpio/gpio133/direction", O_RDWR);  ECHK(dir_fd != -1);
    wlen = write(dir_fd, "in", 3);  ECHK(wlen == 3);
    close(dir_fd);

    /* Release XIO-P1 */
    export_fd = open("/sys/class/gpio/unexport", O_WRONLY);  ECHK(export_fd != -1);
    wlen = write(export_fd, "133", 4);  ECHK(wlen == 4);
    close(export_fd);
  }  /* for i */

  printf("Now exiting\n");

  return 0;
}  /* main */
