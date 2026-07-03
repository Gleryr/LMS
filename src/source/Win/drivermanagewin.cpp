#include "../../header/Win/drivermanagewin.h"
#include "../../header/Win/editdriverwin.h"

#include <graphics.h>

extern IMAGE g_bg;

int driverManagerWin() {
    WINDOW_T driverManagerWin = {100, 80, 600, 500, WHITE, 4, {
        {300, 180, 220, 50, "1. 司机信息录入", CYAN, LIGHTCYAN, BLACK, BUTTON, 1}, // 0
        {300, 250, 220, 50, "2. 司机查询与管理", CYAN, LIGHTCYAN, BLACK, BUTTON, 0}, // 1
        {300, 320, 220, 50, "3. 返回上级菜单", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},   // 2
        
        {120, 510, 500, 30, "操作说明：通过上下键切换菜单，按 Enter 键进入", BLACK, LIGHTGRAY, BLACK, LABEL, 0}
    }};

    while (1) {
        cleardevice();
        putimage(0, 0, &g_bg);
        window_show(driverManagerWin);

        driverManagerWin = window_run(driverManagerWin);

        // 业务分发
        if (driverManagerWin.current == 0) {
            editDriverWin();
        }
        else if (driverManagerWin.current == 1) {
            selectDriverWin();
        }
        else if (driverManagerWin.current == 2) {
            return 1;
        }
    }
}
