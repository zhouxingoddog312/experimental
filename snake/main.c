#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <curses.h>

#include "data.h"

bool Cheat=false;
int Current_len=2;
bool Map[WINDOW_HEIGHT-2][WINDOW_WIDTH-2]={{false}};
int main(int argc,char *argv[])
{
	int command_result;
	if(argc>1)
	{
		command_result=command_mode(argc,argv);
		if(!command_result)
			exit(command_result);
	}
	exit(0);
}
