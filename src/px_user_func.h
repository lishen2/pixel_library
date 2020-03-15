#ifndef _PX_USER_FUNC_H_
#define _PX_USER_FUNC_H_

uint8_t px_user_init(struct px_gui *gui, int width, int height);
void px_user_deinit(struct px_gui *gui);

void px_user_render(struct px_gui *gui);
void px_user_poll_event(struct px_gui *gui);

void px_delay_at_least(struct px_gui *gui, int ms);

#endif

