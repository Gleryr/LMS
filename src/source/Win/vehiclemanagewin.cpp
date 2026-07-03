#include "../../header/Win/vehiclemanagewin.h"
#include <graphics.h>
#include "../../header/file.h"

extern IMAGE g_bg;
extern LNode *vehicleList;
int vehicleManagerWin() {
    WINDOW_T managerWin = {100, 80, 600, 500, LIGHTGRAY, 4, {
        {300, 180, 220, 50, "1. 车辆信息录入", CYAN, LIGHTCYAN, BLACK, BUTTON, 1},
        {300, 250, 220, 50, "2. 车辆查询", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
        {300, 320, 220, 50, "3. 返回上级菜单", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
        
        {120, 510, 500, 30, "操作说明：通过上下键切换菜单，按 Enter 键进入", BLACK, LIGHTGRAY, BLACK, LABEL, 0}
    }};

    while (1) {
        cleardevice();
        putimage(0, 0, &g_bg);
        window_show(managerWin);

        managerWin = window_run(managerWin);

        if (managerWin.current == 0) {
            editVehicleWin(); 
        }
        else if (managerWin.current == 1) {
            selectVehicleWin();
        }
        else if (managerWin.current == 2) {
            return 1;
        }
    }
}

int editVehicleWin() {
    WINDOW_T editWin = {30, 20, 740, 550, WHITE, 13, {
        {40, 40, 200, 30, "车辆档案录入", BLACK, LIGHTGRAY, BLACK, LABEL, 0}, 

        {40, 100, 100, 30, "车牌号(ID):", BLACK, LIGHTGRAY, BLACK, LABEL, 0},        
        {150, 100, 130, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 1},                  // 2
        {290, 100, 60, 30, "检 索", YELLOW, LIGHTCYAN, BLACK, BUTTON, 0},           // 3

        {40, 170, 100, 30, "车辆类型:", BLACK, LIGHTGRAY, BLACK, LABEL, 0},         
        {150, 170, 200, 30, "厢式货车,厢式货车,冷链专车", WHITE, LIGHTCYAN, BLACK, SELECT, 0}, // 5

        {40, 240, 100, 30, "最大载重(KG):", BLACK, LIGHTGRAY, BLACK, LABEL, 0},         
        {150, 240, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},                  // 7

        {40, 310, 100, 30, "当前状态:", BLACK, LIGHTGRAY, BLACK, LABEL, 0},         
        {150, 310, 200, 30, "空闲,空闲,运输中,维修中", WHITE, LIGHTCYAN, BLACK, SELECT, 0}, 

        {90, 420, 100, 45, "保  存", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},            // 10
        {230, 420, 100, 45, "返  回", DARKGRAY, LIGHTGRAY, BLACK, BUTTON, 0},       // 11

        {420, 50, 300, 30, "系统现有车辆列表：", BLACK, LIGHTGRAY, BLACK, LABEL, 0}    // 12
    }};

    Table_t vehicleTable = {0};
    vehicleTable.x = 400;
    vehicleTable.y = 100;
    vehicleTable.width = 350;
    vehicleTable.height = 360;
    vehicleTable.row = 6; 
    vehicleTable.col = 3; 
    strcpy(vehicleTable.text[0], "车牌号");
    strcpy(vehicleTable.text[1], "车辆类型");
    strcpy(vehicleTable.text[2], "当前状态");

    int pageNow = 1;

    while (1) {
        memset(vehicleTable.data, 0, sizeof(vehicleTable.data));
        int totalVehicles = 0;
        int currentDrawRow = 0;
        int startIdx = (pageNow - 1) * 8;

        LNode* pv = vehicleList->next;
        while (pv) {
            Vehicle_t* v = (Vehicle_t*)pv->data;
            if (totalVehicles >= startIdx && currentDrawRow < 8) {
                strcpy(vehicleTable.data[currentDrawRow][0], v->Vehicle_ID);
                strcpy(vehicleTable.data[currentDrawRow][1], v->VehicleType == 0 ? "厢式货车" : "冷链专车");
                
                if (v->VehicleStatus == 0) strcpy(vehicleTable.data[currentDrawRow][2], "空闲");
                else if (v->VehicleStatus == 1) strcpy(vehicleTable.data[currentDrawRow][2], "运输中");
                else strcpy(vehicleTable.data[currentDrawRow][2], "维修中");
                
                currentDrawRow++;
            }
            totalVehicles++;
            pv = pv->next;
        }

        int pageCount = (totalVehicles == 0) ? 1 : (totalVehicles + 7) / 8;
        if (pageNow > pageCount) pageNow = pageCount;
        putimage(0, 0, &g_bg);
        window_show(editWin);
        table_show(vehicleTable, pageNow, pageCount);
        editWin = window_run(editWin);

        if (editWin.current == -1 ) {
            if (pageNow > 1) pageNow--;
            else { showPopup(300, 250, 200, 80, "已经是第一页了"); Sleep(800); }
        }
        else if (editWin.current == -2 ) {
            if (pageNow < pageCount) pageNow++;
            else { showPopup(300, 250, 200, 80, "已经是最后一页了"); Sleep(800); }
        }

        // 返回按钮
        else if (editWin.current == 11) {
            return 8; 
        }

        // 检索按钮
        else if (editWin.current == 3) {
            char* inputID = editWin.controls[2].text;
            if (strlen(inputID) == 0) {
                showPopup(300, 250, 200, 80, "请先输入车牌号！");
                continue;
            }

            int hasRecord = 0;
            LNode* pv_search = vehicleList->next;
            while (pv_search) {
                Vehicle_t* v = (Vehicle_t*)pv_search->data;
                if (strcmp(v->Vehicle_ID, inputID) == 0) {
                    if (v->VehicleType == 0) sprintf(editWin.controls[5].text, "厢式货车,厢式货车,冷链专车");
                    else sprintf(editWin.controls[5].text, "冷链专车,厢式货车,冷链专车");
                    
                    if (v->VehicleStatus == 0) sprintf(editWin.controls[9].text, "空闲,空闲,运输中,维修中");
                    else if (v->VehicleStatus == 1) sprintf(editWin.controls[9].text, "运输中,空闲,运输中,维修中");
                    else sprintf(editWin.controls[9].text, "维修中,空闲,运输中,维修中");
                    
                    // 恢复载重
                    sprintf(editWin.controls[7].text, "%d", v->CapacityWeight);
                    
                    hasRecord = 1;
                    showPopup(300, 250, 250, 80, "读取成功！可直接修改档案。");
                    break;
                }
                pv_search = pv_search->next;
            }

            if (!hasRecord) {
                memset(editWin.controls[7].text, 0, sizeof(editWin.controls[7].text));
                sprintf(editWin.controls[5].text, "厢式货车,厢式货车,冷链专车");
                sprintf(editWin.controls[9].text, "空闲,空闲,运输中,维修中");
                
                showPopup(300, 250, 250, 80, "首次建档，请补充详细信息。");
            }
            Sleep(800);
        }

        //保存按钮
        else if (editWin.current == 10) {
            char* inputID = editWin.controls[2].text;
            char* weightStr = editWin.controls[7].text;
            
            // 判空
            if (strlen(inputID) == 0 || strlen(weightStr) == 0) {
                showPopup(300, 250, 200, 80, "请填写车牌号和载重！");
                continue;
            }

            // 载重纯数字强校验
            int isNumValid = 1;
            for (int i = 0; i < strlen(weightStr); i++) {
                if (weightStr[i] < '0' || weightStr[i] > '9') { isNumValid = 0; break; }
            }
            if (!isNumValid) {
                showPopup(300, 250, 250, 80, "载重必须为纯数字！");
                Sleep(1000);
                continue;
            }

            // 提取类型 0 厢式, 1 冷链
            char typeMode[20] = {0};
            sscanf(editWin.controls[5].text, "%[^,]", typeMode);
            int vType = 0;
            if (strcmp(typeMode, "冷链专车") == 0) vType = 1;

            // 4. 提取状态 (0: 空闲, 1: 运输中, 2: 维修中)
            char statusMode[20] = {0};
            sscanf(editWin.controls[9].text, "%[^,]", statusMode);
            int vStatus = 0;
            if (strcmp(statusMode, "运输中") == 0) vStatus = 1;
            else if (strcmp(statusMode, "维修中") == 0) vStatus = 2;

            // 执行插入或更新
            int isUpdate = 0;
            LNode* pv_update = vehicleList->next;
            while (pv_update) {
                Vehicle_t* v = (Vehicle_t*)pv_update->data;
                if (strcmp(v->Vehicle_ID, inputID) == 0) {
                    v->VehicleType = vType;
                    v->CapacityWeight = atoi(weightStr);
                    v->VehicleStatus = vStatus;
                    isUpdate = 1;
                    break;
                }
                pv_update = pv_update->next;
            }

            if (!isUpdate) {
                Vehicle_t newVehicle = {0};
                strcpy(newVehicle.Vehicle_ID, inputID);
                newVehicle.VehicleType = vType;
                newVehicle.CapacityWeight = atoi(weightStr);
                newVehicle.VehicleStatus = vStatus;

                insertTail(vehicleList, &newVehicle, sizeof(Vehicle_t));
                fileWriteFromLinkList("./data/vehicle.txt", vehicleList, sizeof(Vehicle_t));
            }

            showPopup(300, 250, 200, 80, isUpdate ? "档案修改成功！" : "新车辆录入成功！");
            
            memset(editWin.controls[2].text, 0, sizeof(editWin.controls[2].text));
            memset(editWin.controls[7].text, 0, sizeof(editWin.controls[7].text));
            sprintf(editWin.controls[5].text, "厢式货车,厢式货车,冷链专车");
            sprintf(editWin.controls[9].text, "空闲,空闲,运输中,维修中");
            
            Sleep(800);

        }
    }
}

int selectVehicleWin() {
    WINDOW_T selectVehicleWin = {50, 20, 800, 600, WHITE, 6, {

        {80, 60, 100, 30, "车牌搜索：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {180, 55, 150, 35, "", WHITE, LIGHTCYAN, BLACK, EDIT, 1},             // 1

        {360, 60, 100, 30, "状态筛选：", BLACK, LIGHTGRAY, BLACK, LABEL, 0},
        {460, 55, 150, 35, "全部,全部,空闲,运输中,维修中", WHITE, LIGHTCYAN, BLACK, SELECT, 0}, // 3

        {640, 50, 100, 45, "执行搜索", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},     // 4

        {650, 520, 100, 40, "返 回", DARKGRAY, LIGHTGRAY, WHITE, BUTTON, 0}  // 5
    }};

    Table_t table = {0};
    table.x = 80; 
    table.y = 130;          
    table.width = 640; 
    table.height = 360;
    table.row = 7; 
    table.col = 4;
    strcpy(table.text[0], "车牌号"); 
    strcpy(table.text[1], "车辆类型"); 
    strcpy(table.text[2], "最大载重(KG)"); 
    strcpy(table.text[3], "当前状态");

    int pageNow = 1;
    
    char activeSearchID[50] = {0};
    char activeStatusMode[20] = "全部";

    while (1) {
        int filteredCount = 0;

        //车辆总数
        LNode* tp = vehicleList->next;
        while (tp) {
            Vehicle_t* v = (Vehicle_t*)tp->data;
            int isMatch = 1;
            
            // 匹配车牌
            if (strlen(activeSearchID) > 0 && strstr(v->Vehicle_ID, activeSearchID) == NULL) {
                isMatch = 0;
            }
            
            // 状态
            if (isMatch) {
                if (strcmp(activeStatusMode, "空闲") == 0 && v->VehicleStatus != 0) isMatch = 0;
                else if (strcmp(activeStatusMode, "运输中") == 0 && v->VehicleStatus != 1) isMatch = 0;
                else if (strcmp(activeStatusMode, "维修中") == 0 && v->VehicleStatus != 2) isMatch = 0;
            }

            if (isMatch) filteredCount++;
            tp = tp->next;
        }

        // 计算总页数 
        int pageCount = (filteredCount == 0) ? 1 : (filteredCount + 7) / 8;
        if (pageNow > pageCount) pageNow = pageCount;

        memset(table.data, 0, sizeof(table.data));
        LNode* p = vehicleList->next;
        int count = 0;
        int skipped = 0;
        int skipLimit = (pageNow - 1) * 8;

        while (p && count < 8) {
            Vehicle_t* v = (Vehicle_t*)p->data;
            int isMatch = 1;
            
            if (strlen(activeSearchID) > 0 && strstr(v->Vehicle_ID, activeSearchID) == NULL) isMatch = 0;
            
            if (isMatch) {
                if (strcmp(activeStatusMode, "空闲") == 0 && v->VehicleStatus != 0) isMatch = 0;
                else if (strcmp(activeStatusMode, "运输中") == 0 && v->VehicleStatus != 1) isMatch = 0;
                else if (strcmp(activeStatusMode, "维修中") == 0 && v->VehicleStatus != 2) isMatch = 0;
            }

            if (isMatch) {
                if (skipped < skipLimit) {
                    skipped++;
                } else {
                    strcpy(table.data[count][0], v->Vehicle_ID);
                    strcpy(table.data[count][1], v->VehicleType == 0 ? "厢式货车" : "冷链专车");
                    sprintf(table.data[count][2], "%d", v->CapacityWeight);
                    
                    if (v->VehicleStatus == 0) strcpy(table.data[count][3], "空闲");
                    else if (v->VehicleStatus == 1) strcpy(table.data[count][3], "运输中");
                    else strcpy(table.data[count][3], "维修中");
                    
                    count++;
                }
            }
            p = p->next;
        }

        putimage(0, 0, &g_bg);
        window_show(selectVehicleWin);
        table_show(table, pageNow, pageCount);


        selectVehicleWin = window_run(selectVehicleWin);

        // 搜索
        if (selectVehicleWin.current == 4) {
            strcpy(activeSearchID, selectVehicleWin.controls[1].text);
            sscanf(selectVehicleWin.controls[3].text, "%[^,]", activeStatusMode);
            
            pageNow = 1; 
        }

        // 翻页
        else if (selectVehicleWin.current == -1 ) {
            if (pageNow > 1) pageNow--;
            else { showPopup(300, 250, 200, 80, "已经是第一页了"); Sleep(800); }
        }
        else if (selectVehicleWin.current == -2 ) {
            if (pageNow < pageCount) pageNow++;
            else { showPopup(300, 250, 200, 80, "已经是最后一页了"); Sleep(800); }
        }

        else if (selectVehicleWin.current == 5) {
            return 1; 
        }
    }
}
    
