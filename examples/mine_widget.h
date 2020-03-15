#ifndef _MINE_WIDGET_H_
#define _MINE_WIDGET_H_

#define MINE_WIDGET_WIDTH 5
#define MINE_WIDGET_HEIGHT 5

enum mine_state{
    MINE_STATE_CLOSE_UNMARK = 0,    /* 未翻开，未标记 */  
    MINE_STATE_CLOSE_EXPLOSIVE,     /* 未翻开，标记炸弹 */
    MINE_STATE_CLOSE_QUESTION,      /* 未翻开，标记可疑 */      
    MINE_STATE_OPEN_NORMAL,         /* 已翻开，正常 */
    MINE_STATE_OPEN_EXPLOSIVE,      /* 已翻开，炸弹 */
    MINE_STATE_MAX_SIZE,
};

struct mine_widget
{
    struct px_widget widget;

    uint8_t state;  /* 当前方块的状态 */
    uint8_t count;  /* 周围雷计数 */
    uint8_t is_mine;/* 是否是雷 */

    /* 图片边距 */
    int16_t top;
    int16_t left;
};

void mine_widget_init(struct px_gui *gui, 
                      struct mine_widget *widget, int16_t x, int16_t y, int16_t width, int16_t height);

#define mine_widget_set_state(widget, _state) (widget)->state = _state
#define mine_widget_get_state(widget) (widget)->state

#define mine_widget_set_is_mine(widget, _is_mine) (widget)->is_mine = _is_mine
#define mine_widget_get_is_mine(widget) (widget)->is_mine

#define mine_widget_set_count(widget, _count) (widget)->count = _count
#define mine_widget_get_count(widget) (widget)->count

void mine_widget_render(struct mine_widget *widget);

#define mine_widget_event_active(_widget) px_widget_event_active(&_widget->widget)

#define mine_widget_is_left_click(_widget) px_widget_is_left_button(&_widget->widget)
#define mine_widget_is_right_click(_widget) px_widget_is_right_button(&_widget->widget)

#endif

