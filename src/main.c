#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>

#define FRAME_WIDTH (960)
#define FRAME_HEIGHT (720)

SDL_Renderer* renderer;
SDL_Window* window;

int main() {
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Fluid128", FRAME_WIDTH, FRAME_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, NULL);

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type)
            {
            case SDL_EVENT_QUIT:
                quit = true;
                break;

            case SDL_EVENT_KEY_DOWN:
                switch (e.key.key)
                {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 70, 100, 140, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}



