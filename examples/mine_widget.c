#include <stdint.h>
#include <string.h>

#include "px_common.h"
#include "px_config.h"
#include "px_core.h"
#include "px_widget.h"
#include "px_digit.h"
#include "mine_widget.h"

static uint8_t g_bitmap[MINE_STATE_MAX_SIZE][MINE_WIDGET_HEIGHT][MINE_WIDGET_WIDTH] = {
    /* MINE_STATE_CLOSE_UNMARK */
    {
        {0 ,0 ,0 ,0 ,0},
        {0 ,0 ,0 ,0 ,0},
        {0 ,0 ,1 ,0 ,0},
        {0 ,0 ,0 ,0 ,0},
        {0 ,0 ,0 ,0 ,0},
    },
    /* MINE_STATE_CLOSE_EXPLOSIVE */
    {
        {0 ,0 ,0 ,0 ,0},
        {0 ,1 ,0 ,1 ,0},
        {0 ,0 ,1 ,0 ,0},
        {0 ,1 ,0 ,1 ,0},
        {0 ,0 ,0 ,0 ,0},
    },    
    /* MINE_STATE_CLOSE_QUESTION */
    {
        {0 ,0 ,1 ,0 ,0},
        {0 ,1 ,0 ,1 ,0},
        {0 ,0 ,0 ,1 ,0},
        {0 ,0 ,1 ,0 ,0},
        {0 ,0 ,1 ,0 ,0},
    },
    /* MINE_STATE_OPEN_NORMAL */
    {
        {0 ,0 ,0 ,0 ,0},
        {0 ,0 ,0 ,0 ,0},
        {0 ,0 ,0 ,0 ,0},
        {0 ,0 ,0 ,0 ,0},
        {0 ,0 ,0 ,0 ,0},
    },
    /* MINE_STATE_OPEN_EXPLOSIVE */
    {
        {1 ,0 ,1 ,0 ,1},
        {0 ,1 ,1 ,1 ,0},
        {1 ,1 ,1 ,1 ,1},
        {0 ,1 ,1 ,1 ,0},
        {1 ,0 ,1 ,0 ,1},
    }
};


void mine_widget_init(struct px_gui *gui, 
                      struct mine_widget *widget, int16_t x, int16_t y, int16_t width, int16_t height)
{   
    px_widget_init(gui, &widget->widget, x, y, width, height);

    if (height > MINE_WIDGET_HEIGHT){
        widget->top = (height - MINE_WIDGET_HEIGHT) / 2;
    } else {
        widget->top = 0;
    }

    if (width > MINE_WIDGET_WIDTH){
        widget->left = (width - MINE_WIDGET_WIDTH) / 2;
    } else {
        widget->left = 0;
    }

    return;
}

void mine_widget_render(struct mine_widget *widget)
{
    struct px_bitmap bitmap;

    if (widget->state < MINE_STATE_MAX_SIZE){
        /* 翻开不等于0的时候填数字，其他情况填图案 */
        if (MINE_STATE_OPEN_NORMAL == mine_widget_get_state(widget) && 0 != mine_widget_get_count(widget)){
            bitmap.ptr = px_digit_get(PX_DIGIT_SIZE_5x3, mine_widget_get_count(widget));
            ASSERT(NULL != bitmap.ptr);
            bitmap.width = 3;
            bitmap.height = 5;
            px_widget_render_bitmap(&widget->widget, &bitmap, widget->left + 1, widget->top);
        } else {
            bitmap.ptr = (uint8_t **)g_bitmap[widget->state];
            bitmap.width = MINE_WIDGET_WIDTH;
            bitmap.height = MINE_WIDGET_HEIGHT;
            px_widget_render_bitmap(&widget->widget, &bitmap, widget->left, widget->top);
        }
    }

    return;
}

uint8_t mine_widget_event_left_click(struct mine_widget *widget)
{
	return px_widget_event_active(&widget->widget) && px_widget_is_left_button(&widget->widget);
}

uint8_t mine_widget_event_right_click(struct mine_widget *widget)
{
	return px_widget_event_active(&widget->widget) && px_widget_is_right_button(&widget->widget);
}

