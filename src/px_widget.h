#ifndef _PX_WIDGET_H_
#define _PX_WIDGET_H_

struct px_rect{
    int16_t start_x;
    int16_t start_y;
    int16_t end_x;
    int16_t end_y;
};

struct px_bitmap{
    uint8_t **ptr;
    int16_t width;
    int16_t height;
};

struct px_widget
{
    struct px_gui *gui;

    /* ±ß¿ò³ß´ç */
    struct px_rect rect;
};

SYMBOL_EXPORT void px_widget_init(struct px_gui *gui, struct px_widget *widget, int16_t x, int16_t y, int16_t width, int16_t height);
SYMBOL_EXPORT void px_widget_render_bitmap(struct px_widget *widget, const struct px_bitmap *bitmap, int16_t left, int16_t top);
SYMBOL_EXPORT void px_widget_render_borader(struct px_widget *widget, int width, uint8_t gray_scale);
SYMBOL_EXPORT void px_widget_render_fill(struct px_widget *widget, uint8_t gray_scale);
SYMBOL_EXPORT int px_widget_is_hot(struct px_widget *widget);
SYMBOL_EXPORT int px_widget_is_active(struct px_widget *widget);

#endif

