#include <concord/discord.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include "common.h"

#define TEXTTWIST_IMPLEMENTATION
#include "texttwist.h"



enum GameState  {
	GAMESTATE_INIT=0,
	GAMESTATE_START
} gameState=GAMESTATE_INIT;


bool gameStarted=false;



char **dwords=NULL;
size_t ndwords=0;

char **rwords=NULL;
size_t nrwords=0;

char **anagrams=NULL;
size_t nanagrams=0;

char *rword;
char *sword;

bool *guessed=NULL;
size_t nguessed=0;

char *secretWord=NULL;



void sendf(struct discord *client, const struct discord_message *msg, const char *fmt, ...) {

	char message[STRING_MAX];

	va_list arg;

	va_start(arg,fmt);

		vsprintf(message,fmt,arg);

		struct discord_create_message params = { .content = message };

		discord_create_message(client, msg->channel_id, &params, NULL);

	va_end(arg);

}



void on_ready(struct discord *client, const struct discord_ready *rdy) {
	const struct discord_user *bot = discord_get_self(client);
	log_info("Logged in as %s!", bot->username);
}



void on_message(struct discord *client, const struct discord_message *msg) {

	const struct discord_user *bot = discord_get_self(client);
	if(strcasecmp(msg->author->username,bot->username)==0) return;

	if(strcasecmp(msg->content,PFX "start")==0) {

		if(gameState==GAMESTATE_INIT) {
			gameStarted=true;
	
			rword=rwords[rand()%nrwords];

			if(anagrams) {
				for(size_t i=0;i<nanagrams;i++) {
					free(anagrams[i]);
					anagrams[i]=NULL;
				}
				nanagrams=0;
				free(anagrams);
				anagrams=NULL;
			}

			TextTwist_Anagrams(&anagrams,&nanagrams,dwords,ndwords,rword);

			secretWord=anagrams[rand()%nanagrams];

			if(guessed) {
				free(guessed);
				guessed=NULL;
			}

			guessed=malloc(sizeof(*guessed)*nanagrams);
			nguessed=0;

			for(size_t i=0;i<nanagrams;i++) guessed[i]=false;
	
			sword=strdup(rword);

			TextTwist_ShuffleWord(sword);

			TextTwist_ShuffleWords(anagrams,nanagrams);

			sendf(client,msg,GAME_TITLE " %s",sword);

			gameState=GAMESTATE_START;
		} else if(gameState==GAMESTATE_START) {
			sendf(client,msg,GAME_TITLE " game is already started");
		}

	} else if(strcasecmp(msg->content,PFX "twist")==0) {
		if(gameState==GAMESTATE_START) {
			TextTwist_ShuffleWord(sword);
			sendf(client,msg,GAME_TITLE " %s",sword);
		} else {
			sendf(client,msg,GAME_TITLE " game is not started");
		}

	} else if(strcasecmp(msg->content,PFX "list")==0) {

		if(gameState==GAMESTATE_START) {
			char message[STRING_MAX]="";
			char strnum[STRING_MAX]="";
			for(size_t i=0;i<nanagrams;i++) {
				sprintf(strnum,"(%zu) ",i+1);
				strcat(message,strnum);

				if(guessed[i]) {
					strcat(message,anagrams[i]);
				} else {
					for(size_t j=0;j<strlen(anagrams[i]);j++) {
						strcat(message,"?");
					}
				}

				strcat(message,"\n");
			}
			sendf(client,msg,GAME_TITLE "(%zu words)\n%s",nanagrams,message);
		} else {
			sendf(client,msg,GAME_TITLE " game is not started");
		}

	} else {

		char message[STRING_MAX];
		strcpy(message,msg->content);
		trim(message);

		for(size_t i=0;i<nanagrams;i++) {

			if(!guessed[i] && strcasecmp(message,anagrams[i])==0) {
				guessed[i]=true;
				nguessed++;

				int points=0;

				points+=strlen(anagrams[i]);

				message[0]='\0';

				if(strlen(anagrams[i])==strlen(rword)) {
					points+=longWordBonus;
					strcat(message," LONG WORD BONUS ");
				} 

				if(strcasecmp(anagrams[i],secretWord)==0) {
					points+=secretWordBonus;
					strcat(message," SECRET WORD BONUS ");
				} 

				sendf(client,msg,"%s you guessed %s plus %d points. %s",msg->author->username,anagrams[i],points,message);				
				break;
			}
		}

		if(nguessed==nanagrams) gameState=GAMESTATE_INIT;

	}
}



int main(void) {

	srand(time(NULL));

	TextTwist_LoadWords(WORDLIST_FILE,&dwords,&ndwords,3,8);
	TextTwist_LoadWords(RANDLIST_FILE,&rwords,&nrwords,6,8);

	struct discord *client = discord_init("OTkzNjA0NTQxMDIwOTAxNTE2.G46wTb.YLEOWKfnay9eRJlEq0NRka6a82qVvzyFtnGS3U");
	discord_set_on_ready(client, &on_ready);
	discord_set_on_message_create(client, &on_message);
	discord_run(client);
}


