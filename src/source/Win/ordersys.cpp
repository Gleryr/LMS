//8
#include "../header/ordersys.h"
int orderSys(){
	WINDOW_T orderSys = {100, 80, 600, 500, WHITE, 6,
        {
            {120, 220, 220, 50, "1. 创建订单", CYAN, LIGHTCYAN, WHITE, BUTTON, 1},
            {380, 220, 220, 50, "2. 订单审核", CYAN, LIGHTCYAN, WHITE, BUTTON, 0},
            {120, 290, 220, 50, "3. 订单查询", CYAN, LIGHTCYAN, WHITE, BUTTON, 0},
            {380, 290, 220, 50, "4. 订单修改", CYAN, LIGHTCYAN, WHITE, BUTTON, 0},
            {120, 360, 220, 50, "5. 订单跟踪", CYAN, LIGHTCYAN, WHITE, BUTTON, 0},
            {380, 360, 220, 50, "6. 返回上级", CYAN, LIGHTCYAN, WHITE, BUTTON, 0},
            {120, 510, 500, 30, "操作说明：通过上下键切换菜单，按Enter键进入", BLACK, LIGHTGRAY, BLACK, LABEL, 0}
        }
	};
	
	
	window_show(orderSys);
	orderSys = window_run(orderSys);
	if(orderSys.current == 5){
		return 3;           //!
	}
	else if(orderSys.current == 0){
		return 9;           //!
	}
	
}
