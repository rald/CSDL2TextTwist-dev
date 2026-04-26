#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>


void load(char *fn,char ***w,size_t *nw) {
	char *line=NULL;
	size_t llen=0;
	ssize_t rlen=0;
	FILE *fp=fopen(fn,"r");
	while((rlen=getline(&line,&llen,fp))!=-1) {
		line[strcspn(line,"\n")]='\0';
		(*w)=realloc(*w,sizeof(**w)*((*nw)+1));
		(*w)[(*nw)++]=strdup(line);
	}
	free(line);
	fclose(fp);
}

bool isanagram(char *w0, char *w1) {
    int f0[26] = {0};
    int f1[26] = {0};

    for(int i=0; w0[i]; i++) if(isalpha(w0[i])) f0[tolower(w0[i])-'a']++;
    for(int i=0; w1[i]; i++) if(isalpha(w1[i])) f1[tolower(w1[i])-'a']++;

    for(int i=0; i<26; i++) {
        if(f0[i] < f1[i]) return false; 
    }
    return true;
}

void getanagrams(char ***a,size_t *na,char **d,size_t nd,char *w0) {
	for(int i=0;i<nd;i++) {
		if(isanagram(w0,d[i])) {
			(*a)=realloc(*a,sizeof(**a)*((*na)+1));
			(*a)[(*na)++]=strdup(d[i]);
			if((*na)>=20) break;
		}
	}
}

void freearray(char ***a,size_t *na) {
	for(int i=0;i<(*na);i++) {
		free((*a)[i]);
		(*a)[i]=NULL;
	}
	free(*a);
	(*a)=NULL;
	(*na)=0;
}

int main(void) {

	char **d=NULL;
	size_t nd=0;

	char **r=NULL;
	size_t nr=0;

	char **a=NULL;
	size_t na=0;

	load("dict.txt",&d,&nd);
	load("rand.txt",&r,&nr);

	FILE *fp=fopen("rand2.txt","w");

	for(int i=0;i<nr;i++) {
		getanagrams(&a,&na,d,nd,r[i]);

		if(na>=10 && na<=20) {
			fprintf(fp,"%s\n",r[i]);
			printf("%s\n",r[i]);
		}

		freearray(&a,&na);
	}

	freearray(&r,&nr);
	freearray(&d,&nd);

	return 0;
}
