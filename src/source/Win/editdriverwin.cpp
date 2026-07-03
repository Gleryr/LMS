#include "../../header/Win/editdriverwin.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 
#include "../../header/file.h"

extern LNode *driverList;
extern LNode *userList;
extern IMAGE g_bg;

int editDriverWin() {
    WINDOW_T editDriverWin = {30, 20, 740, 550, WHITE, 15, {
        {40, 40, 200, 30, "司机档案录入", BLACK, LIGHTGRAY, BLACK, LABEL, 0}, 
        
        {40, 100, 100, 30, "输入账号ID:", BLACK, LIGHTGRAY, BLACK, LABEL, 0},        
        {150, 100, 130, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 1},
        {290, 100, 60, 30, "检 索", YELLOW, LIGHTCYAN, BLACK, BUTTON, 0},
        
        {40, 160, 100, 30, "司机姓名:", BLACK, LIGHTGRAY, BLACK, LABEL, 0},         
        {150, 160, 200, 30, "", BLUE, WHITE, BLUE, LABEL, 0},         // 5 
        
        {40, 220, 100, 30, "身份证号:", BLACK, LIGHTGRAY, BLACK, LABEL, 0},         
        {150, 220, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},
        
        {40, 280, 100, 30, "手机号码:", BLACK, LIGHTGRAY, BLACK, LABEL, 0},         
        {150, 280, 200, 30, "", WHITE, LIGHTCYAN, BLACK, EDIT, 0},                  // 9
        
        {40, 340, 100, 30, "驾照类型:", BLACK, LIGHTGRAY, BLACK, LABEL, 0},         
        {150, 340, 200, 30, "A2,A1,A2,B1,B2,C1", WHITE, LIGHTCYAN, BLACK, SELECT, 0}, // 11
        
        {90, 450, 100, 45, "保  存", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},            // 12
        {230, 450, 100, 45, "返  回", DARKGRAY, LIGHTGRAY, BLACK, BUTTON, 0},         // 13

        {420, 50, 300, 30, "系统已注册的司机账号列表：", BLACK, LIGHTGRAY, BLACK, LABEL, 0} // 14
    }};

    Table_t userTable = {0};
    userTable.x = 420;
    userTable.y = 100;
    userTable.width = 380;
    userTable.height = 360;
    userTable.row = 9; // 1表头 + 8数据
    userTable.col = 2;
    strcpy(userTable.text[0], "司机账号(ID)");
    strcpy(userTable.text[1], "系统姓名");

    int pageNow = 1;

    while (1) {
        memset(userTable.data, 0, sizeof(userTable.data));
        int totalDrivers = 0;
        int currentDrawRow = 0;
        int startIdx = (pageNow - 1) * 8;

        LNode* pu = userList->next;
        while (pu) {
            User_t* u = (User_t*)pu->data;
            // 筛选出角色为 4 的系统用户
            if (u->role == 4) {
                if (totalDrivers >= startIdx && currentDrawRow < 8) {
                    strcpy(userTable.data[currentDrawRow][0], u->ID);
                    strcpy(userTable.data[currentDrawRow][1], u->name);
                    currentDrawRow++;
                }
                totalDrivers++;
            }
            pu = pu->next;
        }

        int pageCount = (totalDrivers == 0) ? 1 : (totalDrivers + 7) / 8;
        if (pageNow > pageCount) pageNow = pageCount;

        putimage(0, 0, &g_bg);
        window_show(editDriverWin);
        table_show(userTable, pageNow, pageCount);
        editDriverWin = window_run(editDriverWin);

        // 翻页逻辑 
        if (editDriverWin.current == -1) {
            if (pageNow > 1) {
                pageNow--;
            } 
			else {
                showPopup(300, 250, 200, 80, "已经是第一页了！");
                Sleep(800);
            }
        } 
        // 右翻页操作
        else if (editDriverWin.current == -2) {
            if (pageNow < pageCount) {
                pageNow++;
            } 
			else {
                showPopup(300, 250, 200, 80, "已经是最后一页了！");
                Sleep(800);
            }
        }
        else if (editDriverWin.current == 13) {
            return 1; 
        }

        else if (editDriverWin.current == 3) {
            char* inputID = editDriverWin.controls[2].text;
            if (strlen(inputID) == 0) {
                showPopup(300, 250, 200, 80, "请先输入司机账号ID！");
            	Sleep(1000);    
                continue;
            }

            //先验证系统账号是否存在
            User_t* foundUser = NULL;
            LNode* pNode = userList->next;
            while (pNode) {
                User_t* u = (User_t*)pNode->data;
                if (strcmp(u->ID, inputID) == 0 && u->role == 4) { foundUser = u; break; }
                pNode = pNode->next;
            }

            if (!foundUser) {
                showPopup(300, 250, 300, 80, "账号不存在或角色非司机！");
                memset(editDriverWin.controls[5].text, 0, sizeof(editDriverWin.controls[5].text)); // 清空姓名
                strcpy(editDriverWin.controls[5].text, "检索后自动获取");
                continue;
            }

            // 账号存在，把名字填充到只读 Label
            strcpy(editDriverWin.controls[5].text, foundUser->name);

            //查业务系统看是否建过档
            int hasRecord = 0;
            LNode* pd = driverList->next;
            while (pd) {
                Driver_t* d = (Driver_t*)pd->data;
                if (strcmp(d->User_ID, inputID) == 0) {
                    strcpy(editDriverWin.controls[7].text, d->IDNum);
                    strcpy(editDriverWin.controls[9].text, d->DriverPhone);
                    // 复原下拉菜单格式
                    char temp[50];
                    sscanf(d->LicenceType, "%[^,]", temp);
                    sprintf(editDriverWin.controls[11].text, "%s,A1,A2,B1,B2,C1", temp);
                    hasRecord = 1;
                    showPopup(300, 250, 250, 80, "读取成功!");
                    Sleep(1000);
                    break;
                }
                pd = pd->next;
            }

            if (!hasRecord) {
                // 如果没建过档，清空下方输入框让用户手填
                memset(editDriverWin.controls[7].text, 0, sizeof(editDriverWin.controls[7].text));
                memset(editDriverWin.controls[9].text, 0, sizeof(editDriverWin.controls[9].text));
                showPopup(300, 250, 250, 80, "首次建档，请补充详细信息。");
            }
            Sleep(800);
        }

        else if (editDriverWin.current == 12) {
            char* inputID = editDriverWin.controls[2].text;
            char* idCard = editDriverWin.controls[7].text;
            char* phone = editDriverWin.controls[9].text;
            
            // 判空
            if (strlen(inputID) == 0 || strlen(idCard) == 0 || strlen(phone) == 0) {
                showPopup(300, 250, 200, 80, "请将表单填写完整！");
                continue;
            }

            //账号合法性再校验 
            User_t* targetUser = NULL;
            LNode* pNode = userList->next;
            while (pNode) {
                User_t* u = (User_t*)pNode->data;
                if (strcmp(u->ID, inputID) == 0 && u->role == 4) { targetUser = u; break; }
                pNode = pNode->next;
            }
            if (!targetUser) {
                showPopup(300, 250, 300, 80, "账号校验失败，请检查ID！");
                Sleep(1000);
                continue;
            }
            int idValid = 1;
            if (strlen(idCard) != 18) {
                idValid = 0;
            } else {
                for (int i = 0; i < 17; i++) {
                    if (idCard[i] < '0' || idCard[i] > '9') { idValid = 0; break; }
                }
                if (idValid) {
                    char lastChar = idCard[17];
                    if (!( (lastChar >= '0' && lastChar <= '9') || lastChar == 'X' || lastChar == 'x' )) {
                        idValid = 0;
                    }
                }
            }

            if (!idValid) {
                showPopup(300, 250, 350, 80, "身份证格式错误！必须为18位且符合规范");
                Sleep(1000);
                continue;
            }

            char licenceMode[10] = {0};
            sscanf(editDriverWin.controls[11].text, "%[^,]", licenceMode);

            int isUpdate = 0;
            LNode* pd = driverList->next;
            while (pd) {
                Driver_t* d = (Driver_t*)pd->data;
                if (strcmp(d->User_ID, inputID) == 0) {
                    strcpy(d->IDNum, idCard);
                    strcpy(d->DriverPhone, phone);
                    strcpy(d->LicenceType, licenceMode);
                    isUpdate = 1;
                    break;
                }
                pd = pd->next;
            }

            if (!isUpdate) {
                // 插入模式
                Driver_t newDriver = {0};
                strcpy(newDriver.User_ID, inputID);
                strcpy(newDriver.IDNum, idCard);
                strcpy(newDriver.DriverPhone, phone);
                strcpy(newDriver.LicenceType, licenceMode);
                newDriver.DriverStatus = 0; 

                insertTail(driverList, &newDriver, sizeof(Driver_t));
                fileWriteFromLinkList("./data/driver.txt", driverList, sizeof(Driver_t));
			}
            showPopup(300, 250, 200, 80, isUpdate ? "档案修改成功！" : "新档案录入成功！");
            Sleep(1000);
            return 8; 
        }
    }
}
