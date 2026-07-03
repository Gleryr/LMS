#include "../../header/Win/dispatchwin.h"
#include <graphics.h>
#include "../../header/Win/createdispatchwin.h"
#include "../../header/Win/selectdriverwin.h"
#include "../../header/Win/drivermanagewin.h"
#include "../../header/Win/vehiclemanagewin.h"
#include "../../header/Win/trackdispatchwin.h" 
extern IMAGE g_bg;

int dispatchWin() {
    WINDOW_T dispatchWin = {100, 80, 600, 500, WHITE, 6, {

        {150, 180, 220, 50, "1. 创建调度单", CYAN, LIGHTCYAN, BLACK, BUTTON, 1}, // 0
        {400, 180, 220, 50, "2. 调度单跟踪", CYAN, LIGHTCYAN, BLACK, BUTTON, 0}, // 1
        {150, 250, 220, 50, "3. 司机管理", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},   // 2
        {400, 250, 220, 50, "4. 车辆管理", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},   // 3
        {250, 320, 220, 50, "5. 返回上级", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},   // 4
        
        {120, 510, 500, 30, "操作说明：通过上下键切换菜单，按Enter键进入", BLACK, LIGHTGRAY, BLACK, LABEL, 0}
    }};

    while (1) {
        cleardevice();
        putimage(0, 0, &g_bg);
        window_show(dispatchWin);
        
        dispatchWin = window_run(dispatchWin);

        if (dispatchWin.current == 0) {
			createdispatchwin();
        }
        else if (dispatchWin.current == 1) {
			trackDispatchWin();
        }
        else if (dispatchWin.current == 2) {
			driverManagerWin();
        }
        else if (dispatchWin.current == 3) {
			vehicleManagerWin();
        }
        else if (dispatchWin.current == 4) {

            return 3;
        }
    }
}
