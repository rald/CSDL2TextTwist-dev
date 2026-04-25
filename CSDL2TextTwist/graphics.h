#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "types.h"
#include "palette.h"

#define INPUT_STATUS 0x03DA
#define VRETRACE     0x08

extern byte *VGA;
extern word *my_clock;

void SetMode(byte mode);
void SetPalette();
void vsync();
void DrawPoint(byte *srf,int x,int y,byte c);
int ReadPoint(byte *srf,int x,int y);
void DrawLine(byte *srf,int x0,int y0,int x1,int y1,byte c);

#endif
