#include "../../header/Win/createorderwin.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern LNode *orderList;
extern LNode *goodsList;
extern LNode *customerList;
extern IMAGE g_bg;

int resetorderWin() {
    WINDOW_T resetWin = {50, 20, 700, 600, WHITE, 25, {
        {50, 80, 100, 30, "订单号：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 80, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 1},       // 1
        {50, 120, 100, 30, "客户姓名：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 120, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},      // 3
        {50, 160, 100, 30, "联系电话：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 160, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},      // 5
        {50, 200, 100, 30, "客户地址：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 200, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},      // 7
        {50, 240, 100, 30, "货物名称：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 240, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},      // 9
        {50, 280, 100, 30, "货物类型：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 280, 200, 30, "普通,易碎,化工,冷链", WHITE, LIGHTCYAN, BLACK, SELECT, 0}, // 11
        {50, 320, 100, 30, "货物重量：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 320, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},      // 13
        {50, 360, 100, 30, "货物数量：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 360, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},      // 15
        {50, 400, 100, 30, "货物体积：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 400, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},      // 17
        {50, 440, 100, 30, "发货地址：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 440, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},      // 19
        {50, 480, 100, 30, "收货地址：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 480, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},      // 21
        
        {120, 530, 120, 45, "查  询", YELLOW, LIGHTCYAN, BLACK, BUTTON, 0}, // 22
        {300, 530, 120, 45, "修  改", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},     // 23
        {480, 530, 120, 45, "返  回", DARKGRAY, LIGHTGRAY, BLACK, BUTTON, 0}  // 24
    }};


    Table_t todoTable = {0};
    todoTable.x = 400;
    todoTable.y = 80;           
    todoTable.width = 250;      
    todoTable.height = 300;
    todoTable.row = 5;
    todoTable.col = 1;          
    strcpy(todoTable.text[0], "可修改订单 (待审核)"); 


    Order_t* foundOrder = NULL; 

    while (1) {
        memset(todoTable.data, 0, sizeof(todoTable.data)); 
        LNode* pNode = orderList->next;
        int todoCount = 0;
        
        while (pNode != NULL && todoCount < 7) { 
            Order_t* tmpO = (Order_t*)pNode->data;
            if (tmpO->status == 0) {
                strcpy(todoTable.data[todoCount][0], tmpO->ID);
                todoCount++;
            }
            pNode = pNode->next;
        }
        todoTable.count = todoCount;

        putimage(0, 0, &g_bg);
        window_show(resetWin);
        table_show(todoTable, 1, 1);
        resetWin = window_run(resetWin);

        if (resetWin.current == 24) return 8;

        //订单查询
        if (resetWin.current == 22) {
            char targetID[20];
            strcpy(targetID, resetWin.controls[1].text);

            LNode* p = orderList->next;
            foundOrder = NULL;
            while(p) {
                Order_t* tmp = (Order_t*)p->data;
                if(strcmp(tmp->ID, targetID) == 0) { foundOrder = tmp; break; }
                p = p->next;
            }

            if (!foundOrder) {
                showPopup(300, 250, 200, 80, "未找到该订单！");
                Sleep(1000);
                continue;
            }
            if (foundOrder->status != 0) {
                showPopup(300, 250, 250, 80, "非待审核状态不可修改！");
                Sleep(1000);
                foundOrder = NULL; 
                continue;
            }

            // 回显客户信息
            LNode* pc = customerList->next;
            while(pc) {
                Customer_t* c = (Customer_t*)pc->data;
                if(strcmp(c->Customer_ID, foundOrder->custumerID) == 0) {
                    strcpy(resetWin.controls[3].text, c->CustomerName);
                    strcpy(resetWin.controls[5].text, c->CustomerPhone);
                    strcpy(resetWin.controls[7].text, c->CustomerAddress);
                    break;
                }
                pc = pc->next;
            }
            // 回显货物信息
            LNode* pg = goodsList->next;
            while(pg) {
                Goods_t* g = (Goods_t*)pg->data;
                if(strcmp(g->ID, foundOrder->goodsID) == 0) {
                    strcpy(resetWin.controls[9].text, g->name);
                    sprintf(resetWin.controls[13].text, "%d", g->weight);
                    sprintf(resetWin.controls[15].text, "%d", g->nums);
                    sprintf(resetWin.controls[17].text, "%d", g->volume);
                    const char* types[] = {"普通", "易碎", "化工", "冷链"};
                    if(g->type >= 0 && g->type <= 3) strcpy(resetWin.controls[11].text, types[g->type]);
                    break;
                }
                pg = pg->next;
            }
            strcpy(resetWin.controls[19].text, foundOrder->sendAddress);
            strcpy(resetWin.controls[21].text, foundOrder->receiveAddress);

            showPopup(300, 250, 200, 80, "查询完毕，已载入数据");
            Sleep(1000);
        }

        //保存修改
        if (resetWin.current == 23) {
            if (!foundOrder) {
                showPopup(300, 250, 200, 80, "请先查询订单！");
                continue;
            }

            //手机号校验
            int isPhoneValid = 1;
            if (strlen(resetWin.controls[5].text) != 11) {
                isPhoneValid = 0;
            } else {
                for (int i = 0; i < 11; i++) {
                    if (resetWin.controls[5].text[i] < '0' || resetWin.controls[5].text[i] > '9') {
                        isPhoneValid = 0; break;
                    }
                }
            }
            if (!isPhoneValid) {
                showPopup(300, 250, 250, 80, "联系电话需为11位数字");
                Sleep(1000);
                continue;
            }

            // 数值合法性校验
            int isNumValid = 1;
            int checkIdx[] = {13, 15, 17};
            for (int k = 0; k < 3; k++) {
                char* str = resetWin.controls[checkIdx[k]].text;
                if (strlen(str) == 0) { isNumValid = 0; break; }
                for (int i = 0; i < (int)strlen(str); i++) {
                    if (str[i] < '0' || str[i] > '9') { isNumValid = 0; break; }
                }
            }
            if (!isNumValid) {
                showPopup(300, 250, 250, 80, "重量/数量/体积须为数字");
                Sleep(1000);
                continue;
            }

            //查找关联节点并覆盖数据
            Customer_t* foundCust = NULL;
            LNode* pCust = customerList->next;
            while(pCust) {
                Customer_t* c = (Customer_t*)pCust->data;
                if(strcmp(c->Customer_ID, foundOrder->custumerID) == 0) { foundCust = c; break; }
                pCust = pCust->next;
            }

            Goods_t* foundGood = NULL;
            LNode* pGood = goodsList->next;
            while(pGood) {
                Goods_t* g = (Goods_t*)pGood->data;
                if(strcmp(g->ID, foundOrder->goodsID) == 0) { foundGood = g; break; }
                pGood = pGood->next;
            }

            if (foundCust && foundGood) {
                strcpy(foundOrder->sendAddress, resetWin.controls[19].text);
                strcpy(foundOrder->receiveAddress, resetWin.controls[21].text);

                strcpy(foundCust->CustomerName, resetWin.controls[3].text);
                strcpy(foundCust->CustomerPhone, resetWin.controls[5].text);
                strcpy(foundCust->CustomerAddress, resetWin.controls[7].text);

                strcpy(foundGood->name, resetWin.controls[9].text);
                foundGood->weight = atoi(resetWin.controls[13].text);
                foundGood->nums = atoi(resetWin.controls[15].text);
                foundGood->volume = atoi(resetWin.controls[17].text);

                if(strstr(resetWin.controls[11].text, "普通")) foundGood->type = 0;
                else if(strstr(resetWin.controls[11].text, "易碎")) foundGood->type = 1;
                else if(strstr(resetWin.controls[11].text, "化工")) foundGood->type = 2;
                else if(strstr(resetWin.controls[11].text, "冷链")) foundGood->type = 3;

                showPopup(300, 250, 200, 80, "数据修改成功！");
                Sleep(1000);
                return 8;
            } else {
                showPopup(300, 250, 250, 80, "修改失败");
                Sleep(1000);
            }
        }
    }
}
