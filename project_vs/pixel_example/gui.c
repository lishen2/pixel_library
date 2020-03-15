#include "SDL2/SDL.h"
#include <stdio.h>
#include <math.h>

static SDL_Window *window = NULL;                    // Declare a pointer
static SDL_Renderer *renderer;

// Simplified interface to SDL's fillrect call
static void drawrect(int x, int y, int w, int h, int color)
{
	SDL_SetRenderDrawColor(renderer, 
		(color >> 16) & 0xFF, 
		(color >> 8) & 0xFF, 
		(color >> 0) & 0xFF, 
		SDL_ALPHA_OPAQUE);

	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	SDL_RenderFillRect(renderer, &r);
}

// Rendering function
static void render()
{
	// clear screen
	drawrect(0, 0, 640, 480, 0);

	// test that the fillrect is working
	drawrect(64, 48, 64, 48, 0xff);

	// update the screen
	SDL_RenderPresent(renderer);

	// don't take all the cpu time
	SDL_Delay(10);
}

int main2(int argc, char* argv[])
{
	SDL_Event e;
	int quit = 0;
	int ret;

	ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);         // Initialize SDL2
	if (0 != ret) {
		printf("SDL_Init Error: %s\r\n", SDL_GetError());
		return 1;
	}
	// Create an application window with the following settings:
	window = SDL_CreateWindow(
		"An SDL2 window",                  // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		640,                               // width, in pixels
		480,                               // height, in pixels
		SDL_WINDOW_OPENGL                  // flags - see below
	);

	// Check that the window was successfully created
	if (window == NULL) {
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	// The window is open: could enter program loop here (see SDL_PollEvent())
	quit = 0;
	while (1 != quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				quit = 1;
			}
			else if (e.type == SDL_KEYDOWN) {
			}
			else if (e.type == SDL_KEYUP) {
				// If escape is pressed, return (and thus, quit)
				if (e.key.keysym.sym == SDLK_ESCAPE) {
					quit = 1;
					break;
				}
			}
		}
		render();
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();
	return 0;
}
