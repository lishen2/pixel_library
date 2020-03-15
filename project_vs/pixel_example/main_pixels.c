// Example program:
// Using SDL2 to create an application window

#include "SDL2/SDL.h"
#include <stdio.h>
#include <math.h>

static SDL_Window *window = NULL;                    // Declare a pointer
static SDL_Surface* surface = NULL;

const unsigned char sprite[] =
{
0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,
0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,
0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,
0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0
};

static void drawsprite(int x, int y, unsigned int color)
{
	int i, j, c, yofs;
	yofs = y * (surface->pitch / 4) + x;
	for (i = 0, c = 0; i < 16; i++)
	{
		for (j = 0; j < 16; j++, c++)
		{
			if (sprite[c])
			{
				((unsigned int*)surface->pixels)[yofs + j] = color;
			}
		}
		yofs += (surface->pitch / 4);
	}
}

static void putpixel(int x, int y, int color)
{
	unsigned int *ptr = (unsigned int*)surface->pixels;
	int lineoffset = y * (surface->pitch / 4);
	ptr[lineoffset + x] = color;
}

static void render(void)
{
	//Ëø¶¨surface
	SDL_LockSurface(surface);

	// Ask SDL for the time in milliseconds
	int tick = SDL_GetTicks();

#if 0


	// Declare a couple of variables
	int i, j, yofs, ofs;

	// Draw to screen
	yofs = 0;
	for (i = 0; i < 480; i++)
	{
		for (j = 0, ofs = yofs; j < 640; j++, ofs++)
		{
			((unsigned int*)surface->pixels)[ofs] = i * i + j * j + tick;
		}
		yofs += surface->pitch / 4;
	}

	putpixel(10, 10, 0xFF0000);
	putpixel(11, 10, 0xFF0000);
	putpixel(10, 11, 0xFF0000);
	putpixel(11, 11, 0xFF0000);
#endif

	int i;
	for (i = 0; i < 128; i++)
	{
		drawsprite((int)(320 + sin((tick + i * 10) * 0.003459734f) * 300),
			(int)(240 + sin((tick + i * 10) * 0.003345973f) * 220),
			((int)(sin((tick * 0.2f + i) * 0.234897f) * 127 + 128) << 16) |
			((int)(sin((tick * 0.2f + i) * 0.123489f) * 127 + 128) << 8) |
			((int)(sin((tick * 0.2f + i) * 0.312348f) * 127 + 128) << 0));
	}

	// Unlock if needed
	SDL_UnlockSurface(surface);

	SDL_UpdateWindowSurface(window);
}

int main1(int argc, char* argv[]) 
{
	SDL_Event e;
	int quit = 0;
	int ret;

	ret = SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS);         // Initialize SDL2
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

	surface = SDL_GetWindowSurface(window);

	// The window is open: could enter program loop here (see SDL_PollEvent())
	quit = 0;
	while (1 != quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				quit = 1;
			} else if (e.type == SDL_KEYDOWN) {
			} else if (e.type == SDL_KEYUP) {
				// If escape is pressed, return (and thus, quit)
				if (e.key.keysym.sym == SDLK_ESCAPE) {
					quit = 1;
					break;
				}
			}
		}
		render();
	}

	SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();
	return 0;
}
