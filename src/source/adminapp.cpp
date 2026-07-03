#include "../header/Win/adminapp.h"
#include "../header/Win/analysiswin.h" 
int adminapp(){
	WINDOW_T adminapp = {100, 80, 600, 500, WHITE, 9,
        {
            {120, 220, 220, 50, "1. 系统管理", CYAN, LIGHTCYAN, BLACK, BUTTON, 1},
            {380, 220, 220, 50, "2. 订单管理", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
            {120, 290, 220, 50, "3. 存储管理", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
            {380, 290, 220, 50, "4. 运输管理", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
            {120, 360, 220, 50, "5. 统计分析", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
            {380, 360, 220, 50, "6. 客户服务", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
            {120, 430, 220, 50, "7. 密码修改", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
            {380, 430, 220, 50, "8. 退    出", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
            {120, 510, 500, 30, "操作说明：通过上下键切换菜单，按Enter键进入", BLACK, LIGHTGRAY, BLACK, LABEL, 0}
        }
	};

	
	window_show(adminapp);
	adminapp = window_run(adminapp);
	if(adminapp.current == 0){
		return 4;
	}
	else if(adminapp.current == 1){
		return 8;
	}
	else if(adminapp.current == 2){
		return 15;
	}
	else if(adminapp.current == 3){
		return 16;
	}
	else if(adminapp.current == 4){
		analysiswin();
		return 3;
	}
	else if(adminapp.current == 7){
		return 0;
	}
}
	
