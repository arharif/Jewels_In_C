#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Shortcuts for the unsigned types
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define SWAP(a, b)                                                             \
    do {                                                                       \
        typeof(a) temp = a;                                                    \
        a              = b;                                                    \
        b              = temp;                                                 \
    } while (0)

#endif
