#define _GNU_SOURCE
#include <getopt.h>
#include <stdio.h>
#include "data.h"

//用于参数模式的函数声明
static void version(void);
static void help(void);
static void opt_error(char c);

//用于开始界面的函数声明


//用于参数模式的函数定义
static void version(void)
{
	fprintf(stdout,"greedy snake version %s\n",VERSION);
}
static void help(void)
{
	fprintf(stdout,"Usage: snake [options]\n");
	fprintf(stdout,"Options:\n");
	fprintf(stdout,"\t-v/--version\tdisplay the version information\n");
	fprintf(stdout,"\t-h/--help\tdisplay the help information\n");
	fprintf(stdout,"\t-i/--init\tinitialize the ranking list\n");
	fprintf(stdout,"\t-c/--cheat\tinto the cheat mode, you will not die until got full marks\n");
}
static void opt_error(char c)
{
	fprintf(stderr,"unknown option: %c\nplease use snake --help to get more information\n",c);
}
static void cheat(void)
{
	extern bool Cheat;
	Cheat=true;
}
int command_mode(int argc,char *argv[])
{
	extern bool Cheat;
	int result=0,opt;
	struct option longopts[]=
	{
		{"version",0,NULL,'v'},
		{"help",0,NULL,'h'},
		{"init",0,NULL,'i'},
		{"cheat",0,NULL,'c'},
		{0,0,0,0}
	};
	while((opt=getopt_long(argc,argv,":vhic",longopts,NULL))!=-1)
	{
		switch(opt)
		{
			case 'v':
				version();
				result=1;
				break;
			case 'h':
				help();
				result=1;
				break;
			case 'i':
				/*删除旧的得分榜数据库文件，创建新的*/
				result=0;
				break;
			case 'c':
				cheat();
				result=0;
				break;
			case '?':
				opt_error(optopt);
				result=2;
				break;
		}
	}
	return result;
}


//用于开始界面的函数定义
void draw_select_window(WINDOW *win_ptr,char *options[],int current_highlight,int start_row,int start_col)
{/*不想有高亮显示时将current_highlight设为-1*/
	int current_row=0;
	char **option_ptr;
	char *txt_ptr;
	option_ptr=options;
	while(*option_ptr)
	{
		if(current_row==current_highlight)
			wattron(win_ptr,A_STANDOUT);
		txt_ptr=options[current_row];
		mvwprintw(win_ptr,start_row+current_row*2,start_col,"%s",txt_ptr);
		if(current_row==current_highlight)
			wattroff(win_ptr,A_STANDOUT);
		current_row++;
		option_ptr++;
	}
	wrefresh(win_ptr);
}
void clear_start_screen(void)
{
	clear();
	mvprintw(1,COLS/2-7,"%s","Greedy Snake");
	refresh();
}
int getchoice(WINDOW *win_ptr,char *choices[])
{
	static int selected_row=0;
	int max_row=0;
	int start_screenrow=WINDOW_HEIGHT/2-5,start_screencol=WINDOW_WIDTH/2-6;
	char **options;
	int selected;
	int key=0;
	options=choices;
	mvprintw(LINES-2,1,"Move highlight then press enter");
	refresh();
	while(*options)
	{
		max_row++;
		options++;
	}
	keypad(stdscr,true);
	cbreak();
	noecho();
	while(key!=KEY_ENTER&&key!='\n')
	{
		if(key==KEY_UP)
		{
			if(selected_row==0)
				selected_row=max_row-1;
			else
				selected_row--;
		}
		if(key==KEY_DOWN)
		{
			if(selected_row==max_row-1)
				selected_row=0;
			else
				selected_row++;
		}
		selected=*choices[selected_row];
		draw_select_window(win_ptr,choices,selected_row,start_screenrow,start_screencol);
		key=getch();
	}
	keypad(stdscr,false);
	nocbreak();
	echo();
	return selected;
}


