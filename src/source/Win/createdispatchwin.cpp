#include "../../header/Win/createdispatchwin.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../header/file.h"

extern LNode *orderList;
extern LNode *userList;
extern LNode *driverList;
extern LNode *vehicleList;
extern LNode *dispatchList;
extern IMAGE g_bg;

void getActiveValue(const char* src, char* dest) {
    int i = 0;
    while (src[i] != '\0' && src[i] != ',') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

int createdispatchwin() {
    // 界面尺寸 800x600，控件索引：15(SELECT), 16(查询), 17(确认), 18(返回)
    WINDOW_T createdispatchwin = {30, 20, 800, 600, WHITE, 19, {
        {50, 80, 100, 30, "调度单号：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 80, 200, 30, "", BLUE, WHITE, BLUE, LABEL, 0},          // 1
        
        {50, 130, 100, 30, "订单ID：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 130, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 1},    // 3
        
        {50, 180, 100, 30, "司机ID：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 180, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},    // 5
        
        {50, 230, 100, 30, "车辆ID：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 230, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},    // 7
        
        {50, 280, 100, 30, "线路信息：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 280, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},    // 9
        
        {50, 330, 100, 30, "预计出发：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 330, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},    // 11
        
        {50, 380, 100, 30, "预计到达：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {160, 380, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},    // 13

        {380, 80, 80, 30, "查询结果：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {460, 75, 200, 35, "待运订单,待运订单,空闲司机,空闲车辆", WHITE, LIGHTCYAN, BLACK, SELECT, 0}, // 15
        
        {680, 75, 80, 35, "查 询", YELLOW, LIGHTCYAN, BLACK, BUTTON, 0}, // 16

        {200, 520, 120, 45, "确认调度", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},  // 17
        {400, 520, 120, 45, "返  回", DARKGRAY, LIGHTGRAY, BLACK, BUTTON, 0} // 18
    }};

    DataTime time = getDataTime();
    int dNum = getListNodeCount(dispatchList) + 1;
    char dispatchID[20] = {0};
    int isUnique = 0;
    while (!isUnique) {
        sprintf(dispatchID, "DD%d%02d%02d%04d", time.year, time.month, time.day, dNum);
        isUnique = 1;
        LNode* p = dispatchList->next;
        while (p) {
            Dispatch_t* d = (Dispatch_t*)p->data;
            if (strcmp(d->Dispatch_ID, dispatchID) == 0) { isUnique = 0; dNum++; break; }
            p = p->next;
        }
    }
    strcpy(createdispatchwin.controls[1].text, dispatchID);

    Table_t infoTable = {0};
    infoTable.x = 380;        
    infoTable.y = 130;
    infoTable.width = 380;    
    infoTable.height = 300;
    infoTable.row = 6; 

    int pageNow = 1;
    char activeViewMode[50] = "待运订单";

    while (1) {
        memset(infoTable.data, 0, sizeof(infoTable.data));
        int totalMatch = 0;
        int currentDrawRow = 0;
        int startIdx = (pageNow - 1) * 5;

        if (strcmp(activeViewMode, "待运订单") == 0) {
            infoTable.col = 1; 
            strcpy(infoTable.text[0], "待分配订单ID");
            LNode* p = orderList->next;
            while (p) {
                Order_t* o = (Order_t*)p->data;
                if (o->status == 3) {
                    if (totalMatch >= startIdx && currentDrawRow < 5) {
                        strcpy(infoTable.data[currentDrawRow][0], o->ID);
                        currentDrawRow++;
                    }
                    totalMatch++;
                }
                p = p->next;
            }
        } 
        else if (strcmp(activeViewMode, "空闲司机") == 0) {
            infoTable.col = 3;
            strcpy(infoTable.text[0], "司机ID"); strcpy(infoTable.text[1], "姓名"); strcpy(infoTable.text[2], "驾照"); 
            LNode* p = driverList->next;
            while (p) {
                Driver_t* d = (Driver_t*)p->data;
                if (d->DriverStatus == 0) {
                    if (totalMatch >= startIdx && currentDrawRow < 5) {
                        strcpy(infoTable.data[currentDrawRow][0], d->User_ID);
                        // 获取姓名
                        char name[20] = "未知";
                        LNode* pu = userList->next;
                        while(pu) {
                            User_t* u = (User_t*)pu->data;
                            if(strcmp(u->ID, d->User_ID) == 0) { strcpy(name, u->name); break; }
                            pu = pu->next;
                        }
                        strcpy(infoTable.data[currentDrawRow][1], name);
                        strcpy(infoTable.data[currentDrawRow][2], d->LicenceType);
                        currentDrawRow++;
                    }
                    totalMatch++;
                }
                p = p->next;
            }
        } 
        else if (strcmp(activeViewMode, "空闲车辆") == 0) {
            infoTable.col = 3;
            strcpy(infoTable.text[0], "车牌号"); strcpy(infoTable.text[1], "类型"); strcpy(infoTable.text[2], "载重(KG)"); 
            LNode* p = vehicleList->next;
            while (p) {
                Vehicle_t* v = (Vehicle_t*)p->data;
                if (v->VehicleStatus == 0) {
                    if (totalMatch >= startIdx && currentDrawRow < 5) {
                        strcpy(infoTable.data[currentDrawRow][0], v->Vehicle_ID);
                        strcpy(infoTable.data[currentDrawRow][1], v->VehicleType == 0 ? "厢式" : "冷链");
                        sprintf(infoTable.data[currentDrawRow][2], "%d", v->CapacityWeight);
                        currentDrawRow++;
                    }
                    totalMatch++;
                }
                p = p->next;
            }
        }

        int pageCount = (totalMatch == 0) ? 1 : (totalMatch + 4) / 5;
        if (pageNow > pageCount) pageNow = pageCount;

        putimage(0, 0, &g_bg);
        window_show(createdispatchwin);
        table_show(infoTable, pageNow, pageCount);


        createdispatchwin = window_run(createdispatchwin);

        //  查询
        if (createdispatchwin.current == 16) {
            char currentSelect[50] = {0};
            getActiveValue(createdispatchwin.controls[15].text, currentSelect);
            
            if(strcmp(currentSelect, "查询目标") != 0) {
                strcpy(activeViewMode, currentSelect);
                pageNow = 1; 
            }
        }

        //翻页
        else if (createdispatchwin.current == -1 ) {
            if (pageNow > 1) pageNow--;
            else { showPopup(300, 250, 200, 80, "已经是第一页了"); Sleep(800); }
        }
        else if (createdispatchwin.current == -2 ) {
            if (pageNow < pageCount) pageNow++;
            else { showPopup(300, 250, 200, 80, "已经是最后一页了"); Sleep(800); }
        }
        
        //  返回
        else if (createdispatchwin.current == 18) return 1;

        // 确认
        else if (createdispatchwin.current == 17) {
            char* oID = createdispatchwin.controls[3].text;
            char* dID = createdispatchwin.controls[5].text;
            char* vID = createdispatchwin.controls[7].text;
            
            if (strlen(oID) == 0 || strlen(dID) == 0 || strlen(vID) == 0) {
                showPopup(300, 250, 300, 80, "请填写订单、司机和车辆ID！");
                continue;
            }

            // 业务校验逻辑逻辑
            Order_t* foundOrder = NULL;
            LNode* po = orderList->next;
            while(po) { Order_t* o = (Order_t*)po->data; if(strcmp(o->ID, oID) == 0) { foundOrder = o; break; } po = po->next; }
            if(!foundOrder || foundOrder->status != 3) { showPopup(300, 250, 250, 80, "订单ID错误或不可运"); continue; }

            Driver_t* foundDriver = NULL;
            LNode* pd = driverList->next;
            while(pd) { Driver_t* d = (Driver_t*)pd->data; if(strcmp(d->User_ID, dID) == 0) { foundDriver = d; break; } pd = pd->next; }
            if(!foundDriver || foundDriver->DriverStatus != 0) { showPopup(300, 250, 250, 80, "司机ID错误或不空闲"); continue; }

            Vehicle_t* foundVehicle = NULL;
            LNode* pv = vehicleList->next;
            while(pv) { Vehicle_t* v = (Vehicle_t*)pv->data; if(strcmp(v->Vehicle_ID, vID) == 0) { foundVehicle = v; break; } pv = pv->next; }
            if(!foundVehicle || foundVehicle->VehicleStatus != 0) { showPopup(300, 250, 250, 80, "车辆ID错误或不空闲"); continue; }

            // 保存调度单
            Dispatch_t newDisp = {0};
            strcpy(newDisp.Dispatch_ID, createdispatchwin.controls[1].text);
            strcpy(newDisp.Order_ID, oID);
            strcpy(newDisp.Driver_ID, dID);
            strcpy(newDisp.Vehicle_ID, vID);
            strcpy(newDisp.RouteInfo, createdispatchwin.controls[9].text);
            strcpy(newDisp.Est_Departure, createdispatchwin.controls[11].text);
            strcpy(newDisp.Est_Arrival, createdispatchwin.controls[13].text);
            newDisp.DispatchStatus = 0; 

            insertTail(dispatchList, &newDisp, sizeof(Dispatch_t));
            fileWriteFromLinkList("./data/dispatch.txt", dispatchList, sizeof(Dispatch_t));

            // 更新状态
            foundOrder->status = 4;
            foundDriver->DriverStatus = 1;
            foundVehicle->VehicleStatus = 1;
            fileWriteFromLinkList("./data/order.txt", orderList, sizeof(Order_t));
            fileWriteFromLinkList("./data/driver.txt", driverList, sizeof(Driver_t));
            fileWriteFromLinkList("./data/vehicle.txt", vehicleList, sizeof(Vehicle_t));

            showPopup(300, 250, 200, 80, "排单成功！");
            Sleep(1000);
            return 1;
        }
    }
}
