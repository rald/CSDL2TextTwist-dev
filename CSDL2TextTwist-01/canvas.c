#include "canvas.h"

Canvas *Canvas_New(char *fn) {
	Canvas *c=(Canvas*)malloc(sizeof(Canvas));
	FILE *fp=fopen(fn,"r");
	char *h="0123456789ABCDEF";
	int i,j,k;
	int ch;

	if(!c) {
		fprintf(stderr,"Error: allocating canvas\n");
		exit(1);
	}

	if(!fp) {
		fprintf(stderr,"Error: opening %s: %s\n",fn,strerror(errno));
		exit(1);
	}

	if(fscanf(fp,"%d,%d,%d,%d",&c->w,&c->h,&c->f,&c->t)!=4) {
		fprintf(stderr,"Error: reading header of file %s: %s\n",fn,strerror(errno));
		exit(1);
	}

	c->p=(byte*)calloc(c->w*c->h*c->f,sizeof(byte));

	if(!c->p) {
		fprintf(stderr,"Error: allocating canvas pixels\n");
		exit(1);
	}

	k=0;
	while((ch=fgetc(fp))!=EOF) {
		j=-1;
		for(i=0;i<16;i++) {
			if(ch==h[i]) {
				j=i;
				break;
			}
		}
		if(j!=-1) {
			c->p[k++]=j;
		}
	}
	fclose(fp);

	return c;
}

void Canvas_Free(Canvas *c) {
	free(c->p);
	free(c);
}

void Canvas_DrawChar(Canvas *c,int x,int y,int s,int f,byte fg,byte bg) {
	int i,j,k;
	for(j=0;j<c->h;j++) {
		for(i=0;i<c->w;i++) {
			k=f*c->w*c->h+j*c->w+i;
			if(c->p[k]==c->t) continue;
			if(c->p[k]==0) {
				FillRect(x+i*s,y+j*s,s,s,bg);
			} else {
				FillRect(x+i*s,y+j*s,s,s,fg);
			}
		}
	}
}
