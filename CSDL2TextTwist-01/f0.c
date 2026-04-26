#include <stdio.h>

int f(int c) {
	return (int)((double)c/255*63);
}

int main() {
	int r,g,b;
	int R,G,B;

	while(fscanf(stdin,"%d,%d,%d",&r,&g,&b)==3) {
		R=f(r);
		G=f(g);
		B=f(b);
		printf("%d,%d,%d\n",R,G,B);
	}

}