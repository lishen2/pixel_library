#include <stdint.h>
#include <string.h>

#include "px_config.h"
#include "px_common.h"
#include "px_core.h"
#include "px_user_func.h"

int px_init(struct px_gui *gui)
{
    memset(gui, 0, sizeof(*gui));
    gui->is_run = 1;
	gui->buf.width = PX_SCREEN_WIDTH;
	gui->buf.height = PX_SCREEN_HEIGHT;

    return px_user_init(gui, PX_SCREEN_WIDTH, PX_SCREEN_HEIGHT);
}

void px_deinit(struct px_gui *gui)
{
    px_user_deinit(gui);
}

void px_poll_event(struct px_gui *gui)
{
    px_user_poll_event(gui);
}

void px_render(struct px_gui *gui)
{
    px_user_render(gui);
}

void px_clear_buffer(struct px_gui *gui)
{
    memset(gui->buf.buf, 0, sizeof(gui->buf.buf));
    return;
}

int px_is_run(struct px_gui *gui)
{
    return gui->is_run;
}

#define PX_CORE_DELAY_TIME (1000UL/PX_FRAME_RATE)
void px_delay(struct px_gui *gui)
{
    px_delay_at_least(gui, PX_CORE_DELAY_TIME);
    return;
}

