
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

SDL_Window *gWindow = NULL;

bool init() {
    bool success = SDL_Init(SDL_INIT_VIDEO);
    if (!success) {
        SDL_Log("Initialization failed! Cuz: %s\n", SDL_GetError());
    }

    return success;
}

bool makeWindow() {
    const int kScreenWidth = 640;
    const int kScreenHeight = 480;

    gWindow = SDL_CreateWindow("SDL3 Teste", kScreenWidth, kScreenHeight, 0);
    if (gWindow == NULL) {
        SDL_Log("Window creation failed! Cuz: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void loop() {
    bool quit = false;

    SDL_Event e;
    SDL_zero(e);
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }
    }
}

void end() {
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}

int main(int argc, char** argv) {
    if (!init()) { return -1; }

    if (!makeWindow()) { return -2; }

    loop();

    end();
    return 0;
}