#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include <concord/discord.h>

#include "common.h"

#define TEXTTWIST_IMPLEMENTATION
#include "texttwist.h"

#define PFX "."

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

	if(strstr(msg->content,PFX"start")!=NULL) {
		sendf(client,msg,"hello");
	}

}



int main(void) {

	srand(time(NULL));

	struct discord *client = discord_init("OTkzNjA0NTQxMDIwOTAxNTE2.GgdWPc.FeH5SrlxX1WzhKxXMjFEINTUXm8oyT81ru59OI");
	discord_set_on_ready(client, &on_ready);
	discord_set_on_message_create(client, &on_message);
	discord_run(client);
}