//用于游戏界面的函数定义
void draw_snake_window(WINDOW *win_ptr,snake greedy,food f1)
{
	extern int Current_len;
	wclear(win_ptr);
	box(win_ptr,ACS_VLINE,ACS_HLINE);
	mvwaddch(win_ptr,f1.y,f1.x,'@');
	for(int i=0;i<Current_len;i++)
	{
		if(i==0)
			mvwaddch(win_ptr,greedy[i].y,greedy[i].x,'#');
		else
			mvwaddch(win_ptr,greedy[i].y,greedy[i].x,'*');
	}
	wrefresh(win_ptr);
}
void draw_status_window(WINDOW *win_ptr,double speed)
{
	extern int Current_len;
	int score=Current_len;
	char speed_string[38];
	char score_string[38];
	sprintf(score_string,"Current Score = %d",score);
	sprintf(speed_string,"Current Speed = %10.5lf",speed);
	wclear(win_ptr);
	box(win_ptr,ACS_VLINE,ACS_HLINE);
	mvwprintw(win_ptr,WINDOW_HEIGHT/6,WINDOW_WIDTH/3,"%s",score_string);
	mvwprintw(win_ptr,WINDOW_HEIGHT/3,WINDOW_WIDTH/3,"%s",speed_string);
	wrefresh(win_ptr);
}
void update_snake(snake greedy,direct d,bool *eated)
{
	extern bool Map[WINDOW_HEIGHT-2][WINDOW_WIDTH-2];
	extern int Current_len;
	int i;
	if(*eated)
	{
		Current_len++;
		*eated=false;
	}
	node temp;
	temp=greedy[0];
	temp.x+=d.x;
	temp.y+=d.y;
	for(i=Current_len-1;i>0;i--)
	{
		greedy[i]=greedy[i-1];
	}
	greedy[0]=temp;

	for(i=0;i<Current_len;i++)
	{
		Map[greedy[i].y-1][greedy[i].x-1]=false;
	}
}
void init_keyboard(void)
{
	keypad(stdscr,true);
	noecho();
	cbreak();
	timeout(1000);
}
void get_key(direct *d)
{
	int key;
	if((key=getch())!=ERR)
		{
			switch(key)
			{
				case 'A':
				case 'a':
				case KEY_LEFT:
					if(d->x!=1)
					{
						d->x=-1;
						d->y=0;
					}
					break;
				case 'D':
				case 'd':
				case KEY_RIGHT:
					if(d->x!=-1)
					{
						d->x=1;
						d->y=0;
					}
					break;
				case 'W':
				case 'w':
				case KEY_UP:
					if(d->y!=1)
					{
						d->x=0;
						d->y=-1;
					}
					break;
				case 'S':
				case 's':
				case KEY_DOWN:
					if(d->y!=-1)
					{
						d->x=0;
						d->y=1;
					}
					break;
			}
		}
}
void close_keyboard(void)
{
	keypad(stdscr,false);
	echo();
	timeout(-1);
	nocbreak();
}
bool Eatfood(snake greedy,food f1)
{
	if(greedy[0].x==f1.x&&greedy[0].y==f1.y)
		return true;
	else
		return false;
}
bool Isover(snake greedy)
{
	extern int Current_len;
	bool flag=false;
	if(greedy[0].x==0||greedy[0].x==(COLS-1)||greedy[0].y==0||greedy[0].y==(LINES-1))
		flag=true;
	for(int i=1;i<Current_len;i++)
	{
		if(greedy[0].x==greedy[i].x&&greedy[0].y==greedy[i].y)
			flag=true;
	}
	return flag;
}
bool Iswin(void)
{
	extern int Current_len;
	if(Current_len==TOTLE_POINT)
		return true;
	else
		return false;
}
void Createfood(food *fd)
{
	int index_x=0,index_y=0;
	extern bool Map[WINDOW_HEIGHT-2][WINDOW_WIDTH-2];
	extern int Current_len;
	int residue=TOTLE_POINT-Current_len;
	int count=0;
	count=rand()%residue+1;
	while(count!=0)
	{
		if(Map[index_y][index_x])
			count--;
		if(index_x==WINDOW_WIDTH-3)
		{
			index_y++;
			index_x=0;
		}
		else
			index_x++;
	}
	fd->x=index_x+1;
	fd->y=index_y+1;
}
