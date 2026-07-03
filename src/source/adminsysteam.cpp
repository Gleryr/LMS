#include "../header/Win/adminsysteam.h"


int adminsys(){
	WINDOW_T adminsys = {100, 80, 600, 500, WHITE, 9,
        {
            {120, 220, 220, 50, "1. 创建用户", CYAN, LIGHTCYAN, BLACK, BUTTON, 1},
            {380, 220, 220, 50, "2. 用户查询", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
            {120, 290, 220, 50, "3. 密码重置", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
            {380, 290, 220, 50, "4. 角色管理", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
            {120, 360, 220, 50, "5. 仓库管理", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
            {380, 360, 220, 50, "6. 运输配置", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
            {120, 430, 220, 50, "7. 数据备份", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
            {380, 430, 220, 50, "8. 退    出", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
            {120, 510, 500, 30, "操作说明：通过上下键切换菜单，按Enter键进入", BLACK, LIGHTGRAY, BLACK, LABEL, 0}
        }
	};
	
	window_show(adminsys);
	adminsys = window_run(adminsys);
	if(adminsys.current == 0){
		return 5;
	}
	else if(adminsys.current == 1){
		return 9;            //!
	}
	else if(adminsys.current == 2){
		return 6;
	}
	else if(adminsys.current == 3){
		return 7;
	}
	else if(adminsys.current == 7){
		return 3;
	}
	
}
