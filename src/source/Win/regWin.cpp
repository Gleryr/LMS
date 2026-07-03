#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include "../../header/Win/regWin.h"


int regWin(){
	
	WINDOW_T regWin = {
        255, 180, 290, 300, LIGHTGRAY, 8,
        {
            {265, 210, 70, 35, "用 户 名:", WHITE, LIGHTCYAN, BLACK, LABEL, 0},
            {265, 255, 70, 35, "密    码:", WHITE, LIGHTCYAN, BLACK, LABEL, 0},
            {265, 300, 70, 35, "确认密码:", WHITE, LIGHTCYAN, BLACK, LABEL, 0},
            {340, 210, 170, 35, "", WHITE, LIGHTCYAN, BLACK, EDIT, 1},
            {340, 255, 170, 35, "", WHITE, LIGHTCYAN, BLACK, EDIT_PWD, 0},
            {340, 300, 170, 35, "", WHITE, LIGHTCYAN, BLACK, EDIT_PWD, 0},
            {270, 350, 100, 40, "注册", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
            {380, 350, 100, 40, "返回", CYAN, LIGHTCYAN, BLACK, BUTTON, 0}
        }
    };
	
	window_show(regWin);
	regWin = window_run(regWin);
	
	
	if(regWin.current == 6){
		return 1;
	}
	else if(regWin.current == 7){
		return 0;
	}
}
