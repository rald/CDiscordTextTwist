#include <time.h>

#include "common.h"

#define TEXTTWIST_IMPLEMENTATION
#include "texttwist.h"

char **dwords=NULL;
size_t ndwords=0;

char **rwords=NULL;
size_t nrwords=0;

char **anagrams=NULL;
size_t nanagrams=0;

int main() {

	srand(time(NULL));

	TextTwist_LoadWords(WORDLIST_FILE,&dwords,&ndwords,3,8);
	TextTwist_LoadWords(RANDLIST_FILE,&rwords,&nrwords,6,8);

	char *rword=rwords[rand()%nrwords];

	TextTwist_Anagrams(&anagrams,&nanagrams,dwords,ndwords,rword);
	printf("%s\n\n",rword);
	for(size_t i=0;i<nanagrams;i++) {
		if(i!=0) printf(", ");
		printf("%s",anagrams[i]);
	}
	printf("\n");	
	
	return 0;
}
