/*
 * \file        tp_track.c
 * \brief       
 *
 * \version     1.0.0
 * \date        2012年06月27日
 * \author      James Deng <csjamesdeng@allwinnertech.com>
 *
 * Copyright (c) 2012 Allwinner Technology. All Rights Reserved.
 *
 */

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "drv_display_sun4i.h"
#include "dragonboard.h"
#include "script.h"
#include "df_view.h"

static int disp;
static unsigned int layer_id;
static int screen_width;
static int screen_height;
static int fb;
static unsigned char *buffer = NULL;
static __disp_layer_info_t layer_para;

static int draw_type;
static int tp_draw_color_idx;
static unsigned int tp_draw_color;

static int pre_x;
static int pre_y;

int tp_track_init(void)
{
    unsigned int args[4];
    __disp_fb_create_para_t fb_para;

    if ((disp = open("/dev/disp", O_RDWR)) == -1) {
        db_error("can't open /dev/disp(%s)\n", strerror(errno));
        return -1;
    }

    args[0] = 0;
    screen_width  = ioctl(disp, DISP_CMD_SCN_GET_WIDTH, args);
    screen_height = ioctl(disp, DISP_CMD_SCN_GET_HEIGHT, args);

#if 0
    pre_x = screen_width >> 1;
    pre_y = screen_height >> 1;
#else
    pre_x = -1;
    pre_y = -1;
#endif

    memset(&fb_para, 0, sizeof(__disp_fb_create_para_t));
    fb_para.fb_mode = FB_MODE_SCREEN0;
    fb_para.mode = DISP_LAYER_WORK_MODE_NORMAL;
    fb_para.buffer_num = 2;
    fb_para.width = screen_width;
    fb_para.height = screen_height;

    args[0] = 2;
    args[1] = (unsigned int)&fb_para;
    if (ioctl(disp, DISP_CMD_FB_REQUEST, args) < -1) {
        db_error("request framebuffer failed\n");
        return -1;
    }

    /* open /dev/fb2 and mmap */
    fb = open("/dev/fb2", O_RDWR);
    buffer = mmap(NULL, screen_width * screen_height * 4, PROT_READ|PROT_WRITE, MAP_SHARED, fb, 0);
    memset(buffer, 0, screen_width * screen_height * 4);
    ioctl(fb, FBIOGET_LAYER_HDL_0, &layer_id);

    /* set alpha value */
    args[0] = 0;
    args[1] = layer_id;
    ioctl(disp, DISP_CMD_LAYER_ALPHA_OFF, args);
    args[0] = 0;
    args[1] = layer_id;
    args[2] = 0;
    ioctl(disp, DISP_CMD_LAYER_SET_ALPHA_VALUE, args);

    /* set layer pipe1 */
    args[0] = 0;
    args[1] = layer_id;
    args[2] = 1;
    ioctl(disp, DISP_CMD_LAYER_SET_PIPE, args);

    /* set layer on top */
    args[0] = 0;
    args[1] = layer_id;
    args[2] = 1;
    ioctl(disp, DISP_CMD_LAYER_TOP, args);

    args[0] = 0;
    args[1] = layer_id;
    args[2] = (__u32)&layer_para;
    ioctl(disp, DISP_CMD_LAYER_GET_PARA, args);

    db_debug("layer para:\n");
    db_debug("mode      : %d\n", layer_para.mode);
    db_debug("pipe      : %d\n", layer_para.pipe);
    db_debug("alpha_en  : %d\n", layer_para.alpha_en);
    db_debug("alpha_val : %d\n", layer_para.alpha_val);

    /* get draw type */
    if (script_fetch("tp", "draw_type", &draw_type, 1) || 
            (draw_type != 0 && draw_type != 1)) {
        draw_type = 0;
    }

    if (script_fetch("df_view", "tp_draw_color", &tp_draw_color_idx, 1) || 
            tp_draw_color < COLOR_WHITE_IDX || 
            tp_draw_color > COLOR_BLACK_IDX) {
        tp_draw_color_idx = COLOR_WHITE_IDX;
    }
    tp_draw_color = 0xff << 24 | 
                    color_table[tp_draw_color_idx].r << 16 | 
                    color_table[tp_draw_color_idx].g << 8  |
                    color_table[tp_draw_color_idx].b;
    db_msg("tp draw color: 0x%x\n", tp_draw_color);

    return 0;
}

void tp_track_draw_pixel(int x, int y, unsigned int color)
{
    unsigned int *pixel;

    pixel = (unsigned int *)(buffer + screen_width * y * 4 + x * 4);
    *pixel = color;
}

void tp_track_draw_line(int x1, int y1, int x2, int y2, unsigned int color)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    int ux = ((dx > 0) << 1) - 1; // x的增量方向，取或-1
    int uy = ((dy > 0) << 1) - 1; // y的增量方向，取或-1
    int x = x1, y = y1, eps;      // eps为累加误差

    eps = 0;
    dx = abs(dx); 
    dy = abs(dy); 
    if (dx > dy) 
    {
        for (x = x1; x != x2; x += ux)
        {
            tp_track_draw_pixel(x, y, color);
            eps += dy;
            if ((eps << 1) >= dx)
            {
                y += uy; eps -= dx;
            }
        }
    }
    else
    {
        for (y = y1; y != y2; y += uy)
        {
            tp_track_draw_pixel(x, y, color);
            eps += dx;
            if ((eps << 1) >= dy)
            {
                x += ux; eps -= dy;
            }
        }
    }             
}

int tp_track_draw(int x, int y, int press)
{
    if (x < 0 || x > screen_width || y < 0 || y > screen_height)
        return -1;

    if (draw_type) {
        tp_track_draw_pixel(x, y, tp_draw_color);
    }
    else {
        if (press == -1) {
            if (pre_x != -1 && pre_y != -1) {
                tp_track_draw_line(pre_x, pre_y, x, y, tp_draw_color);
            }
            pre_x = pre_y = -1;
        }
        else if (press == 0) {
            pre_x = x;
            pre_y = y;
        }
        else if (press == 1) {
            if (pre_x != -1 && pre_y != -1) {
                tp_track_draw_line(pre_x, pre_y, x, y, tp_draw_color);
            }
            pre_x = x;
            pre_y = y;
        }
    }

    return 0;
}

void tp_track_start(int x, int y)
{
    pre_x = x;
    pre_y = y;
}

void tp_track_clear(void)
{
    memset(buffer, 0, screen_width * screen_height * 4);
}
