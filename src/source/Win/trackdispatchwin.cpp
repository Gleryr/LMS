#include "../../header/Win/trackdispatchwin.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../header/file.h"

extern LNode *orderList;     
extern LNode *driverList;    
extern LNode *vehicleList;   
extern LNode *dispatchList;
extern IMAGE g_bg;

void getActiveValueFromSelect(const char* src, char* dest) {
    int i = 0;
    while (src[i] != '\0' && src[i] != ',') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

int trackDispatchWin() {
    WINDOW_T trackWin = {10, 20, 780, 600, WHITE, 13, {

        {60, 40, 100, 30, "调度单号：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 35, 150, 35, "", WHITE, LIGHTCYAN, BLACK, EDIT, 1},             // 1

        {360, 40, 100, 30, "关联订单：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {460, 35, 150, 35, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},             // 3

        {60, 95, 100, 30, "状态筛选：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 90, 200, 35, "全部,全部,未执行,运输中,已完成", WHITE, LIGHTCYAN, BLACK, SELECT, 0}, // 5

        {600, 90, 100, 40, "执行查询", YELLOW, LIGHTCYAN, BLACK, BUTTON, 0},   // 6

        {70,  500, 80,  30, "操作单号:", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {150, 500, 150, 35, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0}, 
        
        {320, 500, 80,  30, "变更状态:", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {400, 500, 100, 35, "运输中,运输中,已完成", WHITE, LIGHTCYAN, BLACK, SELECT, 0}, 

        {530, 490, 80, 45, "确认", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
        {660, 490, 80, 45, "返回", DARKGRAY, LIGHTGRAY, BLACK, BUTTON, 0}  
    }};

    Table_t table = {0};
    table.x = 20; 
    table.y = 150;          
    table.width = 760; 
    table.height = 340;     
    table.row = 6;          
    table.col = 5;          
    strcpy(table.text[0], "调度单号"); 
    strcpy(table.text[1], "订单号"); 
    strcpy(table.text[2], "司机ID"); 
    strcpy(table.text[3], "车辆ID");
    strcpy(table.text[4], "调度状态");

    int pageNow = 1;
    char activeDispID[50] = {0};
    char activeOrderID[50] = {0};
    char activeStatus[20] = "全部";

    while (1) {
        int filteredCount = 0;

        // 统计符合条件的数据总量
        LNode* tp = dispatchList->next;
        while (tp) {
            Dispatch_t* d = (Dispatch_t*)tp->data;
            int isMatch = 1;
            
            if (strlen(activeDispID) > 0 && strstr(d->Dispatch_ID, activeDispID) == NULL) isMatch = 0;
            if (strlen(activeOrderID) > 0 && strstr(d->Order_ID, activeOrderID) == NULL) isMatch = 0;
            
            if (isMatch) {
                if (strcmp(activeStatus, "待出库") == 0 && d->DispatchStatus != 0) isMatch = 0;
                else if (strcmp(activeStatus, "运输中") == 0 && d->DispatchStatus != 1) isMatch = 0;
                else if (strcmp(activeStatus, "已完成") == 0 && d->DispatchStatus != 2) isMatch = 0;
            }

            if (isMatch) filteredCount++;
            tp = tp->next;
        }

        int pageCount = (filteredCount == 0) ? 1 : (filteredCount + 7) / 8;
        if (pageNow > pageCount) pageNow = pageCount;

        // 提取当前页的表格数据
        memset(table.data, 0, sizeof(table.data));
        LNode* p = dispatchList->next;
        int count = 0;
        int skipped = 0;
        int skipLimit = (pageNow - 1) * 8;

        while (p && count < 8) {
            Dispatch_t* d = (Dispatch_t*)p->data;
            int isMatch = 1;
            
            if (strlen(activeDispID) > 0 && strstr(d->Dispatch_ID, activeDispID) == NULL) isMatch = 0;
            if (strlen(activeOrderID) > 0 && strstr(d->Order_ID, activeOrderID) == NULL) isMatch = 0;
            
            if (isMatch) {
                if (strcmp(activeStatus, "待出库") == 0 && d->DispatchStatus != 0) isMatch = 0;
                else if (strcmp(activeStatus, "运输中") == 0 && d->DispatchStatus != 1) isMatch = 0;
                else if (strcmp(activeStatus, "已完成") == 0 && d->DispatchStatus != 2) isMatch = 0;
            }

            if (isMatch) {
                if (skipped < skipLimit) {
                    skipped++;
                } else {
                    strcpy(table.data[count][0], d->Dispatch_ID);
                    strcpy(table.data[count][1], d->Order_ID);
                    strcpy(table.data[count][2], d->Driver_ID);
                    strcpy(table.data[count][3], d->Vehicle_ID);
                    
                    if (d->DispatchStatus == 0) strcpy(table.data[count][4], "待出库");
                    else if (d->DispatchStatus == 1) strcpy(table.data[count][4], "运输中");
                    else if (d->DispatchStatus == 2) strcpy(table.data[count][4], "已完成");
                    
                    count++;
                }
            }
            p = p->next;
        }

        putimage(0, 0, &g_bg);
        window_show(trackWin);
        table_show(table, pageNow, pageCount);

        trackWin = window_run(trackWin);

        // 查询
        if (trackWin.current == 6) {
            strcpy(activeDispID, trackWin.controls[1].text);
            strcpy(activeOrderID, trackWin.controls[3].text);
            char currentSelect[50] = {0};
            getActiveValueFromSelect(trackWin.controls[5].text, currentSelect);
            strcpy(activeStatus, currentSelect);
            pageNow = 1; 
        }

        // 确认修改
        else if (trackWin.current == 11) {
            char* tgtDispID = trackWin.controls[8].text;
            if (strlen(tgtDispID) == 0) {
                showPopup(300, 250, 250, 80, "请输入要操作的调度单号！");
                Sleep(1000);
                continue;
            }

            char targetStatusStr[20] = {0};
            getActiveValueFromSelect(trackWin.controls[10].text, targetStatusStr);
            int targetStatusInt = -1;
            if (strcmp(targetStatusStr, "运输中") == 0) targetStatusInt = 1;
            else if (strcmp(targetStatusStr, "已完成") == 0) targetStatusInt = 2;

            // 寻找对应的调度单
            Dispatch_t* targetDisp = NULL;
            LNode* fd = dispatchList->next;
            while(fd) {
                Dispatch_t* d = (Dispatch_t*)fd->data;
                if(strcmp(d->Dispatch_ID, tgtDispID) == 0) { targetDisp = d; break; }
                fd = fd->next;
            }

            if (targetDisp == NULL) {
                showPopup(300, 250, 250, 80, "未找到该调度单！");
                Sleep(800);
            } 
            else if (targetDisp->DispatchStatus == targetStatusInt) {
                showPopup(300, 250, 250, 80, "该调度单已经是此状态！");
                Sleep(800);
            } 
            else if (targetDisp->DispatchStatus == 2) {
                showPopup(300, 250, 250, 80, "已完结的单据不可再修改！");
                Sleep(800);
            } 
            else {
                // 更新状态
                targetDisp->DispatchStatus = targetStatusInt;
                fileWriteFromLinkList("./data/dispatch.txt", dispatchList, sizeof(Dispatch_t));

                if (targetStatusInt == 2) {
                    LNode* po = orderList->next;
                    while(po) { 
						Order_t* o = (Order_t*)po->data; 
							if(strcmp(o->ID, targetDisp->Order_ID) == 0) { 
							o->status = 5; 
							break; 
						} 
						po = po->next; 
					}
                    fileWriteFromLinkList("./data/order.txt", orderList, sizeof(Order_t));

                    LNode* pd = driverList->next;
                    while(pd) { 
						Driver_t* driver = (Driver_t*)pd->data; 
						if(strcmp(driver->User_ID, targetDisp->Driver_ID) == 0) { 
							driver->DriverStatus = 0; 
							break; 
						} 
						pd = pd->next; 
					}
                    fileWriteFromLinkList("./data/driver.txt", driverList, sizeof(Driver_t));

                    LNode* pv = vehicleList->next;
                    while(pv) { 
					Vehicle_t* vehicle = (Vehicle_t*)pv->data; 
						if(strcmp(vehicle->Vehicle_ID, targetDisp->Vehicle_ID) == 0) {
							vehicle->VehicleStatus = 0; 
							break; 
						} 
						pv = pv->next; 
					}
                    fileWriteFromLinkList("./data/vehicle.txt", vehicleList, sizeof(Vehicle_t));
                    
                    showPopup(300, 250, 250, 80, "订单已完结，资源已释放！");
                    Sleep(800);
                } else {
                    showPopup(300, 250, 250, 80, "调度单状态已更新为运输中！");
                    Sleep(800);
                }
                
                memset(trackWin.controls[8].text, 0, sizeof(trackWin.controls[8].text)); 
            }
            Sleep(1000);
        }

        // 左右翻页
        else if (trackWin.current == -1) {
            if (pageNow > 1) pageNow--;
            else { showPopup(300, 250, 200, 80, "已经是第一页了"); Sleep(800); }
        }
        else if (trackWin.current == -2) {
            if (pageNow < pageCount) pageNow++;
            else { showPopup(300, 250, 200, 80, "已经是最后一页了"); Sleep(800); }
        }
        
        // 返回
        else if (trackWin.current == 12) {
            return 1; 
        }
    }
}
