#include "mouse.h"

void get_mouse_motion(sword *dx, sword *dy)
{
  union REGS regs;

  regs.x.ax = MOUSE_GETMOTION;
  int86(MOUSE_INT, &regs, &regs);
  *dx=regs.x.cx;
  *dy=regs.x.dx;
}

sword init_mouse(MOUSE *mouse)
{
  sword dx,dy;
  union REGS regs;

  regs.x.ax = MOUSE_RESET;
  int86(MOUSE_INT, &regs, &regs);
  mouse->on=regs.x.ax;
  mouse->num_buttons=regs.x.bx;
  mouse->button1=0;
  mouse->button2=0;
  mouse->button3=0;
  mouse->x=SCREEN_WIDTH/2;
  mouse->y=SCREEN_HEIGHT/2;
  get_mouse_motion(&dx,&dy);
  return mouse->on;
}

sword get_mouse_press(sword button)
{
  union REGS regs;

  regs.x.ax = MOUSE_GETPRESS;
  regs.x.bx = button;
  int86(MOUSE_INT, &regs, &regs);
  return regs.x.bx;
}

sword get_mouse_release(sword button)
{
  union REGS regs;

  regs.x.ax = MOUSE_GETRELEASE;
  regs.x.bx = button;
  int86(MOUSE_INT, &regs, &regs);
  return regs.x.bx;
}

