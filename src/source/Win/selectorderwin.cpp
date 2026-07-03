#include "../../header/Win/selectorderwin.h"
#include <string.h>
#include <stdio.h>

extern LNode *orderList;
extern LNode *customerList;
extern IMAGE g_bg;

Customer_t* findCustByID(LNode* head, const char* id) {
    LNode* p = head->next;
    while (p != NULL) {
        Customer_t* c = (Customer_t*)p->data;
        if (strcmp(c->Customer_ID, id) == 0) return c;
        p = p->next;
    }
    return NULL;
}

const char* getstate(int state) {
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

int selectorderWin() {
    WINDOW_T selectorderWin = {50, 20, 750, 550, WHITE, 6, {
        {50, 45, 80, 30, "订单号：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {130, 40, 160, 40, "", WHITE, LIGHTCYAN, BLACK, EDIT, 1}, 
        
        {310, 45, 100, 30, "客户姓名：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {410, 40, 160, 40, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0}, 
        
        {600, 40, 70, 40, "查询", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
        {680, 40, 70, 40, "返回", CYAN, LIGHTCYAN, BLACK, BUTTON, 0}
    }};

    //表格初始化
    Table_t table = {0};
    table.x = 75; table.y = 150;
    table.width = 700; table.height = 300;
    table.row = 5;
    table.col = 4;
    strcpy(table.text[0], "订单号"); 
    strcpy(table.text[1], "客户姓名");
    strcpy(table.text[2], "联系电话"); 
    strcpy(table.text[3], "订单状态");

    int pageNow = 1;
    char keyID[20] = {0};
    char keyName[20] = {0};

    while (1) {
        //计符合组合条件的总结果数
        int filteredCount = 0;
        LNode* tempP = orderList->next;
        while (tempP != NULL) {
            Order_t* u = (Order_t*)tempP->data;
            int isMatch = 1;

            // 订单号过滤
            if (strlen(keyID) > 0 && strstr(u->ID, keyID) == NULL) isMatch = 0;
            
            // 姓名过滤
            if (isMatch && strlen(keyName) > 0) {
                Customer_t* c = findCustByID(customerList, u->custumerID);
                if (c == NULL || strstr(c->CustomerName, keyName) == NULL) isMatch = 0;
            }

            if (isMatch) filteredCount++;
            tempP = tempP->next;
        }

        int pageCount = (filteredCount + 3) / 4;
        if (pageCount == 0) pageCount = 1;
        if (pageNow > pageCount) pageNow = pageCount;

        // 填充当前页要显示的数据
        memset(table.data, 0, sizeof(table.data));
        LNode* p = orderList->next;
        int count = 0;
        int skipped = 0;
        int skipLimit = (pageNow - 1) * 4;

        while (p != NULL && count < 4) {
            Order_t* u = (Order_t*)p->data;
            int isMatch = 1;

            if (strlen(keyID) > 0 && strstr(u->ID, keyID) == NULL) isMatch = 0;
            
            Customer_t* targetCustomer = findCustByID(customerList, u->custumerID);
            if (isMatch && strlen(keyName) > 0) {
                if (targetCustomer == NULL || strstr(targetCustomer->CustomerName, keyName) == NULL) isMatch = 0;
            }

            if (isMatch && targetCustomer != NULL) {
                if (skipped < skipLimit) {
                    skipped++;
                } else {
                    strcpy(table.data[count][0], u->ID);
                    strcpy(table.data[count][1], targetCustomer->CustomerName);
                    strcpy(table.data[count][2], targetCustomer->CustomerPhone);
                    strcpy(table.data[count][3], getstate(u->status));
                    count++;
                }
            }
            p = p->next;
        }
        table.count = count;

        putimage(0, 0, &g_bg);      // 绘制背景
        window_show(selectorderWin); // 绘制搜索栏
        table_show(table, pageNow, pageCount); // 绘制表格
        
        selectorderWin = window_run(selectorderWin); // 运行交互逻辑
        

        if (selectorderWin.current == -1) {
            if (pageNow > 1) {
                pageNow--;
            } 
			else {
                showPopup(300, 250, 200, 80, "已经是第一页了！");
                Sleep(800);
            }
        } 
        // 右翻页操作
        else if (selectorderWin.current == -2) {
            if (pageNow < pageCount) {
                pageNow++;
            } 
			else {
                showPopup(300, 250, 200, 80, "已经是最后一页了！");
                Sleep(800);
            }
        }
        // 查询按钮点击
        else if (selectorderWin.current == 4) {
            strcpy(keyID, selectorderWin.controls[1].text);
            strcpy(keyName, selectorderWin.controls[3].text);
            pageNow = 1; // 搜索后重置到第一页
        }
        
        // 返回按钮点击
        else if (selectorderWin.current == 5) {
            return 8; // 返回到上一级菜单
        }
    }
    return 0;
}
