#include <string.h>
#include <concord/discord.h>
#include <pthread.h>

// Add this line
extern int testing(void);

//sudo gcc main.c -o myBot -pthread -ldiscord -lcurl
/*
gcc -c bacon.c -o bacon.o
gcc -c main.c -o main.o -I"C:\Users\jhans\c_projects\libs\concord\include"
gcc bacon.o main.o -o myBot.exe -L"C:\Users\jhans\c_projects\libs\concord\lib" -pthread -ldiscord -lcurl -lws2_32
*/


#define GUILD_ID 1268716880487645277
#define BOT_TOKEN "MTI4MjEzMjQ2ODQwODc3ODg1Nw.GFkQFV.kbe_V3hN6cMoX_hpbWjdeR-n51FPKAM8C_FQHQ"



void on_ready(struct discord *client, const struct discord_ready *event) {
    struct discord_create_guild_application_command params = {
        .name = "ping",
        .description = "Ping command!"
    };
    discord_create_guild_application_command(client, event->application->id,
                                             GUILD_ID, &params, NULL);
}

void on_interaction(struct discord *client, const struct discord_interaction *event) {
    if (event->type != DISCORD_INTERACTION_APPLICATION_COMMAND)
        return; /* return if interaction isn't a slash command */

    if (strcmp(event->data->name, "ping") == 0) {
          struct discord_interaction_response params = {
                .type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE,
                .data = &(struct discord_interaction_callback_data){
                      .content = "pong"
                }
          };
          discord_create_interaction_response(client, event->id,
                                              event->token, &params, NULL);
    }
}

void *run_testing(void *arg) {
    testing();
    return NULL;
}

int main(void) {
    pthread_t thread_id;
    
    // Create a new thread for the testing function
    if (pthread_create(&thread_id, NULL, run_testing, NULL) != 0) {
        fprintf(stderr, "Failed to create thread\n");
        return 1;
    }

    struct discord *client = discord_init(BOT_TOKEN);
    discord_set_on_ready(client, &on_ready);
    discord_set_on_interaction_create(client, &on_interaction);
    discord_run(client);

    // Wait for the testing thread to finish (optional)
    pthread_join(thread_id, NULL);

    return 0;
}