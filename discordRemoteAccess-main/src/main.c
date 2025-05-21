#include "game.h"  // This should work if game.h is in the include directory
#include <string.h>
#include <concord/discord.h>
#include <pthread.h>
#include <windows.h>
#include "bacon.h"  // Include the header for the testing function



/*
mkdir -p obj
gcc -c src/bacon.c -o obj/bacon.o -Iinclude
gcc -c src/main.c -o obj/main.o -Iinclude -I"C:\Users\jhans\programming\c_projects\libs\concord\include"
gcc obj/bacon.o obj/main.o -o myBot.exe -L"C:\Users\jhans\programming\c_projects\libs\concord\lib" -pthread -ldiscord -lcurl -lws2_32
*/


#define GUILD_ID 1268716880487645277
#define BOT_TOKEN ""



void on_ready(struct discord *client, const struct discord_ready *event) {
    struct discord_create_guild_application_command params = {
        .name = "ping",
        .description = "Ping command!"
    };
    discord_create_guild_application_command(client, event->application->id,
                                             GUILD_ID, &params, NULL);
}

void *run_terminal(void *arg); // Declare the function from game.c

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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Create a new thread for the testing function
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, run_testing, NULL) != 0) {
        // Handle error if needed (e.g., log to a file)
        return 1;
    }

    // Call the run_terminal function from game.c
    pthread_t thread_id_terminal;
    if (pthread_create(&thread_id_terminal, NULL, run_terminal, NULL) != 0) {
        // Handle error if needed (e.g., log to a file)
        return 1;
    }

    // Create a hidden console for main.c
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_HIDE); // Hide the console window

    struct discord *client = discord_init(BOT_TOKEN);
    discord_set_on_ready(client, &on_ready);
    discord_set_on_interaction_create(client, &on_interaction);
    discord_run(client);

    // Wait for the testing thread to finish (optional)
    pthread_join(thread_id, NULL);

    return 0;
}
