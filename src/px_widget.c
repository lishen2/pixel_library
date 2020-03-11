#include <stdint.h>
#include <string.h>

#include "px_common.h"
#include "px_config.h"
#include "px_core.h"
#include "px_widget.h"

void px_widget_init(struct px_gui *gui, struct px_widget *widget, int16_t x, int16_t y, int16_t width, int16_t height)
{
    struct px_rect *prect;

    prect = &widget->rect;
    prect->start_x = x;
    prect->start_y = y;
    prect->end_x = x + width;
    if (prect->end_x > gui->buf.width){
        prect->end_x = gui->buf.width;
    }
	prect->end_y = y + height;
    if (prect->end_y > gui->buf.height){
        prect->end_y = gui->buf.height;
    }

    widget->gui = gui;

    return;
}

void px_widget_render_bitmap(struct px_widget *widget, const struct px_bitmap *bitmap, int16_t left, int16_t top)
{
    struct px_rect img_rect;
    struct px_rect *prect;
    int16_t offx, offy;
    int16_t x, y;
    uint8_t *ptr;    

    prect = &widget->rect;

    /* 计算图片边框 */
    img_rect.start_x = prect->start_x + left;
    if (img_rect.start_x > prect->end_x){
        img_rect.start_x = prect->end_x;
    }
    img_rect.start_y = prect->start_y + top;
    if (img_rect.start_y > prect->end_y){
        img_rect.start_y = prect->end_y;
    }
    
    img_rect.end_x = img_rect.start_x + bitmap->width;
    if (img_rect.end_x > prect->end_x){
        img_rect.end_x = prect->end_x;
    }
    img_rect.end_y = img_rect.start_y + bitmap->height;
    if (img_rect.end_y > prect->end_y){
        img_rect.end_y = prect->end_y;
    }

    /* 绘制 */
    ptr = (uint8_t *)bitmap->ptr;
    for (y = img_rect.start_y, offy = 0; y < img_rect.end_y; ++y, ++offy){
        for (x = img_rect.start_x, offx = 0; x < img_rect.end_x; ++x, ++offx){
            widget->gui->buf.buf[y][x] = ptr[offx];
        }
        ptr += offy * bitmap->width;
    }

    return;
}

void px_widget_render_borader(struct px_widget *widget, int width, uint8_t gray_scale)
{
    struct px_rect inner_rect;
    struct px_rect *prect;
    int x, y;

    /* 计算内部没有边框的区域 */
    prect = &widget->rect;
    inner_rect.start_x = widget->rect.start_x + width;
    inner_rect.start_y = widget->rect.start_y + width;
    inner_rect.end_x = widget->rect.end_x - width;
    inner_rect.end_y = widget->rect.end_y - width;    

    /* 如果边框占满整个区域，直接填充 */
    if (inner_rect.end_x <= inner_rect.start_x ||
        inner_rect.end_y <= inner_rect.start_y){
        px_widget_render_fill(widget, gray_scale);
    } 
    /* 否则画四周 */
    else {
        for (y = widget->rect.start_y; y < widget->rect.end_y; ++y){
            for (x = widget->rect.start_x; x < widget->rect.end_x; ++x){
                if (x < inner_rect.start_x || x >= inner_rect.end_x ||
                    y < inner_rect.start_y || y >= inner_rect.end_y){
                    widget->gui->buf.buf[y][x] = gray_scale;
                }
            }
        }
    }

    return;
}

void px_widget_render_fill(struct px_widget *widget, uint8_t gray_scale)
{
    int x, y;

    //bg color
    for (y = widget->rect.start_y; y < widget->rect.end_y; ++y){
        for (x = widget->rect.start_x; x < widget->rect.end_x; ++x){
            widget->gui->buf.buf[y][x] = gray_scale;
        }
    }

    return;
}

int px_widget_is_hot(struct px_widget *widget)
{
    struct px_mouse *mouse;
    struct px_rect *prect;
    
    mouse = &widget->gui->mouse;
    prect = &widget->rect;
    if (mouse->x >= prect->start_x && mouse->x < prect->end_x &&
        mouse->y >= prect->start_y && mouse->y < prect->end_y){
        return 1;
    } else {
        return 0;
    }
}

int px_widget_is_active(struct px_widget *widget)
{
    struct px_mouse *mouse;
    
    int ret;

    mouse = &widget->gui->mouse;
    if (1 == mouse->is_down){
        ret = px_widget_is_hot(widget);
    } else {
        ret = 0;
    }

    return ret;
}

