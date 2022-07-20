#ifndef TEXTTWIST_H
#define TEXTTWIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define UTIL_IMPLEMENTATION
#include "util.h" 


int TextTwist_LoadWords(const char *path,char ***words,size_t *nwords,size_t min,size_t max);
char *TextTwist_ShuffleWord(char *word);
bool TextTwist_isAnagram(const char *w1,const char *w2);
void TextTwist_Anagrams(char ***anagrams,size_t *nanagrams,char **words,size_t nwords,const char *word);


#ifdef TEXTTWIST_IMPLEMENTATION



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


char *TextTwist_ShuffleWord(char *word) {
	for(size_t i=strlen(word)-1;i>0;i--) {
		size_t j=rand()%(i+1);
		char tmp=word[i];
		word[i]=word[j];
		word[j]=tmp;
	}
	return word;
} 



void TextTwist_ShuffleWords(char **words,size_t nwords) {
	for(size_t i=nwords-1;i>0;i--) {
		size_t j=rand()%(i+1);
		char *tmp=words[i];
		words[i]=words[j];
		words[j]=tmp;
	}
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
		}
	}
}


#endif /* TEXTTWIST_IMPLEMENTATION */



#endif /* TEXTTWIST_H */
