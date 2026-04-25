#ifndef CANVAS_H
#define CANVAS_H

#include "types.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

typedef struct {
	int w,h,f,t;
	byte *p;
} Canvas;

Canvas *Canvas_New(char *fn);
void Canvas_Free(Canvas *c);
void Canvas_DrawChar(Canvas *c,int x,int y,int s,int f,byte fg,byte bg);

#endif
