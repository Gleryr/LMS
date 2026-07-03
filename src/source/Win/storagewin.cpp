
#include "../../header/Win/selectstoragewin.h"
#include "../../header/Win/storagewin.h"
#include "../../header/Win/storageinwin.h"
#include "../../header/Win/storageoutwin.h"
extern IMAGE g_bg;
int storageWin(){
	WINDOW_T storageWin = {100, 80, 600, 500, WHITE, 4,
        {
            {120, 220, 220, 50, "1. 货物入库", CYAN, LIGHTCYAN, BLACK, BUTTON, 1},
            {380, 220, 220, 50, "2. 货物出库", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
            {120, 290, 220, 50, "3. 库存管理", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
            {380, 290, 220, 50, "4. 返回上级", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
            {120, 510, 500, 30, "操作说明：通过上下键切换菜单，按Enter键进入", BLACK, LIGHTGRAY, BLACK, LABEL, 0}
        }
	};
	
	while(1){
		cleardevice();
		putimage(0, 0, &g_bg);
		window_show(storageWin);
		storageWin = window_run(storageWin);
		if(storageWin.current == 0){
			storageinWin();
		}
		else if(storageWin.current == 1){
			storageoutWin();
		}
		else if(storageWin.current == 2){
			selectStorageWin();           //!
		}
		else if(storageWin.current == 3){
			return 3;           //!
		}
	}
	
	
	
}
