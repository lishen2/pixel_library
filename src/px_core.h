#ifndef _PX_API_H_
#define _PX_API_H_

struct px_framebuffer{
    uint8_t buf[PX_SCREEN_HEIGHT][PX_SCREEN_WIDTH];
    int16_t width;
    int16_t height;
};

struct px_mouse{
    int16_t x;
    int16_t y;
    uint32_t is_down:1;
    uint32_t left_button:1;
    uint32_t right_button:1;
};

struct px_gui{
    uint32_t is_run:1;

    struct px_framebuffer buf;
    struct px_mouse mouse;

    /* 用户数据区域 */
    void *user;
};

SYMBOL_EXPORT uint8_t px_init(struct px_gui *gui);

SYMBOL_EXPORT void px_deinit(struct px_gui *gui);

SYMBOL_EXPORT void px_poll_event(struct px_gui *gui);

SYMBOL_EXPORT void px_clear_buffer(struct px_gui *gui);

SYMBOL_EXPORT void px_render(struct px_gui *gui);

SYMBOL_EXPORT uint8_t px_is_run(struct px_gui *gui);

SYMBOL_EXPORT void px_delay(struct px_gui *gui);

SYMBOL_EXPORT void px_printf(char* fmt, ...);

#endif

