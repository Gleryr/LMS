#include "../../header/Win/createorderwin.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../../header/file.h"

extern LNode *orderList;
extern LNode *goodsList;
extern LNode *customerList;
extern IMAGE g_bg;

int createOrdWin() {
    WINDOW_T createWin = {50, 20, 700, 600, WHITE, 24, {
        {50, 80, 100, 30, "订单号：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 80, 200, 30, "", BLUE, WHITE, BLUE, LABEL, 0},         
        {50, 120, 100, 30, "客户姓名：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 120, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 1},   
        {50, 160, 100, 30, "联系电话：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 160, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},   
        {50, 200, 100, 30, "客户地址：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 200, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},   
        {50, 240, 100, 30, "货物名称：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 240, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},   
        {50, 280, 100, 30, "货物类型：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 280, 200, 30, "请选择,普通,易碎,化工,冷链", WHITE, LIGHTCYAN, BLACK, SELECT, 0}, 
        {50, 320, 100, 30, "货物重量：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 320, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},   
        {50, 360, 100, 30, "货物数量：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 360, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},   
        {50, 400, 100, 30, "货物体积：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 400, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},   
        {50, 440, 100, 30, "发货地址：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 440, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},   
        {50, 480, 100, 30, "收货地址：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 480, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},   
        {220, 530, 120, 45, "提  交", CYAN, LIGHTCYAN, BLACK, BUTTON, 0}, 
        {380, 530, 120, 45, "返  回", CYAN, LIGHTCYAN, BLACK, BUTTON, 0}  
    }};

    DataTime time = getDataTime();
    int oNum = getListNodeCount(orderList) + 1;
    char orderID[20] = {0};
    int isUnique = 0;
    while (!isUnique) {
        sprintf(orderID, "WL%4d%02d%02d%06d", time.year, time.month, time.day, oNum);
        isUnique = 1; 
        LNode* p = orderList->next;
        while (p) {
            Order_t* o = (Order_t*)p->data;
            if (strcmp(o->ID, orderID) == 0) { isUnique = 0; oNum++; break; }
            p = p->next;
        }
    }
    strcpy(createWin.controls[1].text, orderID);

    int cNum = getListNodeCount(customerList) + 1;
    char customerID[10] = {0};
    isUnique = 0;
    while (!isUnique) {
        sprintf(customerID, "%06d", cNum);
        isUnique = 1;
        LNode* p = customerList->next;
        while (p) {
            Customer_t* c = (Customer_t*)p->data;
            if (strcmp(c->Customer_ID, customerID) == 0) { isUnique = 0; cNum++; break; }
            p = p->next;
        }
    }

    int gNum = getListNodeCount(goodsList) + 1;
    char goodsID[20] = {0};
    isUnique = 0;
    while (!isUnique) {
        sprintf(goodsID, "%06d", gNum);
        isUnique = 1;
        LNode* p = goodsList->next;
        while (p) {
            Goods_t* g = (Goods_t*)p->data;
            if (strcmp(g->ID, goodsID) == 0) { isUnique = 0; gNum++; break; }
            p = p->next;
        }
    }

    while (1) {
        putimage(0, 0, &g_bg);
        window_show(createWin);
        createWin = window_run(createWin);

        if (createWin.current == 23) return 8;

        if (createWin.current == 22) {
            if (strlen(createWin.controls[3].text) == 0 || strlen(createWin.controls[9].text) == 0) {
                showPopup(300, 250, 250, 80, "请完整填写姓名和货名");
                Sleep(1000);
                continue;
            }

            int isValid = 1;
            if (strlen(createWin.controls[5].text) != 11) {
                isValid = 0;
            } else {
                for (int i = 0; i < 11; i++) {
                    if (createWin.controls[5].text[i] < '0' || createWin.controls[5].text[i] > '9') {
                        isValid = 0;
                        break;
                    }
                }
            }

            if (!isValid) {
                showPopup(300, 250, 250, 80, "手机号需为11位纯数字");
                Sleep(1000);
                continue; 
            }
			int isNumValid = 1;
			int checkIdx[] = {13, 15, 17};
			for (int k = 0; k < 3; k++) {
			    char* str = createWin.controls[checkIdx[k]].text;
			    if (strlen(str) == 0) { isNumValid = 0; break; }
			    for (int i = 0; i < strlen(str); i++) {
			        if (str[i] < '0' || str[i] > '9') { isNumValid = 0; break; }
			    }
			}
			if (!isNumValid) {
			    showPopup(300, 250, 250, 80, "输入错误,存在非数字符号!");
			    Sleep(1000);
			    continue;
			}
            Order_t neworder = {0};
            strcpy(neworder.ID, orderID);
            strcpy(neworder.goodsID, goodsID);
            strcpy(neworder.custumerID, customerID);
            strcpy(neworder.sendAddress, createWin.controls[19].text);
            strcpy(neworder.receiveAddress, createWin.controls[21].text);
            neworder.status = 0;

            Customer_t newcustomer = {0};
            strcpy(newcustomer.Customer_ID, customerID);
            strcpy(newcustomer.CustomerName, createWin.controls[3].text);
            strcpy(newcustomer.CustomerAddress, createWin.controls[7].text);
            strcpy(newcustomer.CustomerPhone, createWin.controls[5].text);

            Goods_t newgood = {0};
            strcpy(newgood.ID, goodsID);
            strcpy(newgood.name, createWin.controls[9].text);
            newgood.nums = atoi(createWin.controls[15].text);
            newgood.volume = atoi(createWin.controls[17].text);
            newgood.weight = atoi(createWin.controls[13].text);
            
            int type = 0;
            char* tText = createWin.controls[11].text;
            if (strstr(tText, "易碎")) type = 1;
            else if (strstr(tText, "化工")) type = 2;
            else if (strstr(tText, "冷链")) type = 3;
            newgood.type = type;

            insertTail(orderList, &neworder, sizeof(Order_t));
            insertTail(goodsList, &newgood, sizeof(Goods_t));
            insertTail(customerList, &newcustomer, sizeof(Customer_t));
            fileWriteFromLinkList("./data/order.txt", orderList, sizeof(Order_t));
            fileWriteFromLinkList("./data/good.txt", goodsList, sizeof(Goods_t));
            fileWriteFromLinkList("./data/customer.txt", customerList, sizeof(Customer_t));

            showPopup(300, 250, 200, 80, "订单提交成功！");
            Sleep(1000);
            return 8; 
        }
    }
}
