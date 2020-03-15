#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "SDL2/SDL.h"
#include <io.h>  
#include <fcntl.h>

#include "px_common.h"
#include "px_config.h"
#include "px_core.h"
#include "px_user_func.h"

/* SDL screen size  */
#define PX_SDL_SCALE_FACTOR  5

struct px_sdl2_ctrl{
    SDL_Window *window;
    SDL_Surface* surface;

    /* 屏幕实际像素宽度 */
    uint16_t   screen_width;
    uint16_t   screen_height;

    /* 上次等待的时间戳 */
    uint32_t   last_run;
    
    /* 颜色 */
    uint32_t   colorBackground;
    uint32_t   colorPixel;
};

uint8_t px_user_init(struct px_gui *gui, int width, int height)
{
    struct px_sdl2_ctrl *ctrl;
    SDL_Window *window;
    SDL_Surface *surface;
    int ret;

    /* 分配控制结构 */
    ctrl = malloc(sizeof(struct px_sdl2_ctrl));
    if (NULL == ctrl){
        printf("malloc error\r\n");
        px_user_deinit(gui);
        return RET_ERROR;
    }
    memset(ctrl, 0, sizeof(*ctrl));
    gui->user = (void*)ctrl;

    //Initialize SDL2
    ret = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);         
	if (0 != ret) {
		printf("SDL_Init Error: %s\r\n", SDL_GetError());
        px_user_deinit(gui);
		return RET_ERROR;
	}

	// Create an application window with the following settings:
	ctrl->screen_width = width * PX_SDL_SCALE_FACTOR;
    ctrl->screen_height = height * PX_SDL_SCALE_FACTOR;
	window = SDL_CreateWindow("SDL2",                  // window title
                        	  SDL_WINDOWPOS_UNDEFINED,           // initial x position
                        	  SDL_WINDOWPOS_UNDEFINED,           // initial y position
                        	  ctrl->screen_width,                // width, in pixels
                        	  ctrl->screen_height,               // height, in pixels
                        	  SDL_WINDOW_OPENGL                  // flags - see below
                        	);
	if (window == NULL) {
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
        px_user_deinit(gui);
		return RET_ERROR;
	}
    ctrl->window = window;

    /* 获取表面（纯软件绘制） */
    surface = SDL_GetWindowSurface(window);
    if (NULL == surface){
        printf("Could not create surface: %s\n", SDL_GetError());
        px_user_deinit(gui);
        return RET_ERROR;
    }
    ctrl->surface = surface;
    
    ctrl->colorBackground = SDL_MapRGB(surface->format, 0x8c, 0xD2, 0xA8);
    ctrl->colorPixel      = SDL_MapRGB(surface->format, 0x3c, 0x3c, 0x3c);
    ctrl->last_run = 0;

    return RET_OK;
}

void px_user_deinit(struct px_gui *gui)
{
    struct px_sdl2_ctrl *ctrl;

    ctrl = (struct px_sdl2_ctrl *)gui->user;
    if (NULL != ctrl){
        if (NULL != ctrl->surface){
            SDL_FreeSurface(ctrl->surface);
            ctrl->surface = NULL;
        }

        if (NULL != ctrl->window){
            SDL_DestroyWindow(ctrl->window);
            ctrl->window = NULL;
        }

        free(ctrl);
        gui->user = NULL;
    }

    SDL_Quit();
    return;
}

void px_user_render(struct px_gui *gui)
{
    struct px_sdl2_ctrl *ctrl;
    uint32_t *pixel;
    int x, y, bufx, bufy;

    ctrl = (struct px_sdl2_ctrl *)gui->user;
    if (NULL == ctrl){
        return;
    }

	// Lock surface if needed
	if (SDL_MUSTLOCK(ctrl->surface))
		if (SDL_LockSurface(ctrl->surface) < 0)
			return;

    // draw
    for (y = 0; y < ctrl->screen_height; ++y){
        pixel = ((uint32_t*)ctrl->surface->pixels) + (y * (ctrl->surface->pitch / 4));
        bufy = y / PX_SDL_SCALE_FACTOR;
        for (x = 0; x < ctrl->screen_width; ++x){
			bufx = x / PX_SDL_SCALE_FACTOR;
            if (0 != gui->buf.buf[bufy][bufx] && 0 != (x%PX_SDL_SCALE_FACTOR) && 0 != (y%PX_SDL_SCALE_FACTOR)){
                pixel[x] = ctrl->colorPixel;
            } else {
                pixel[x] = ctrl->colorBackground;
            }
        }
    }
    
    // Unlock if needed
    if (SDL_MUSTLOCK(ctrl->surface)) 
      SDL_UnlockSurface(ctrl->surface);

    //update
	SDL_UpdateWindowSurface(ctrl->window);

    return;
}

void px_user_poll_event(struct px_gui *gui)
{
    SDL_Event e;
    
	while (SDL_PollEvent(&e)) {
    	if (e.type == SDL_QUIT) {
    		gui->is_run = 0;
            break;
    	} else if (e.type == SDL_MOUSEMOTION){
            gui->mouse.x = e.motion.x / PX_SDL_SCALE_FACTOR;
            gui->mouse.y = e.motion.y / PX_SDL_SCALE_FACTOR;
        } else if (e.type == SDL_MOUSEBUTTONDOWN){
            gui->mouse.is_down = 1;
            if (e.button.button == SDL_BUTTON_LEFT) {
				gui->mouse.left_button = 1;
			} else if (e.button.button == SDL_BUTTON_RIGHT){
				gui->mouse.right_button = 1;
			}
        } else if (e.type == SDL_MOUSEBUTTONUP){
            gui->mouse.is_down = 0;
			gui->mouse.left_button = 0;
			gui->mouse.right_button = 0;
        } else if (e.type == SDL_KEYDOWN) {
    	} else if (e.type == SDL_KEYUP) {
    		// If escape is pressed, return (and thus, quit)
    		if (e.key.keysym.sym == SDLK_ESCAPE) {
    			gui->is_run = 0;
    			break;
    		}
    	}
    }

    return;
}

#define TimerAfter(a,b) ((int32_t)((int32_t)(b) - (int32_t)(a)) < 0)
void px_delay_at_least(struct px_gui *gui, int ms)
{
    struct px_sdl2_ctrl *ctrl;
    uint32_t now;
	uint32_t next;
    uint32_t delay;

    ctrl = (struct px_sdl2_ctrl *)gui->user;
    if (NULL == ctrl){
        return;
    }

    /* 首次运行 */
    if (0 == ctrl->last_run){
        ctrl->last_run = SDL_GetTicks();
        return;
    }

    /* 获取下次运行时间 */
    now = SDL_GetTicks();
	next = ctrl->last_run + (uint32_t)ms;
	
	/* 如果时间已经超了就跳过延时 */
	if (TimerAfter(next, now)) {
		/* 计算延时、处理翻转 */
		if (next > now) {
			delay = next - now;
		}
		else {
			delay = next + (0xFFFFFFFF - now);
		}

		/* 延时最多100ms */
		if (delay > 100) {
			delay = 100;
			if (delay > 10 * 1000) {
				assert(0);
			}
		}
		SDL_Delay(delay);
	}

	ctrl->last_run = SDL_GetTicks();
	
    
    return;
}

void px_printf(char* fmt, ...)
{
	char buf[256];
	va_list ap;
	int len;

	memset(buf, 0, sizeof(buf));
	va_start(ap, fmt);
	len = vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

    printf(buf);
    return;
}


