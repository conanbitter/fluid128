#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>

#include "fluid.h"

#define FRAME_WIDTH (960)
#define FRAME_HEIGHT (720)

SDL_Renderer* renderer;
SDL_Window* window;
SDL_Texture* frame_texture;

Uint64 last_time = 0;

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
    frame_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, FIELD_WIDTH, FIELD_HEIGHT);
    SDL_SetTextureScaleMode(frame_texture, SDL_SCALEMODE_NEAREST);

    bool quit = false;
    SDL_Event e;

    fluid_init();

    last_time = SDL_GetTicksNS();

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

        Uint64 dt = SDL_GetTicksNS() - last_time;
        fluid_update(((float)dt) / 1.0e9);
        last_time = SDL_GetTicksNS();

        void* texture_data;
        int pitch;
        SDL_LockTexture(frame_texture, NULL, &texture_data, &pitch);
        fluid_draw(texture_data);
        SDL_UnlockTexture(frame_texture);

        SDL_SetRenderDrawColor(renderer, 70, 100, 140, 255);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, frame_texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}



