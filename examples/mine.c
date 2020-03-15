#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "px_config.h"
#include "px_common.h"
#include "px_core.h"
#include "px_widget.h"
#include "mine_widget.h"

#define MINE_MAP_WIDTH  20
#define MINE_MAP_HEIGHT 10

struct mine_ctrl{
    struct mine_widget map[MINE_MAP_HEIGHT][MINE_MAP_WIDTH];
    uint16_t map_width;
    uint16_t map_height;
    uint16_t widget_width;
    uint16_t widget_height;
};

struct mine_ctrl g_ctrl;
struct px_gui g_gui;

/* 初始化地图 */
static void _initMap(struct px_gui *gui, struct mine_ctrl *ctrl, 
                     uint16_t map_width, uint16_t map_height, 
                     uint16_t widget_width, uint16_t widget_height)
{
	int16_t x, y;
    
    ctrl->map_width = map_width;
	ctrl->map_height = map_height;
    ctrl->widget_width = widget_width + 1;
    ctrl->widget_height = widget_height + 1;

    /* 初始化控件 */
    for (y = 0; y < ctrl->map_height;  ++y){
        for (x = 0; x < ctrl->map_width; ++x){
            mine_widget_init(gui, &ctrl->map[y][x], 
                             x*ctrl->widget_width, y*ctrl->widget_height, 
                             ctrl->widget_width, ctrl->widget_height);
        }
    }


    
    return;    
}

static uint8_t _isMineOne(struct mine_ctrl *ctrl, int16_t x, int16_t y)
{
    uint8_t ret;
    if (x >= 0 && x < ctrl->map_width && y >= 0 && y < ctrl->map_height){
        if (1 == mine_widget_get_is_mine(&ctrl->map[y][x])){
            ret = 1;
        } else {
            ret = 0;
        }
    } else {
        ret = 0;
    }
    return ret;
}

/* 计算周围雷数 */
static uint8_t _countSurrounding(struct mine_ctrl *ctrl, int16_t x, int16_t y)
{
    uint8_t count;

    count = 0;
    count += _isMineOne(ctrl, x - 1, y - 1);
    count += _isMineOne(ctrl, x, y - 1);
    count += _isMineOne(ctrl, x + 1, y - 1);
    count += _isMineOne(ctrl, x - 1, y);
    count += _isMineOne(ctrl, x + 1, y);
    count += _isMineOne(ctrl, x - 1, y + 1);
    count += _isMineOne(ctrl, x, y + 1);
    count += _isMineOne(ctrl, x + 1, y + 1);

    return count;
}

static void _generageMap(struct mine_ctrl *ctrl, uint8_t difficulty)
{
	uint16_t x, y;
    int rand_num;
	uint8_t count;

	srand((unsigned)time(NULL));
    for (y = 0; y < ctrl->map_height;  ++y){
        for (x = 0; x < ctrl->map_width; ++x){
            rand_num = rand()%100;
            if (rand_num < difficulty){
                mine_widget_set_is_mine(&ctrl->map[y][x], 1);
            }
        }
    }

    /* 填写数字 */
    for (y = 0; y < ctrl->map_height;  ++y){
        for (x = 0; x < ctrl->map_width; ++x){
            count = _countSurrounding(ctrl, x, y);
            mine_widget_set_count(&ctrl->map[y][x], count);
        }
    }

    return;
}

static void _drawMap(struct mine_ctrl *ctrl)
{
	uint16_t x, y;
    
    for (y = 0; y < ctrl->map_height;  ++y){
        for (x = 0; x < ctrl->map_width; ++x){
            mine_widget_render(&ctrl->map[y][x]);
        }
    }

    return;    
}

/* 游戏结束时全部翻开 */
static void _openAll(struct mine_ctrl *ctrl)
{

}

