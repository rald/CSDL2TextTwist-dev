#include "graphics.h"

#include <dos.h>

byte *VGA=(byte*)0xA0000000L;
word *my_clock=(word*)0x0000046C;

void SetMode(byte mode) {
	union REGS regs;
	regs.h.ah=0x00;
	regs.h.al=mode;
	int86(0x10,&regs,&regs);
}

void SetPalette() {
	int i;
	outp(0x03C8,0);
	for(i=0;i<16;i++) {
		outp(0x03C9,palette[i*3+0]);
		outp(0x03C9,palette[i*3+1]);
		outp(0x03C9,palette[i*3+2]);
	}
	for(i=16;i<256;i++) {
		outp(0x03C9,palette[0]);
		outp(0x03C9,palette[1]);
		outp(0x03C9,palette[2]);
	}
}

void vsync(void) {
	while((inp(INPUT_STATUS) & VRETRACE));
	while(!(inp(INPUT_STATUS) & VRETRACE));
}

void DrawPoint(byte *srf,int x,int y,byte c) {
	if(x<0 || x>=320 || y<0 || y>=200) return;
	srf[(y<<8)+(y<<6)+x]=c;
}

int ReadPoint(byte *srf,int x,int y) {
	if(x<0 || x>=320 || y<0 || y>=200) return -1;
	return srf[(y<<8)+(y<<6)+x];
}

void DrawLine(byte *srf,int x0,int y0,int x1,int y1,byte c) {
	int dx=abs(x1-x0),sx=x0<x1?1:-1;
	int dy=abs(y1-y0),sy=y0<y1?1:-1;
	int e1=(dx>dy?dx:-dy)/2,e2;
	for(;;) {
		DrawPoint(srf,x0,y0,c);
		if(x0==x1 && y0==y1) break;
		e2=e1;
		if(e2 > -dx) { e1-=dy; x0+=sx; }
		if(e2 <  dy) { e1+=dx; y0+=sy; }
	}
}

void DrawRect(byte *srf,int x,int y,int w,int h,byte c) {
	int i,j;

	for(i=0;i<w;i++) {
		DrawPoint(srf,x+i,y,c);
		DrawPoint(srf,x+i,y+h-1,c);
	}

	for(j=0;j<h;j++) {
		DrawPoint(srf,x,y+j,c);
		DrawPoint(srf,x+w-1,y+j,c);
	}

}

void FillRect(byte *srf,int x,int y,int w,int h,byte c) {
	int i,j;
	for(j=0;j<h;j++) {
		for(i=0;i<w;i++) {
			DrawPoint(srf,x+i,y+j,c);
		}
	}
}
