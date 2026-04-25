#ifndef MOUSE_H
#define MOUSE_H

#include "common.h"
#include "types.h"


#include <dos.h>

#define MOUSE_INT           0x33
#define MOUSE_RESET         0x00
#define MOUSE_GETPRESS      0x05
#define MOUSE_GETRELEASE    0x06
#define MOUSE_GETMOTION     0x0B
#define LEFT_BUTTON         0x00
#define RIGHT_BUTTON        0x01
#define MIDDLE_BUTTON       0x02

typedef struct {
  byte on;
  byte button1;
  byte button2;
  byte button3;
  int num_buttons;
  sword x;
  sword y;
} MOUSE;

void get_mouse_motion(sword *dx, sword *dy);
sword init_mouse(MOUSE *mouse);
sword get_mouse_press(sword button);
sword get_mouse_release(sword button);

#endif