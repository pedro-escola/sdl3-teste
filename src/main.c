
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#ifndef VERSION
    #define VERSION "DEV"
#endif

const int PlayerSize = 32;
const int kScreenWidth = 640;
const int kScreenHeight = 480;

typedef struct Player {
    float x;
    float y;
    float ySpeed;
    SDL_FRect *rect;
} Player;

typedef struct AppState {
    SDL_Window *window;
    SDL_Renderer *renderer;
    Player *player;
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

    if (!SDL_CreateWindowAndRenderer("SDL3 Teste", kScreenWidth, kScreenHeight, 0, &state->window, &state->renderer)) {
        SDL_Log("Window creation failed! Cuz: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    Player *player = SDL_malloc(sizeof(Player));
    if (player == NULL) {
        SDL_Log("Player creation failed! Cuz: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    player->x = 48;
    player->y = kScreenHeight/2 - PlayerSize/2;

    SDL_FRect *rect = SDL_malloc(sizeof(SDL_FRect));
    if (rect == NULL) {
        SDL_Log("Rect creation failed! Cuz: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    rect->h = rect->w = PlayerSize;
    rect->x = player->x, rect->y = player->y;

    player->rect = rect;
    state->player = player;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    AppState *state = (AppState *)appstate;

    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
            break;
        case SDL_EVENT_KEY_DOWN:
            if (event->key.scancode == SDL_SCANCODE_SPACE || event->key.scancode == SDL_SCANCODE_W || event->key.scancode == SDL_SCANCODE_UP) {
                state->player->ySpeed = 8 * 60;
            }
        default:
            break;
    }
    return SDL_APP_CONTINUE;
}

void handle_player(AppState *state) {
    SDL_assert(state->player != NULL);
    Player *player = state->player;

    player->y -= player->ySpeed * state->delta;
    player->ySpeed -= 30 * 60 * state->delta;

    if (player->y < 0) {
        player->y = player->ySpeed = 0;
    } else if (player->y > (kScreenHeight - PlayerSize)) {
        player->y = kScreenHeight - PlayerSize;
        player->ySpeed = 0;
    }

    if (player->ySpeed < -600) {
        player->ySpeed = -600;
    }
    
    SDL_FRect *rect = state->player->rect;
    rect->x = player->x, rect->y = player->y;
    SDL_RenderRect(state->renderer, rect);
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    AppState *state = ((AppState *)appstate);
    const Uint64 currentTime = SDL_GetTicks();
    static Uint64 prevTime = 0;

    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(state->renderer);

    state->delta = ((double)(currentTime - prevTime)) / 1000;

    SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    handle_player(state);

    SDL_RenderPresent(state->renderer);
    prevTime = currentTime;

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    if (appstate != NULL) {
        AppState *state = ((AppState *)appstate);

        SDL_DestroyRenderer(state->renderer);
        SDL_DestroyWindow(state->window);

        if (state->player != NULL) {
            SDL_free(state->player->rect);
            SDL_free(state->player);
        }

        SDL_free(state);
    }
}