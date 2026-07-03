#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include "../../header/Win/loginWin.h"

extern User_t *g_user;
char username[10] = {0};
int loginWin(){
	WINDOW_T loginWin = { 255, 180, 290, 240, WHITE, 6,
	    {
	        {265, 210, 70, 35, "用户名:", WHITE, LIGHTCYAN, BLACK, LABEL, 0},
	        {265, 255, 70, 35, "密  码:", WHITE, LIGHTCYAN, BLACK, LABEL, 0},
	        {340, 210, 170, 35, "", WHITE, LIGHTCYAN, BLACK, EDIT, 1},
	        {340, 255, 170, 35, "", WHITE, LIGHTCYAN, BLACK, EDIT_PWD, 0},
	        {270, 305, 100, 40, "登录", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
	        {380, 305, 100, 40, "返回", CYAN, LIGHTCYAN, BLACK, BUTTON, 0}
	    }
	};
	window_show(loginWin);
	loginWin = window_run(loginWin);
	if(loginWin.current == 4){
		if(strlen(loginWin.controls[2].text) == 0){
			showPopup(300, 220, 240, 80, "用户名不能为空!");
			Sleep(2000);
			return 1; 
		} 
		else if(strlen(loginWin.controls[3].text) == 0){
			showPopup(300, 220, 240, 80, "密码不能为空!");
			Sleep(2000);
			return 1; 
		}  
		int re = login(loginWin.controls[2].text, loginWin.controls[3].text);
		if(re == 1){
			showPopup(300, 220, 240, 80, "登录成功");
			Sleep(2000);
			strcpy(username, loginWin.controls[2].text);
			return 3;
		}
		else if(re == 0){
			showPopup(300, 220, 240, 80, "用户名输入错误");
			Sleep(2000);
			return 1;
		}
		else if(re == -1){
			showPopup(300, 220, 240, 80, "账号不可用");
			Sleep(2000);
			return 1;
		}
		else if(re == -2){
			showPopup(300, 220, 240, 80, "密码错误");
			Sleep(2000);
			return 1;
		}
	}
	else if(loginWin.current == 5){
		return 0;
	}
}
