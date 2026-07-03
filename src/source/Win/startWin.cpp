#include <graphics.h>
#include <conio.h>
#include "../../header/Win/startWin.h"
#include "../../header/Win/loginWin.h"
extern char timestr[];
int startWin(){
	WINDOW_T startWin = {250, 180, 300, 220, WHITE, 4,
        {
        	{460, 180, 200, 50, "", CYAN, LIGHTCYAN, BLACK, LABEL, 0},
            {270, 220, 200, 50, "µÇ    Â¼", CYAN, LIGHTCYAN, BLACK, BUTTON, 1},
            {270, 280, 200, 50, "Íü¼ÇÃÜÂë", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
            {270, 340, 200, 50, "ÍË    ³ö", CYAN, LIGHTCYAN, BLACK, BUTTON, 0}
        }
    };
    strcpy(startWin.controls[0].text, timestr);
    window_show(startWin);
	startWin = window_run(startWin);
	if(startWin.current == 1){
		return 1;
	}
	else if(startWin.current == 2){
		return 1;
	}
	else if(startWin.current == 3){
		return -1; 
	}
}

