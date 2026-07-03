#include "../../header/Win/selectstoragewin.h"
#include <string.h>
#include <stdio.h>

extern LNode *orderList;
extern LNode *goodsList;
extern LNode *storageList;
extern IMAGE g_bg;

int selectStorageWin() {

    WINDOW_T selectWin = {50, 50, 700, 500, WHITE, 9, {
        {300, 20, 200, 30, "库 存 查 询", BLACK, LIGHTGRAY, BLACK, LABEL, 0},

        {50, 70, 100, 30, "货物名称:", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {150, 70, 150, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 1}, // 2

        {320, 70, 100, 30, "存储货位:", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {420, 70, 150, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0}, // 4

        {600, 65, 50, 40, "查询", YELLOW, LIGHTCYAN, BLACK, BUTTON, 0}, // 5
        {650, 65, 50, 40, "返回", DARKGRAY, LIGHTGRAY, BLACK, BUTTON, 0}, // 6

    }};

    //表格初始化
    Table_t table = {0};
    table.x = 80;
    table.y = 130;
    table.width = 640;
    table.height = 280;
    table.row = 6;
    table.col = 6;
    strcpy(table.text[0], "序号");
    strcpy(table.text[1], "货物名称");
    strcpy(table.text[2], "货物类型");
    strcpy(table.text[3], "库存数量");
    strcpy(table.text[4], "存储货位");
    strcpy(table.text[5], "入库时间");

    int pageNow = 1;
    int pageCount = 1;
    int rowsPerPage = table.row - 1;

    // 搜索条件缓冲
    char searchName[50] = {0};
    char searchLoc[50] = {0};

    while (1) {
        //组装并筛选数据
        memset(table.data, 0, sizeof(table.data));
        LNode* pNode = storageList->next;
        
        int totalMatch = 0; // 匹配到的总条数
        int startIdx = (pageNow - 1) * rowsPerPage; // 当前页的起始数据索引
        int currentDrawRow = 0; // 当前画到表格的第几行

        while (pNode != NULL) {
            Storage_t* s = (Storage_t*)pNode->data;
            
            char gName[50] = "未知";
            char gTypeStr[20] = "未知";
            int gNum = 0;
            
            LNode* pg = goodsList->next;
            while(pg) {
                Goods_t* g = (Goods_t*)pg->data;
                if(strcmp(g->ID, s->goodID) == 0) {
                    strcpy(gName, g->name);
                    gNum = g->nums;
                    const char* types[] = {"普通", "易碎", "化工", "冷链"};
                    if(g->type >= 0 && g->type <= 3) strcpy(gTypeStr, types[g->type]);
                    break;
                }
                pg = pg->next;
            }

            //筛选条件判断
            int match = 1;
            if (strlen(searchName) > 0 && strstr(gName, searchName) == NULL) {
                match = 0;
            }
            if (strlen(searchLoc) > 0 && strstr(s->location, searchLoc) == NULL) {
                match = 0;
            }

            //判断是否属于当前页
            if (match) {
                if (totalMatch >= startIdx && currentDrawRow < rowsPerPage) {
                    sprintf(table.data[currentDrawRow][0], "%d", totalMatch + 1);
                    strcpy(table.data[currentDrawRow][1], gName);
                    strcpy(table.data[currentDrawRow][2], gTypeStr);
                    sprintf(table.data[currentDrawRow][3], "%d", gNum);
                    strcpy(table.data[currentDrawRow][4], s->location);
                    strcpy(table.data[currentDrawRow][5], s->intime);
                    currentDrawRow++;
                }
                totalMatch++;
            }
            pNode = pNode->next;
        }

        // 计算总页数
        pageCount = (totalMatch == 0) ? 1 : (totalMatch + rowsPerPage - 1) / rowsPerPage;
        if (pageNow > pageCount) pageNow = pageCount; // 防止页码越界

        //绘制界面
        putimage(0, 0, &g_bg);
        window_show(selectWin);
        table_show(table, pageNow, pageCount);
        //运行界面并捕捉操作
        selectWin = window_run(selectWin);

        // 返回
        if (selectWin.current == 6) return 8;

        // 查询
        if (selectWin.current == 5) {
            strcpy(searchName, selectWin.controls[2].text);
            strcpy(searchLoc, selectWin.controls[4].text);
            pageNow = 1; // 每次查新重置为第一页
            
            selectWin.current = 5;
            for(int k=0; k<selectWin.count; k++) selectWin.controls[k].status = (k==5)?1:0;
        }

        // 上一页
        if (selectWin.current == -1) {
            if (pageNow > 1) pageNow--;
            else showPopup(300, 250, 200, 60, "已经是第一页了");
            
            selectWin.current = 7;
            for(int k=0; k<selectWin.count; k++) selectWin.controls[k].status = (k==7)?1:0;
        }

        // 下一页
        if (selectWin.current == -2) {
            if (pageNow < pageCount) pageNow++;
            else showPopup(300, 250, 200, 60, "已经是最后一页了");
            
            selectWin.current = 8;
            for(int k=0; k<selectWin.count; k++) selectWin.controls[k].status = (k==8)?1:0;
        }
    }
}
