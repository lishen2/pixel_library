#ifndef _MINE_WIDGET_H_
#define _MINE_WIDGET_H_

enum mine_state{
    MINE_STATE_CLOSE_UNMARK,    /* δ������δ��� */
    MINE_STATE_CLOSE_QUESTION,  /* δ��������ǿ��� */    
    MINE_STATE_CLOSE_EXPLOSIVE, /* δ���������ը�� */
    MINE_STATE_OPEN_NORMAL,     /* �ѷ��������� */
    MINE_STATE_OPEN_EXPLOSIVE,  /* �ѷ�����ը�� */
};

struct mine_widget
{
    struct px_widget widget;

    uint8_t state;
    uint8_t is_mine;

    /* ͼƬ�߾� */
    int16_t top;
    int16_t left;

    const struct px_bitmap *close_unmark;
    const struct px_bitmap *close_question;
    const struct px_bitmap *close_explosive;
    const struct px_bitmap *open_normal;
    const struct px_bitmap *open_explosive;
};

void mine_widget_init(struct px_gui *gui, 
                      struct mine_widget *widget, int16_t x, int16_t y, int16_t width, int16_t height,
                      struct px_bitmap *close_unmark, struct px_bitmap *close_question, struct px_bitmap *close_explosive, 
                      struct px_bitmap *open_normal, struct px_bitmap *open_explosive)

#define mine_widget_set_state(widget, state) (widget)->state = state
#define mine_widget_get_state(widget) (widget)->state

#define mine_widget_set_is_mine(widget, is_mine) (widget)->is_mine = is_mine
#define mine_widget_get_is_mine(widget) (widget)->is_mine

void mine_widget_render(struct mine_widget *widget);

#endif