static void _handleLeftClick(struct mine_ctrl *ctrl, int16_t x, int16_t y)
{
    struct mine_widget *ptr;

    /* 索引合法 */
    if (x >= 0 && x < ctrl->map_width && y >=0 && y < ctrl->map_height){
        ptr = &ctrl->map[y][x];
        /* 只翻开未翻开的 */
        if (MINE_STATE_CLOSE_UNMARK == mine_widget_get_state(ptr)||
            MINE_STATE_CLOSE_QUESTION == mine_widget_get_state(ptr)){
            /* 是地雷全部翻开 */
            if (1 == mine_widget_get_is_mine(ptr)){
                _openAll(ctrl);
            } 
            /* 不是地雷，翻开当前块 */
            else {
                mine_widget_set_state(ptr, MINE_STATE_OPEN_NORMAL);
                /* 周围没有地雷，翻开四周 */
                if (0 == mine_widget_get_count(ptr)){
                    _handleLeftClick(ctrl, x - 1, y - 1);
                    _handleLeftClick(ctrl, x, y - 1);
                    _handleLeftClick(ctrl, x + 1, y - 1);
                    _handleLeftClick(ctrl, x - 1, y);
                    _handleLeftClick(ctrl, x + 1, y);
                    _handleLeftClick(ctrl, x - 1, y + 1);
                    _handleLeftClick(ctrl, x, y + 1);
                    _handleLeftClick(ctrl, x + 1, y + 1);
                }
            }
        }
    }

	return;
}

static void _handleRightClick(struct mine_ctrl *ctrl, int16_t x, int16_t y)
{
    struct mine_widget *ptr;

    ptr = &ctrl->map[y][x];
    if (MINE_STATE_CLOSE_UNMARK == mine_widget_get_state(ptr)){
        mine_widget_set_state(ptr, MINE_STATE_CLOSE_EXPLOSIVE);
    } else if (MINE_STATE_CLOSE_EXPLOSIVE == mine_widget_get_state(ptr)){
        mine_widget_set_state(ptr, MINE_STATE_CLOSE_QUESTION);
    } else if (MINE_STATE_CLOSE_QUESTION == mine_widget_get_state(ptr)){
        mine_widget_set_state(ptr, MINE_STATE_CLOSE_UNMARK);
    }
    
    return;
}

static void _handleEvents(struct mine_ctrl *ctrl)
{
    struct mine_widget *ptr;
	int16_t x, y;
    uint8_t event_occur;
    
    for (y = 0; y < ctrl->map_height;  ++y){
        for (x = 0; x < ctrl->map_width; ++x){
            ptr = &ctrl->map[y][x];
            event_occur = mine_widget_event_active(ptr);
			if (1 == event_occur) {
				if (mine_widget_is_left_click(ptr)) {
					_handleLeftClick(ctrl, x, y);
				}
				else if (mine_widget_is_right_click(ptr)) {
					_handleRightClick(ctrl, x, y);
				}
			}
        }
    }
}

static void _drawMines(struct mine_ctrl *ctrl)
{
	int16_t x, y;
    
    for (y = 0; y < ctrl->map_height;  ++y){
        for (x = 0; x < ctrl->map_width; ++x){      
            mine_widget_render(&ctrl->map[y][x]);
        }
    }
    return;
}

int main(int argc, char* argv[])
{
    uint8_t ret;

    ret = px_init(&g_gui); 
    if (RET_OK != ret){
        printf("Init error\r\n");
        return -1;
    }
    
    _initMap(&g_gui, &g_ctrl, MINE_MAP_WIDTH, MINE_MAP_HEIGHT, MINE_WIDGET_WIDTH, MINE_WIDGET_HEIGHT);
    _generageMap(&g_ctrl, 20);
    
    while(1 == px_is_run(&g_gui)){

        /* event */
        px_poll_event(&g_gui);
        _handleEvents(&g_ctrl);

        /* draw */
        px_clear_buffer(&g_gui);
        _drawMines(&g_ctrl);
        px_render(&g_gui);

        /* delay */
        px_delay(&g_gui);
    }

	px_deinit(&g_gui);
    return 0;
}

