#include <stdint.h>
#include <string.h>

#include "px_common.h"
#include "px_config.h"
#include "px_core.h"
#include "px_widget.h"
#include "mine_widget.h"

#define MINE_WIDGET_BORDER_WIDTH 1

void mine_widget_init(struct px_gui *gui, 
                      struct mine_widget *widget, int16_t x, int16_t y, int16_t width, int16_t height,
                      struct px_bitmap *close_unmark, struct px_bitmap *close_question, struct px_bitmap *close_explosive, 
                      struct px_bitmap *open_normal, struct px_bitmap *open_explosive)
{   
    px_widget_init(gui, &widget->widget, x, y, width, height);
    widget->close_unmark = close_unmark;
    widget->close_question = close_question;
    widget->close_explosive = close_explosive;
    widget->open_normal = open_normal;
    widget->open_explosive = open_explosive;

    if (height > close_unmark->height){
        widget->top = (height - close_unmark->height) / 2;
    } else {
        widget->top = MINE_WIDGET_BORDER_WIDTH;
    }

    if (width > close_unmark->width){
        widget->left = (width - close_unmark->width) / 2;
    } else {
        widget->left = MINE_WIDGET_BORDER_WIDTH;
    }

    return;
}

void mine_widget_render(struct mine_widget *widget)
{
    px_widget_render_borader(&widget->widget, MINE_WIDGET_BORDER_WIDTH, 0xFF);

    switch(widget->state){
        case MINE_STATE_CLOSE_UNMARK:
            px_widget_render_bitmap(&widget->widget, widget->close_unmark, widget->left, widget->top);
            break;  
        case MINE_STATE_CLOSE_QUESTION:
            px_widget_render_bitmap(&widget->widget, widget->close_question, widget->left, widget->top);            
            break;   
        case MINE_STATE_CLOSE_EXPLOSIVE:
            px_widget_render_bitmap(&widget->widget, widget->close_explosive, widget->left, widget->top);            
            break;
        case MINE_STATE_OPEN_NORMAL:
            px_widget_render_bitmap(&widget->widget, widget->open_normal, widget->left, widget->top);            
            break;   
        case MINE_STATE_OPEN_EXPLOSIVE:
            px_widget_render_bitmap(&widget->widget, widget->open_explosive, widget->left, widget->top);            
            break;
    };

    return;
}

