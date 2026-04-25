#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#include "sweetie.h"

#define GAME_TITLE "TextTwist"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

bool quit=false;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event event;

bool moving=false;

typedef struct {
	int letter;
	int x,y;
	int dx,dy;
} Ball;

Ball *Ball_New(int letter,int x,int y) {
	Ball *ball=malloc(sizeof(*ball));
	if(!ball) return NULL;
	ball->letter=letter;
	ball->x=x;
	ball->y=y;
	ball->dx=0;
	ball->dy=0;
	return ball;
}

void Ball_Draw(Ball *ball,SDL_Renderer *renderer,SDL_Texture *texture,SDL_Rect *clips) {
	int letter=ball->letter-'a';
	SDL_RenderCopy(renderer, texture, &clips[26],&(SDL_Rect){ball->x,ball->y,clips[26].w,clips[26].h});
	SDL_RenderCopy(renderer, texture, &clips[letter],&(SDL_Rect){ball->x+(clips[26].w-clips[letter].w)/2,ball->y+(clips[26].h-clips[letter].h)/2,clips[letter].w,clips[letter].h});
}

int sgn(int x) {
	return x>0?1:(x<0?-1:0);
}

void SetColor(int c) {
	SDL_SetRenderDrawColor(renderer, sweetie[c*3+0], sweetie[c*3+1], sweetie[c*3+2], 255);
}

void DrawChar(SDL_Renderer *renderer,SDL_Texture *texture,SDL_Rect *clips,int x,int y,int frame) {
	SDL_RenderCopy(renderer, texture, &clips[26],&(SDL_Rect){x,y,clips[26].w,clips[26].h});
	SDL_RenderCopy(renderer, texture, &clips[frame],&(SDL_Rect){x+(clips[26].w-clips[frame].w)/2,y+(clips[26].h-clips[frame].h)/2,clips[frame].w,clips[frame].h});
}

char *randline(char *fn) {
	FILE *fp=fopen(fn,"r");
	int count=0;

	char *line=NULL;
	size_t llen=0;
	ssize_t rlen=0;
	
	char *chosen=NULL;

	while((rlen=getline(&line,&llen,fp))!=-1) {
		line[strcspn(line,"\n")]='\0';
		count++;
		if(rand()%count==0) {
			if(chosen) free(chosen);
			chosen=strdup(line);
		}
	}
	free(line);
	return chosen;	
}

void BallShuffle(Ball **balls,size_t nballs) {
	int x[nballs],y[nballs],t;

	for(int i=0;i<nballs;i++) {
		x[i]=balls[i]->x;
		y[i]=balls[i]->y;
	}		

	for(int i=nballs-1;i>0;i--) {
		int j=rand()%(i+1);

		t=x[i];
		x[i]=x[j];
		x[j]=t;

		t=y[i];
		y[i]=y[j];
		y[j]=t;
	}
	
	for(int i=0;i<nballs;i++) {
		balls[i]->dx=x[i];
		balls[i]->dy=y[i];
	}		
}

char *WordShuffle(char *w0) {
	char *w1=strdup(w0);
	int t;
	for(int i=strlen(w1)-1;i>0;i--) {
		int j=rand()%(i+1);

		t=w1[i];
		w1[i]=w1[j];
		w1[j]=t;
	}

	return w1;	
}

int main(void) {

	srand(time(NULL));

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr,"Error: SDL_Init SDL_Error: %s\n", SDL_GetError());
		return 1;
	}
	
    window = SDL_CreateWindow(
        GAME_TITLE,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (window == NULL) {
        fprintf(stderr,"Error SDL_CreateWindow SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == NULL) {
        fprintf(stderr,"Error SDL_CreateRenderer SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
	}
	
	SDL_Surface* surface = SDL_LoadBMP("sprites-00.bmp");
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0xFF, 0x00 , 0xFF));
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

	SDL_Rect clips[27];
    for(int i = 0; i < 26; i++) {
        clips[i].x = (i%13)*32;
        clips[i].y = (i/13)*32;
        clips[i].w = 32;
        clips[i].h = 32;
    }
    clips[26].x = 0;
    clips[26].y = 64;
    clips[26].w = 64;
    clips[26].h = 64;
    
    char *word=randline("rand.txt");
    char *shuffled=WordShuffle(word);
    
    size_t nballs=strlen(shuffled);
    
    Ball *balls[nballs];
    
    for(int i=0;i<nballs;i++) {
    	balls[i]=Ball_New(shuffled[i],i*64,0);
    }
            
	while(!quit) {
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_QUIT: quit=true; break;
			case SDL_KEYDOWN: 
				switch(event.key.keysym.sym) {
					case SDLK_ESCAPE: quit=true; break;
					case SDLK_SPACE: if(!moving) { BallShuffle(balls,nballs); moving=true; } break;
					default: break;
				}
			break;
			default: break;
			}
		}


		if(moving) {
			moving=false;
			for(int i=0;i<nballs;i++) {
				balls[i]->x+=sgn(balls[i]->dx-balls[i]->x);
				balls[i]->y+=sgn(balls[i]->dy-balls[i]->y);
				if(balls[i]->x!=balls[i]->dx || balls[i]->y!=balls[i]->dy) moving=true;
			}
		}

		SetColor(6);
		SDL_RenderClear(renderer);
		
		for(int i=0;i<16;i++) {
			SetColor(i);
			SDL_RenderFillRect(renderer,&(SDL_Rect){i*32,SCREEN_HEIGHT-32,32,32});
		}
		
		for(int i=0;i<nballs;i++) {
			Ball_Draw(balls[i],renderer,texture,clips);
		}
		
		SDL_RenderPresent(renderer);		
	
	}	
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
		
	SDL_Quit();
	
	return 0;
}
