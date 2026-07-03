#include "../../header/Win/selectdriverwin.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern LNode *driverList;
extern LNode *userList;
extern IMAGE g_bg;

int selectDriverWin() {
    WINDOW_T driverWin = {50, 20, 800, 600, WHITE, 9, {
        {60, 60, 100, 30, "姓名搜索：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 55, 120, 35, "", WHITE, LIGHTCYAN, BLACK, EDIT, 1},             // 1

        {310, 60, 100, 30, "状态筛选：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {410, 55, 150, 35, "全部,全部,空闲,运输中,请假", WHITE, LIGHTCYAN, BLACK, SELECT, 0}, // 3 

        {600, 50, 100, 45, "执行搜索", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},     // 4

        {60, 120, 150, 30, "输入操作的司机ID：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {220, 115, 150, 35, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},            // 6 
        {400, 110, 120, 40, "请假 / 销假", YELLOW, LIGHTCYAN, BLACK, BUTTON, 0}, // 7

        {550, 520, 100, 40, "返 回", CYAN, LIGHTCYAN, BLACK, BUTTON, 0}  // 
    }};

    Table_t table = {0};
    table.x = 60; 
    table.y = 180;          
    table.width = 680; 
    table.height = 300;
    table.row = 7; 
    table.col = 5;
    strcpy(table.text[0], "司机ID"); strcpy(table.text[1], "姓名"); 
    strcpy(table.text[2], "联系电话"); strcpy(table.text[3], "驾照"); strcpy(table.text[4], "当前状态");

    int pageNow = 1;
    
    char activeSearchName[50] = {0};
    char activeStatusMode[20] = "全部";

    while (1) {
        int filteredCount = 0;

        LNode* tp = driverList->next;
        while (tp) {
            Driver_t* d = (Driver_t*)tp->data;
            int isMatch = 1;
            
            char actualName[20] = "未知";
            LNode* pu = userList->next;
            while(pu) {
                User_t* u = (User_t*)pu->data;
                if(strcmp(u->ID, d->User_ID) == 0) {
                    strcpy(actualName, u->name);
                    break;
                }
                pu = pu->next;
            }
            
            if (strlen(activeSearchName) > 0 && strstr(actualName, activeSearchName) == NULL) isMatch = 0;
            
            if (isMatch) {
                if (strstr(activeStatusMode, "空闲") && d->DriverStatus != 0) isMatch = 0;
                else if (strstr(activeStatusMode, "运输中") && d->DriverStatus != 1) isMatch = 0;
                else if (strstr(activeStatusMode, "请假") && d->DriverStatus != 2) isMatch = 0;
            }

            if (isMatch) filteredCount++;
            tp = tp->next;
        }

        int pageCount = (filteredCount == 0) ? 1 : (filteredCount + 5) / 6;
        if (pageNow > pageCount) pageNow = pageCount;

        memset(table.data, 0, sizeof(table.data));
        LNode* p = driverList->next;
        int count = 0;
        int skipped = 0;
        int skipLimit = (pageNow - 1) * 6;

        while (p && count < 6) {
            Driver_t* d = (Driver_t*)p->data;
            int isMatch = 1;
            
            char actualName[20] = "未知";
            LNode* pu = userList->next;
            while(pu) {
                User_t* u = (User_t*)pu->data;
                if(strcmp(u->ID, d->User_ID) == 0) {
                    strcpy(actualName, u->name);
                    break;
                }
                pu = pu->next;
            }

            if (strlen(activeSearchName) > 0 && strstr(actualName, activeSearchName) == NULL) isMatch = 0;
            if (isMatch) {
                if (strstr(activeStatusMode, "空闲") && d->DriverStatus != 0) isMatch = 0;
                else if (strstr(activeStatusMode, "运输中") && d->DriverStatus != 1) isMatch = 0;
                else if (strstr(activeStatusMode, "请假") && d->DriverStatus != 2) isMatch = 0;
            }

            if (isMatch) {
                if (skipped < skipLimit) {
                    skipped++;
                } else {
                    strcpy(table.data[count][0], d->User_ID);
                    strcpy(table.data[count][1], actualName);
                    strcpy(table.data[count][2], d->DriverPhone);
                    strcpy(table.data[count][3], d->LicenceType);
                    
                    if (d->DriverStatus == 0) strcpy(table.data[count][4], "空闲");
                    else if (d->DriverStatus == 1) strcpy(table.data[count][4], "运输中");
                    else strcpy(table.data[count][4], "请假");
                    
                    count++;
                }
            }
            p = p->next;
        }

        // 画面绘制
        putimage(0, 0, &g_bg);
        window_show(driverWin);
        table_show(table, pageNow, pageCount);

        // 操作响应
        driverWin = window_run(driverWin);
        if (driverWin.current == 4) {
            strcpy(activeSearchName, driverWin.controls[1].text);
            sscanf(driverWin.controls[3].text, "%[^,]", activeStatusMode);
            pageNow = 1; 
        }

        // 翻页逻辑
        else if (driverWin.current == -1 ) {
            if (pageNow > 1) pageNow--;
            else { showPopup(300, 250, 200, 80, "已经是第一页了"); Sleep(800); }
        }
        else if (driverWin.current == -2 ) {
            if (pageNow < pageCount) pageNow++;
            else { showPopup(300, 250, 200, 80, "已经是最后一页了"); Sleep(800); }
        }

        else if (driverWin.current == 8) return 1;

        else if (driverWin.current == 7) {
            char* tgtID = driverWin.controls[6].text;
            if (strlen(tgtID) == 0) {
                showPopup(300, 250, 300, 80, "请先输入要操作的司机ID");
                Sleep(1000);
                continue;
            }

            Driver_t* targetDriver = NULL;
            LNode* fp = driverList->next;
            while(fp) {
                Driver_t* td = (Driver_t*)fp->data;
                if(strcmp(td->User_ID, tgtID) == 0) { targetDriver = td; break; }
                fp = fp->next;
            }

            if (targetDriver == NULL) {
                showPopup(300, 250, 200, 80, "未找到该司机档案！");
                Sleep(1000);
            } else {
                if (targetDriver->DriverStatus == 1) {
                    showPopup(300, 250, 300, 80, "司机正在运输中，无法办理请假！");
                } 
                else if (targetDriver->DriverStatus == 0) {
                    targetDriver->DriverStatus = 2;
                    showPopup(300, 250, 200, 80, "办理请假成功！");
                } 
                else if (targetDriver->DriverStatus == 2) {
                    targetDriver->DriverStatus = 0;
                    showPopup(300, 250, 200, 80, "销假成功，恢复空闲！");
                }
                Sleep(1000);
            }
            memset(driverWin.controls[6].text, 0, sizeof(driverWin.controls[6].text)); 
        }
    }
}
