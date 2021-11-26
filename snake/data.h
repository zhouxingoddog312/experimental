#ifndef _DATA_H
#define _DATA_H

#include <unistd.h>
#include <curses.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ndbm.h>
#include <getopt.h>
#include <stdio.h>
#define WINDOW_WIDTH 40
#define WINDOW_HEIGHT 20
#define TOTLE_POINT ((WINDOW_WIDTH-2)*(WINDOW_HEIGHT-2))
#define SPEED_MAX 1000
#define STR_LEN 38

#define VERSION ("1.00")


typedef struct
{
	int x;
	int y;
}node;
typedef node food;
typedef node direct;
typedef node *snake;

//存档数据和得分榜数据处理函数



//游戏逻辑函数
void init_status(WINDOW *win_ptr,direct *d_ptr,food *f_ptr,snake greedy,char *name);
void destory_status(snake greedy);
void end_game(WINDOW *win_ptr,char *string);
//用于参数模式的函数
int command_mode(int argc,char *argv[]);

//用于开始界面的函数
void draw_base_window(void);
void draw_select_window(WINDOW *win_ptr,char *options[],int current_highlight,int start_row,int start_col);
void clear_start_screen(void);
int getchoice(WINDOW *win_ptr,char *choices[]);

//用于游戏界面的函数
void Checkmap(snake greedy);
void draw_snake_window(WINDOW *win_ptr,snake greedy,food f1);
void draw_status_window(WINDOW *win_ptr,char *name);
void update_snake(snake greedy,direct d,bool *eated);
void init_keyboard(WINDOW *w_ptr);
void get_key(direct *d);
void close_keyboard(WINDOW *w_ptr);
bool Eatfood(snake greedy,food f1);
bool Isover(snake greedy);
bool Iswin(void);
void Createfood(food *fd);
#endif
