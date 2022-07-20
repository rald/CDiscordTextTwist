#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "common.h"

char *trim(char *a) {
		char *p = a, *q = a;
		while (isspace(*q))            ++q;
		while (*q)                     *p++ = *q++;
		*p = '\0';
		while (p > a && isspace(*--p)) *p = '\0';
	return a;
}

static void addWord(char ***words,size_t *nwords,const char *word) {
	(*words)=realloc(*words,sizeof(**words)*(*nwords+1));
	(*words)[(*nwords)++]=strdup(word);	
}

int TextTwist_LoadWords(const char *path,char ***words,size_t *nwords,size_t min,size_t max) {

	FILE *fin;
	char line[STRING_MAX];

	if((fin=fopen(path,"r"))==NULL) {
		fprintf(stderr,"Cannot open file %s.\n",path);
		return 1;
	}

	while(fgets(line,STRING_MAX,fin)) {
		char *p=strchr(line,'\n'); 
		if(p) *p='\0'; 
		trim(line);
		if(strlen(line)>=min && strlen(line)<=max) {
			addWord(words,nwords,line);			
		}
	}

	return 0;	
}

static int *freq(const char *w) {
	int *f=calloc(26,sizeof(*f));

	for(size_t i=0;i<strlen(w);i++) {
		char ch=toupper(w[i]);
		if(ch>='A' && ch<='Z') {
			f[ch-'A']++;
		}
	}

	return f;
}

static bool isAllZero(int *f) {
	for(size_t i=0;i<26;i++) {
		if(f[i]!=0) return false;
	}
	return true;
}

bool TextTwist_isAnagram(const char *w1,const char *w2) {

	bool result=true;

	int *f1=freq(w1);
	int *f2=freq(w2);

	if(isAllZero(f1) || isAllZero(f2)) {
		result=false;
	} else {
		for(size_t i=0;i<26;i++) {
			if(f1[i]<f2[i]) { 
				result=false; 
				break;
			}
		}
	}
	
	free(f1);
	free(f2);

	return result;
}

void TextTwist_Anagrams(char ***anagrams,size_t *nanagrams,char **words,size_t nwords,const char *word) {
	for(size_t i=0;i<nwords;i++) {
		if(TextTwist_isAnagram(word,words[i])) {
			addWord(anagrams,nanagrams,words[i]);
			if((*nanagrams)>30) break;
		}
	}
}

ssize_t indexOf(char **words,size_t nwords,const char *word) {
	for(size_t i=0;i<nwords;i++) {
		if(strcasestr(word,words[i])!=NULL) return i;
	}
	return -1;
}

int main(int argc,char **argv) {

	char **dwords=NULL;
	size_t ndwords=0;

	char **rwords=NULL;
	size_t nrwords=0;
	
	char **anagrams=NULL;
	size_t nanagrams=0;

	FILE *fout=fopen(argv[3],"w");

	TextTwist_LoadWords(argv[1],&dwords,&ndwords,3,8);
	TextTwist_LoadWords(argv[2],&rwords,&nrwords,6,8);

	for(size_t i=0;i<nrwords;i++) {
		printf("%6.2f%%\r",(float)i/nrwords*100);
		TextTwist_Anagrams(&anagrams,&nanagrams,dwords,ndwords,rwords[i]);
		if(nanagrams>=10 && nanagrams<=30) {
			fprintf(fout,"%s\n",rwords[i]);
		}

		for(size_t i=0;i<nanagrams;i++) {
			free(anagrams[i]);
			anagrams[i]=NULL;
		}
		nanagrams=0;
		free(anagrams);
		anagrams=NULL;

	}
	fclose(fout);
	printf("\n");


	return 0;
}
