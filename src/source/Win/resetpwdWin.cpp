#include "../../header/resetuser.h"
#include <graphics.h>
#include <conio.h>
#include <stdio.h>

int resetpwdWin(){
	WINDOW_T resetpwdWin = {255, 180, 290, 240, WHITE, 4,
        {
            {265, 210, 70, 35, "用户名", CYAN, LIGHTCYAN, BLACK, LABEL, 0},
            {340, 210, 170, 35, "", CYAN, LIGHTCYAN, BLACK, EDIT, 1},
            {265, 305, 100, 40, "确认", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
            {380, 305, 100, 40, "退出", CYAN, LIGHTCYAN, BLACK, BUTTON, 0}
        }
    };
    
    window_show(resetpwdWin);
	resetpwdWin = window_run(resetpwdWin);
	if(resetpwdWin.current == 2){
		if(strlen(resetpwdWin.controls[1].text) == 0){
			showPopup(300, 220, 240, 80, "请输入用户名");
			Sleep(2000);
			return 6;                 //!
		}
		else{
			int i = resetpwd(resetpwdWin.controls[1].text);
			if(i == 1){
				return 4;                   //!
			}
			else if(i == -1){
				return 6;                 //!
			}
		}
	}
	else if(resetpwdWin.current == 3){
		return 4;
	}
	
	
}
