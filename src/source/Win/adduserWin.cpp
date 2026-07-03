
#include "../../header/Win/adduserWin.h"
#include "../../header/user.h"
extern LNode *userList; 
int adduserWin(){
	WINDOW_T adduserWin = {200, 150, 400, 350,  WHITE, 8,
	    {
	        {210, 180, 220, 50, "用户名:", CYAN, LIGHTGRAY, BLACK, LABEL, 0},
	        {280, 180, 220, 50, "", CYAN, LIGHTCYAN, BLACK, EDIT, 1},
	        {210, 240, 220, 50, "密  码:", CYAN, LIGHTGRAY, BLACK, LABEL, 0},
	        {280, 240, 220, 50, "", CYAN, LIGHTCYAN, BLACK, EDIT_PWD, 0},
	        {210, 300, 220, 50, "角  色:", CYAN, LIGHTGRAY, BLACK, LABEL, 0},
	        {280, 300, 220, 50, "请选择数据,管理员,仓储员,调度员,客服,司机", CYAN, LIGHTCYAN, BLACK, SELECT, 0},
	        {210, 360, 150, 50, "创  建", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
	        {370, 360, 150, 50, "返  回", CYAN, LIGHTCYAN, BLACK, BUTTON, 0}
	    }
	};
	
	window_show(adduserWin);
	while(kbhit()){
	    getch();
	}
	adduserWin = window_run(adduserWin);
 	if(adduserWin.current == 6){
 		int role = -1; 
 		char selectedRole[50] = {0};
        sscanf(adduserWin.controls[5].text, "%[^,]", selectedRole); 

        if(strcmp(selectedRole, "管理员") == 0){
            role = 0;
        }
        else if(strcmp(selectedRole, "仓储员") == 0){
            role = 1;
        }
        else if(strcmp(selectedRole, "调度员") == 0){
            role = 2;
        }
        else if(strcmp(selectedRole, "客服") == 0){
            role = 3;
        }
        else if(strcmp(selectedRole, "司机") == 0){
            role = 4;
        }
		printf("%s", adduserWin.controls[1].text); 
 		regUser(adduserWin.controls[1].text, adduserWin.controls[3].text, role, userList);
 		
		return 4;
	}
	else if(adduserWin.current == 7){
		return 4;
	}
}
