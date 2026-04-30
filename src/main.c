
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#ifndef VERSION
    #define VERSION "DEV"
#endif

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    double delta;
} AppState;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    if (!SDL_SetAppMetadata("SDL3 Teste", VERSION, "com.example.sdl3")) {
        SDL_Log("App Metadata failed! Cuz: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Initialization failed! Cuz: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    AppState *state = SDL_calloc(1, sizeof(AppState));
    if (!state) {
        SDL_Log("Initialization failed! Cuz: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    *appstate = state;

    const int kScreenWidth = 640;
    const int kScreenHeight = 480;

    if (!SDL_CreateWindowAndRenderer("SDL3 Teste", kScreenWidth, kScreenHeight, 0, &state->window, &state->renderer)) {
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

SDL_AppResult SDL_AppIterate(void *appstate) {
    AppState *state = ((AppState *)appstate);
    const Uint64 currentTime = SDL_GetTicks();
    static Uint64 prevTime = 0;

    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(state->renderer);

    double delta = ((double)(currentTime - prevTime)) / 1000;

    SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_SetRenderScale(state->renderer, 2, 2);
    SDL_RenderDebugTextFormat(state->renderer, 0, 0, "Delta: %f", delta);
    SDL_SetRenderScale(state->renderer, 1, 1);

    SDL_RenderPresent(state->renderer);
    prevTime = currentTime;

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    if (appstate != NULL) {
        AppState *state = ((AppState *)appstate);

        SDL_DestroyRenderer(state->renderer);
        SDL_DestroyWindow(state->window);
        SDL_free(state);
    }
}