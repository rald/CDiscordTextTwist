#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STRING_MAX 256 

char *trim(char *a) {
    char *p = a, *q = a;
    while (isspace(*q)) ++q;
    while (*q) *p++ = *q++;
    *p = '\0';
    while (p > a && isspace(*--p)) *p = '\0';
	return a;
}


void addWord(char ***words,size_t *nwords,const char *word) {
	(*words)=realloc(*words,sizeof(**words)*(*nwords+1));
	(*words)[(*nwords)++]=strdup(word);	
}

ssize_t indexOf(char **words,size_t nwords,const char *word) {
	for(size_t i=0;i<nwords;i++) {
		if(strcasestr(word,words[i])!=NULL) return i;
	}
	return -1;
}

int main(int argc,char **argv) {

	char line[STRING_MAX];

	char **badwords=NULL;
	size_t nbadwords=0;

	FILE *fin;

	char *p;

	fin=fopen(argv[1],"r");
	while(fgets(line,STRING_MAX,fin)) {

		p=strchr(line,'\n');
		if(p) *p='\0';

		addWord(&badwords,&nbadwords,trim(line));
	}
	fclose(fin);

	fin=fopen(argv[2],"r");
	while(fgets(line,STRING_MAX,fin)) {

		p=strchr(line,'\n');
		if(p) *p='\0';

		if(indexOf(badwords,nbadwords,trim(line))==-1) {
			printf("%s\n",line);
		} 
	}

	return 0;
}
