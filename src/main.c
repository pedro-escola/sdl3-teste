
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

static SDL_Window *gWindow = NULL;
static SDL_Renderer *gRenderer = NULL;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Initialization failed! Cuz: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    const int kScreenWidth = 640;
    const int kScreenHeight = 480;

    if (!SDL_CreateWindowAndRenderer("SDL3 Teste", kScreenWidth, kScreenHeight, 0, &gWindow, &gRenderer)) {
        SDL_Log("Window creation failed! Cuz: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {}
void SDL_AppQuit(void *appstate, SDL_AppResult result) {}