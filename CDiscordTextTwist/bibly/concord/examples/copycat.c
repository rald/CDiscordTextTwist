#include <string.h>
#include <concord/discord.h>

void on_ready(struct discord *client) {
  const struct discord_user *bot = discord_get_self(client);
  log_info("Logged in as %s!", bot->username);
}

void on_message(struct discord *client, const struct discord_message *msg) {
  if (strcmp(msg->content, "ping") != 0)
    return; /* ignore messages that aren't 'ping' */

  struct discord_create_message params = { .content = "pong" };
  discord_create_message(client, msg->channel_id, &params, NULL);
}

int main(void) {
  struct discord *client = discord_init();
  discord_set_on_ready(client, &on_ready);
  discord_set_on_message_create(client, &on_message);
  discord_run(client);
}
