#include "../../header/Win/auditorder.h"
#include "../../header/file.h"
extern LNode *orderList;
extern LNode *customerList;
extern IMAGE g_bg;

extern const char* getstate(int state);

int auditorderWin(){
    WINDOW_T auditorderWin = {50, 20, 650, 500, WHITE, 5,
        {
            {50, 40, 150, 30, "待审核订单列表", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
            {200, 40, 120, 35, "处理单号：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
            {320, 40, 200, 35, "", WHITE, WHITE, BLACK, EDIT, 1},
            {480, 40, 100, 35, "审核", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
            {550, 40, 100, 35, "返回", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
        } 
    };
    
    Table_t table = {0};
    table.x = 50; table.y = 150;
    table.width = 650; table.height = 300;
    table.row = 5; table.col = 4;
    strcpy(table.text[0], "订单编号"); 
    strcpy(table.text[1], "客户姓名");
    strcpy(table.text[2], "联系电话"); 
    strcpy(table.text[3], "当前状态");
    
    int pageNow = 1;
    char searchKey[20] = {0};
    
    while (1) {
        memset(table.data, 0, sizeof(table.data));
        
        // 修正统计逻辑：计算符合当前过滤条件的总数
        int totalFiltered = 0;
        LNode* tempP = orderList->next;
        while (tempP) {
            Order_t* o = (Order_t*)tempP->data;
            if (o->status == 0) {
                if (strlen(searchKey) == 0 || strstr(o->ID, searchKey) != NULL) {
                    totalFiltered++;
                }
            }
            tempP = tempP->next;
        }
        int pageCount = (totalFiltered + 3) / 4;
        if (pageCount <= 0) pageCount = 1;
        if (pageNow > pageCount) pageNow = pageCount;

        LNode* p = orderList->next;
        int count = 0;
        int skipped = 0;
        int skipLimit = (pageNow - 1) * 4;

        while (p != NULL && count < 4) {
            Order_t* u = (Order_t*)p->data;
            
            if (u->status != 0 || (strlen(searchKey) > 0 && strstr(u->ID, searchKey) == NULL)) {
                p = p->next;
                continue;
            }

            if (skipped < skipLimit) {
                skipped++;
            } else {
                Customer_t* targetCustomer = NULL;
                LNode* tmpNode = customerList->next;
                while(tmpNode != NULL) {
                    Customer_t* c = (Customer_t*)tmpNode->data;
                    if(strcmp(c->Customer_ID, u->custumerID) == 0) {
                        targetCustomer = c;
                        break; 
                    }
                    tmpNode = tmpNode->next;
                }

                if(targetCustomer != NULL) {
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

        putimage(0, 0, &g_bg);
        window_show(auditorderWin);
        table_show(table, pageNow, pageCount);
        
        auditorderWin = window_run(auditorderWin);

        if (auditorderWin.current == -1) {
            if (pageNow > 1) pageNow--;
            else { showPopup(300, 250, 200, 80, "已经是第一页了"); Sleep(800); }
        }
        else if (auditorderWin.current == -2) {
            if (pageNow < pageCount) pageNow++;
            else { showPopup(300, 250, 200, 80, "已经是最后一页了"); Sleep(800); }
        }
        else if (auditorderWin.current == 4) return 8;
        else if (auditorderWin.current == 3) {
            char targetID[20];
            strcpy(targetID, auditorderWin.controls[2].text);
            
            if (strlen(targetID) == 0) {
                showPopup(300, 220, 200, 80, "请输入处理单号");
                Sleep(800);
                continue;
            }

            LNode* findP = orderList->next;
            Order_t* targetOrder = NULL;
            while(findP) {
                Order_t* o = (Order_t*)findP->data;
                if(strcmp(o->ID, targetID) == 0 && o->status == 0) {
                    targetOrder = o;
                    break;
                }
                findP = findP->next;
            }

            if(targetOrder != NULL) {
                auditDetailWin(targetOrder); 
            } else {
                showPopup(300, 220, 300, 80, "无效单号或订单非待审核状态");
                Sleep(800);
            }
        }
    }
}

void auditDetailWin(Order_t *order) {
    if (order == NULL) return;
     
    WINDOW_T detailWin = { 150, 80, 500, 480, WHITE, 8, {
        {170, 70, 400, 25, "", BLACK, LIGHTGRAY, BLACK, LABEL, 0},  
        {170, 105, 400, 25, "", BLACK, LIGHTGRAY, BLACK, LABEL, 0}, 
        {170, 140, 400, 25, "", BLACK, LIGHTGRAY, BLACK, LABEL, 0}, 
        
        {170, 200, 150, 30, "审核备注/驳回理由：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {170, 240, 400, 35, "", WHITE, LIGHTCYAN, BLACK, EDIT, 1},  
        
        {170, 380, 120, 45, "批准", GREEN, LIGHTGREEN, WHITE, BUTTON, 0},
        {170, 440, 120, 45, "驳回", RED, LIGHTRED, WHITE, BUTTON, 0},
        {170, 500, 120, 45, "返回", DARKGRAY, LIGHTGRAY, WHITE, BUTTON, 0}
    }};

    sprintf(detailWin.controls[1].text, "订单编号：%s", order->ID);
    sprintf(detailWin.controls[2].text, "收货地址：%s", order->receiveAddress);
    sprintf(detailWin.controls[3].text, "发货地址：%s", order->sendAddress);

    while (1) {
        putimage(0, 0, &g_bg);
        window_show(detailWin);
        
        detailWin = window_run(detailWin);

        if (detailWin.current == 5) {
            order->status = 2;
            strncpy(order->reason, strlen(detailWin.controls[5].text) > 0 ? detailWin.controls[5].text : "审核通过", 99);
            fileWriteFromLinkList("./data/order.txt", orderList, sizeof(Order_t));
            showPopup(300, 250, 200, 80, "操作成功：已批准");
            break; 
        }

        else if (detailWin.current == 6) {
            if (strlen(detailWin.controls[5].text) == 0) {
                showPopup(300, 250, 250, 80, "请在备注框输入驳回原因");
                continue;
            }
            order->status = 1;
            strncpy(order->reason, detailWin.controls[5].text, 99);
            fileWriteFromLinkList("./data/order.txt", orderList, sizeof(Order_t));
            showPopup(300, 250, 200, 80, "操作成功：已驳回");
            break; 
        }

        else if (detailWin.current == 7) {
            break;
        }
    }
}
