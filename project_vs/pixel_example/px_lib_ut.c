#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include "px_config.h"
#include "px_common.h"
#include "px_core.h"
#include "px_widget.h"
#include "px_user_func.h"
#include "SDL2/SDL.h"

#define TEST_ASSERT(exp, ...)\
    if (!(exp)){\
        printf("TEST FAULT, %s:%d | ", __FILE__, __LINE__);\
        printf(__VA_ARGS__);\
        printf("\r\n");\
    }

static void _basic_widget(void)
{
    struct px_gui gui;
    struct px_widget widget;
    int ret;
    int lasthot;
    int lastactive;

    ret = px_init(&gui);
    TEST_ASSERT(0 == ret, "");    

    px_widget_init(&gui, &widget, 50, 30, 10, 5);

    lasthot = 0;
    lastactive = 0;
    while(1 == px_is_run(&gui)){
 
        px_poll_event(&gui);

        ret = px_widget_is_hot(&widget);
        if (lasthot != ret){
            if (1 == ret){
                printf("Widget hot\r\n");
            } else {
                printf("Widget not hot\r\n");
            }
            lasthot = ret;
        }

        ret = px_widget_is_active(&widget);
        if (lastactive != ret){
            if (1 == ret){
                printf("Widget active\r\n");
            } else {
                printf("Widget not active\r\n");
            }
            lastactive = ret;
        }

        px_clear_buffer(&gui);
		px_widget_render_fill(&widget, 0xFF);
		//px_widget_render_borader(&widget, 1, 0xFF);
        px_render(&gui);

        px_delay(&gui);
    }

	px_deinit(&gui);
}

int main(int argc, char* argv[])
{
    //_basic_render();
    _basic_widget();

    return 0;
}

