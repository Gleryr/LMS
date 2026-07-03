#include "../../header/Win/storageoutwin.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../header/file.h"

extern LNode *orderList;
extern LNode *goodsList;
extern LNode *storageList;
extern IMAGE g_bg;

extern char username[10]; 

int storageoutWin() {
    WINDOW_T storageoutWin = {50, 20, 700, 500, WHITE, 16, {
        {250, 30, 200, 30, "货物出库办理", BLACK, LIGHTGRAY, BLACK, LABEL, 0},    // 0
        
        {50, 80, 100, 30, "订单号:", BLACK, LIGHTGRAY, BLACK, LABEL, 0},         // 1
        {160, 75, 200, 40, "", CYAN, WHITE, BLACK, EDIT, 1},                      // 2

        {50, 140, 100, 30, "货物名称:", BLACK, LIGHTGRAY, BLACK, LABEL, 0},       // 3
        {160, 140, 200, 30, "", CYAN, LIGHTCYAN, RED, LABEL, 0},      

        {50, 180, 100, 30, "存储货位:", BLACK, LIGHTGRAY, BLACK, LABEL, 0},       // 5
        {160, 180, 200, 30, "", CYAN, LIGHTCYAN, BLACK, LABEL, 0},

        {50, 220, 100, 30, "入库时间:", BLACK, LIGHTGRAY, BLACK, LABEL, 0},       // 7
        {160, 220, 200, 30, "", CYAN, LIGHTCYAN, BLACK, LABEL, 0},              

        {50, 280, 100, 30, "出库时间:", BLACK, LIGHTGRAY, BLACK, LABEL, 0},       // 9
        {160, 280, 200, 30, "", CYAN, LIGHTCYAN, BLACK, LABEL, 0},               // 10

        {50, 330, 100, 30, "经办人:", BLACK, LIGHTGRAY, BLACK, LABEL, 0},         // 11
        {160, 330, 200, 30, "", CYAN, LIGHTCYAN, BLACK, LABEL, 0},               // 12

        {380, 75, 100, 40, "查询", YELLOW, LIGHTCYAN, BLACK, BUTTON, 0},          // 13
        {200, 420, 120, 45, "确认出库", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},       // 14
        {350, 420, 120, 45, "返回", DARKGRAY, LIGHTGRAY, BLACK, BUTTON, 0}        // 15
    }};

    //初始化待出库表格 
    Table_t todoTable = {0};
    todoTable.x = 420;          
    todoTable.y = 120;           
    todoTable.width = 230;      
    todoTable.height = 250;     
    todoTable.row = 6;  
    todoTable.col = 1;          
    strcpy(todoTable.text[0], "待出库订单列表"); 
	Sleep(1000);
    Order_t* o = NULL;
    Storage_t* s = NULL;

    while (1) {
        //提取状态为 3 
        memset(todoTable.data, 0, sizeof(todoTable.data));
        LNode* pNode = orderList->next;
        int todoCount = 0;
        while (pNode != NULL && todoCount < 5) {
            Order_t* tmpO = (Order_t*)pNode->data;
            if (tmpO->status == 3) { // 
                strcpy(todoTable.data[todoCount][0], tmpO->ID);
                todoCount++;
            }
            pNode = pNode->next;
        }
        todoTable.count = todoCount;

        putimage(0, 0, &g_bg);
        window_show(storageoutWin);
        table_show(todoTable, 1, 1); // 绘制右侧表格
        storageoutWin = window_run(storageoutWin);

        if (storageoutWin.current == 15) return 8; // 统一返回上级索引

        // 查询逻辑
        if (storageoutWin.current == 13) {
            char targetID[20];
            strcpy(targetID, storageoutWin.controls[2].text);

            LNode* p = orderList->next;
            o = NULL;
            while(p) {
                Order_t* tmp = (Order_t*)p->data;
                if(strcmp(tmp->ID, targetID) == 0) { o = tmp; break; }
                p = p->next;
            }

            if (!o) {
                showPopup(300, 250, 200, 80, "未找到该订单!");
                Sleep(1000);
                continue;
            }
            
            if (o->status != 3) {
                showPopup(300, 250, 250, 80, "订单状态错误!");
                Sleep(1000);
                o = NULL;
                continue;
            }

            // 查找货物信息
            LNode* pg = goodsList->next;
            while(pg) {
                Goods_t* tmpG = (Goods_t*)pg->data;
                if(strcmp(tmpG->ID, o->goodsID) == 0) {
                    strcpy(storageoutWin.controls[4].text, tmpG->name);
                    break;
                }
                pg = pg->next;
            }

            // 查找库存信息
            LNode* ps = storageList->next;
            s = NULL;
            while(ps) {
                Storage_t* tmpS = (Storage_t*)ps->data;
                if(strcmp(tmpS->ID, o->ID) == 0) {
                    s = tmpS;
                    strcpy(storageoutWin.controls[6].text, s->location);
                    strcpy(storageoutWin.controls[8].text, s->intime);
                    break;
                }
                ps = ps->next;
            }

            // 设置时间与经办人
            DataTime dt = getDataTime();
            sprintf(storageoutWin.controls[10].text, "%d-%02d-%02d", dt.year, dt.month, dt.day);
            strcpy(storageoutWin.controls[12].text, username); 
            
            showPopup(300, 250, 200, 80, "查询成功!");
        }

        // 确认出库逻辑
        if (storageoutWin.current == 14) {
            if (o == NULL || s == NULL) {
                showPopup(300, 250, 200, 80, "请先查询订单!");
                Sleep(1000);
                continue;
            }

            // 更新状态为 4
            o->status = 4;
            fileWriteFromLinkList("./data/order.txt", orderList, sizeof(Order_t));

            // 从库存链表中移除该记录
            LNode* pre = storageList;
            LNode* cur = storageList->next;
            while (cur != NULL) {
                Storage_t* tmpS = (Storage_t*)cur->data;
                if (strcmp(tmpS->ID, o->ID) == 0) {
                    pre->next = cur->next;
                    free(cur->data);
                    free(cur);
                    break;
                }
                pre = cur;
                cur = cur->next;
            }
            fileWriteFromLinkList("./data/storage.txt", storageList, sizeof(Storage_t));

            showPopup(300, 250, 200, 80, "出库成功!");
            Sleep(1000);
            o = NULL;
            s = NULL;
            return 8;
        }
    }
}
