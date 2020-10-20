#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string>

#include <SDL.h>
#include <SDL_image.h>

// TODO: Re-work this file to handle errors better
// Utility macros
#define CHECK_FATAL(test, message) \
    do { \
        if((test)) { \
            fprintf(stderr, "%s\n", (message)); \
            exit(1); \
        } \
    } while(0)

#define CHECK_ERROR(test, message) \
    do { \
        if((test)) { \
            fprintf(stderr, "%s\n", (message)); \
			return true; \
        } \
		return false; \
    } while(0)


SDL_Window* createWindow(std::string title, int width, int height);
SDL_Renderer* createRenderer(SDL_Window* window);
SDL_Surface* loadSurface(std::string path);
SDL_Surface* convertSurface(SDL_Surface* surface, SDL_Window* window);
SDL_Texture* createTexture(SDL_Renderer* renderer, SDL_Surface* surface);

// Get a random number from 0 to 255
int randInt(int rmin, int rmax) {
    return rand() % rmax + rmin;
}

//Key press surfaces constants
enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};
    
SDL_Window* createWindow(const char *title, int width, int height)
{
    SDL_Window *window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
    CHECK_FATAL(window == NULL, SDL_GetError());
	return window;
}

SDL_Renderer* createRenderer(SDL_Window* window)
{
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);    
    CHECK_FATAL(renderer == NULL, SDL_GetError());
	return renderer;
}

SDL_Surface* loadSurface(const char *path)
{
	SDL_Surface *surface = SDL_LoadBMP(path);
	CHECK_FATAL(surface == NULL, SDL_GetError());
	return surface;
}

SDL_Surface* convertSurface(SDL_Surface* surface, SDL_Window* window)
{
	SDL_Surface *windowSurface = SDL_GetWindowSurface(window);
	CHECK_FATAL(windowSurface == NULL, SDL_GetError());

	SDL_Surface *convertedSurface = SDL_ConvertSurface(surface, windowSurface->format, 0);
	CHECK_FATAL(convertedSurface == NULL, SDL_GetError());

	SDL_FreeSurface(surface);
	return convertedSurface;
}

SDL_Texture* createTexture(SDL_Renderer* renderer, SDL_Surface* surface)
{
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	CHECK_FATAL(texture == NULL, SDL_GetError());
	return texture;
}

void loadRendererWithSurfaceToTexture(SDL_Renderer* renderer, SDL_Surface* surface)
{
	SDL_Texture *texture = createTexture(renderer, surface);
	CHECK_FATAL(SDL_RenderCopy(renderer, texture, NULL, NULL) < 0, SDL_GetError());
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

int main(int argc, char **argv) {
    // Initialize the random number generator
    srand((unsigned int)time(NULL));

    // Initialize SDL
    CHECK_FATAL(SDL_Init(SDL_INIT_VIDEO) != 0, SDL_GetError());

	SDL_Window *window = createWindow("Hello SDL", 800, 600);

	SDL_Renderer *renderer = createRenderer(window);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	
    bool running = true;
    SDL_Event event;
    while(running) {
        // Process events
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = false;
            } else if(event.type == SDL_KEYDOWN) {
				SDL_RenderClear(renderer);

				switch(event.key.keysym.sym)
				{
					case 'c':
						SDL_SetRenderDrawColor(renderer, randInt(0, 255), randInt(0, 255), randInt(0, 255), 255);
						break;
					case SDLK_RIGHT:
						loadRendererWithSurfaceToTexture(renderer, loadSurface("../assets/right.bmp"));
						break;
					case SDLK_LEFT:
						loadRendererWithSurfaceToTexture(renderer, loadSurface("../assets/left.bmp"));
						break;
					case SDLK_UP:
						loadRendererWithSurfaceToTexture(renderer, loadSurface("../assets/up.bmp"));
						break;
					case SDLK_DOWN:
						loadRendererWithSurfaceToTexture(renderer, loadSurface("../assets/down.bmp"));
						break;
					default:
						loadRendererWithSurfaceToTexture(renderer, loadSurface("../assets/hello_world.bmp"));
						break;
				}
            }
        }
        // Show what was drawn
        SDL_RenderPresent(renderer);
    }

    // Release resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


