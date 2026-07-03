#include "../../header/Win/storageinwin.h"
#include <string.h>
#include <stdio.h>
#include "../../header/file.h"

extern LNode *orderList;
extern LNode *goodsList;
extern LNode *customerList;
extern LNode *storageList;
extern IMAGE g_bg;

int storageinWin() {
    WINDOW_T storageinWin = {50, 20, 700, 500, WHITE, 16, {
        {250, 30, 200, 30, "仓储入库办理", BLACK, LIGHTGRAY, BLACK, LABEL, 0},    
        
        {50, 80, 100, 30, "订单号:", BLACK, LIGHTGRAY, BLACK, LABEL, 0},         
        {160, 75, 200, 40, "", CYAN, WHITE, BLACK, EDIT, 1},

        {50, 140, 100, 30, "货物名称:", BLACK, LIGHTGRAY, BLACK, LABEL, 0},       
        {160, 140, 200, 30, "", CYAN, LIGHTCYAN, RED, LABEL, 0},

        {50, 180, 100, 30, "货物重量:", BLACK, LIGHTGRAY, BLACK, LABEL, 0},       
        {160, 180, 200, 30, "", CYAN, LIGHTCYAN, BLACK, LABEL, 0},

        {50, 220, 100, 30, "货物数量:", BLACK, LIGHTGRAY, BLACK, LABEL, 0},       
        {160, 220, 200, 30, "", CYAN, LIGHTCYAN, BLACK, LABEL, 0},

        {50, 280, 100, 30, "存储货位:", BLACK, LIGHTGRAY, BLACK, LABEL, 0},       
        {160, 275, 200, 40, "", CYAN, WHITE, BLACK, EDIT, 0}, 

        {50, 330, 100, 30, "入库时间:", BLACK, LIGHTGRAY, BLACK, LABEL, 0},       
        {160, 330, 200, 30, "", CYAN, LIGHTCYAN, BLACK, LABEL, 0},

        {380, 75, 100, 40, "查询", YELLOW, LIGHTCYAN, BLACK, BUTTON, 0},  
        {200, 420, 120, 45, "入库", CYAN, LIGHTCYAN, BLACK, BUTTON, 0}, 
        {350, 420, 120, 45, "返回", DARKGRAY, LIGHTGRAY, BLACK, BUTTON, 0}
    }};

    Table_t todoTable = {0};
    todoTable.x = 420;          
    todoTable.y = 120;           
    todoTable.width = 230;      
    todoTable.height = 250;     
    todoTable.row = 6;          
    todoTable.col = 1;          
    strcpy(todoTable.text[0], "待办入库订单"); 

    Order_t* o = NULL;
    Goods_t* g = NULL;

    while (1) {
        memset(todoTable.data, 0, sizeof(todoTable.data));
        LNode* pNode = orderList->next;
        int todoCount = 0;
        
        while (pNode != NULL && todoCount < 5) {
            Order_t* tmpO = (Order_t*)pNode->data;
            if (tmpO->status == 2) { 
                strcpy(todoTable.data[todoCount][0], tmpO->ID);
                todoCount++;
            }
            pNode = pNode->next;
        }
        todoTable.count = todoCount;

        putimage(0, 0, &g_bg);
        window_show(storageinWin);
        table_show(todoTable, 1, 1);
        storageinWin = window_run(storageinWin);

        if (storageinWin.current == 15) return 8;

        if (storageinWin.current == 13) {
            char targetID[20];
            strcpy(targetID, storageinWin.controls[2].text);

            LNode* p = orderList->next;
            o = NULL;
            while(p) {
                Order_t* tmp = (Order_t*)p->data;
                if(strcmp(tmp->ID, targetID) == 0) { o = tmp; break; }
                p = p->next;
            }

            if (!o) {
                showPopup(300, 250, 200, 80, "未找到订单!");
                continue;
            }
            if (o->status != 2) {
                showPopup(300, 250, 250, 80, "订单状态不支持入库!");
                o = NULL;
                continue;
            }

            LNode* pg = goodsList->next;
            g = NULL;
            while(pg) {
                Goods_t* tmpG = (Goods_t*)pg->data;
                if(strcmp(tmpG->ID, o->goodsID) == 0) {
                    g = tmpG;
                    strcpy(storageinWin.controls[4].text, g->name);
                    sprintf(storageinWin.controls[6].text, "%d KG", g->weight);
                    sprintf(storageinWin.controls[8].text, "%d 件", g->nums);
                    break;
                }
                pg = pg->next;
            }

            DataTime dt = getDataTime();
            sprintf(storageinWin.controls[12].text, "%d-%02d-%02d", dt.year, dt.month, dt.day);

            showPopup(300, 250, 200, 80, "订单查询成功!");
        }

        if (storageinWin.current == 14) {
            if (o == NULL || g == NULL) {
                showPopup(300, 250, 200, 80, "请先查询订单!");
                continue;
            }
            if (strlen(storageinWin.controls[10].text) == 0) {
                showPopup(300, 250, 200, 80, "请输入货位!");
                continue;
            }

            Storage_t newStore;
            memset(&newStore, 0, sizeof(Storage_t));
            strcpy(newStore.ID, o->ID);
            strcpy(newStore.goodID, o->goodsID);
            strcpy(newStore.location, storageinWin.controls[10].text);
            strcpy(newStore.intime, storageinWin.controls[12].text); 

            insertTail(storageList, &newStore, sizeof(Storage_t));
            fileWriteFromLinkList("./data/storage.txt", storageList, sizeof(Storage_t));

            o->status = 3;
            fileWriteFromLinkList("./data/order.txt", orderList, sizeof(Order_t));

            showPopup(300, 250, 200, 80, "入库成功!");
            Sleep(1000);
            o = NULL;
            g = NULL;
            return 8; 
        }
    }
}
