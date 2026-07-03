#include "../../header/Win/ordertrackwin.h"
#include <string.h>
#include <stdio.h>

extern LNode *orderList;
extern LNode *customerList;
extern IMAGE g_bg;

static Customer_t* findCust(LNode* head, const char* id) {
    LNode* p = head->next;
    while (p != NULL) {
        Customer_t* c = (Customer_t*)p->data;
        if (strcmp(c->Customer_ID, id) == 0) return c;
        p = p->next;
    }
    return NULL;
}

static const char* trackState(int state) {
    switch (state) {
        case 0: return "待审核";
        case 1: return "已驳回";
        case 2: return "待出库";
        case 3: return "待运输";
        case 4: return "运输中";
        case 5: return "已送达";
        case 6: return "已完成";
        default: return "未知";
    }
}

int orderTrackWin() {
    WINDOW_T trackWin = {50, 20, 700, 550, WHITE, 5, {
        {280, 40, 200, 30, "订单实时跟踪", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {50, 105, 100, 30, "输入订单号：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 100, 250, 40, "", WHITE, LIGHTCYAN, BLACK, EDIT, 1},
        
        {430, 100, 100, 40, "跟踪查询", CYAN, LIGHTCYAN, BLACK, BUTTON, 0}, 
        {550, 100, 100, 40, "返回", DARKGRAY, LIGHTGRAY, BLACK, BUTTON, 0} 
    }};

    Table_t table = {0};
    table.x = 50; table.y = 180;
    table.width = 700; table.height = 300;
    table.row = 5;
    table.col = 4;
    strcpy(table.text[0], "订单编号"); 
    strcpy(table.text[1], "收货人");
    strcpy(table.text[2], "收货地址"); 
    strcpy(table.text[3], "当前物流状态");

    int pageNow = 1;
    char searchID[20] = {0}; // 搜索关键词

    while (1) {
        int filteredCount = 0;
        LNode* tempP = orderList->next;
        while (tempP != NULL) {
            Order_t* o = (Order_t*)tempP->data;
            if (strlen(searchID) == 0 || strstr(o->ID, searchID)) {
                filteredCount++;
            }
            tempP = tempP->next;
        }

        int pageCount = (filteredCount + 3) / 4; 
        if (pageCount == 0) pageCount = 1;
        if (pageNow > pageCount) pageNow = pageCount;
        memset(table.data, 0, sizeof(table.data));
        LNode* p = orderList->next;
        int count = 0;
        int skipped = 0;
        int skipLimit = (pageNow - 1) * 4;

        while (p != NULL && count < 4) {
            Order_t* o = (Order_t*)p->data;
            
            if (strlen(searchID) == 0 || strstr(o->ID, searchID)) {
                if (skipped < skipLimit) {
                    skipped++;
                } else {
                    Customer_t* c = findCust(customerList, o->custumerID);
                    
                    strcpy(table.data[count][0], o->ID);
                    strcpy(table.data[count][1], c ? c->CustomerName : "未知");
                    strcpy(table.data[count][2], o->receiveAddress);
                    strcpy(table.data[count][3], trackState(o->status));
                    count++;
                }
            }
            p = p->next;
        }
        table.count = count;
        putimage(0, 0, &g_bg);      
        window_show(trackWin); 
        table_show(table, pageNow, pageCount); 
        
        trackWin = window_run(trackWin); 
        if (trackWin.current == -1) {
            if (pageNow > 1) pageNow--;
            else { showPopup(300, 250, 200, 80, "已经是第一页了"); Sleep(800); }
        }
        else if (trackWin.current == -2) {
            if (pageNow < pageCount) pageNow++;
            else { showPopup(300, 250, 200, 80, "已经是最后一页了"); Sleep(800); }
        }
        else if (trackWin.current == 3) {
            strcpy(searchID, trackWin.controls[2].text);
            pageNow = 1; 
        }
        else if (trackWin.current == 4) {
            return 8;
        }
    }
}
