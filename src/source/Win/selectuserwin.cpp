#include "../../header/Win/selectuserwin.h"
#include "../../header/control.h"
#include "../../header/List.h"
#include "../../header/user.h"
#include <string.h>

extern LNode *userList;
extern IMAGE g_bg;

const char* getRoleName(int role) {
    switch(role) {
        case 0: return "管理员";
        case 1: return "仓储员";
        case 2: return "调度员";
        case 3: return "客服";
        case 4: return "司机";
        default: return "未知角色";
    }
}

const char* getStatusName(int status) {
    if(status == 1) return "可用";
    return "不可用";
}

int selectuserWin() {
    WINDOW_T selectuserWin = {50, 100, 700, 450, WHITE, 5,
        {
            {80, 130, 120, 40, "用户名搜索：", BLACK, BLACK, BLACK, LABEL, 0},
            {200, 120, 200, 50, "", CYAN, LIGHTCYAN, BLACK, EDIT, 1}, 
            {400, 120, 100, 50, "查询", CYAN, DARKGRAY, WHITE, BUTTON, 0},
            {500, 120, 100, 50, "退出", CYAN, DARKGRAY, WHITE, BUTTON, 0},
            {80, 500, 600, 30, "操作说明：左右方向键翻页，回车键查询", BLACK, WHITE, DARKGRAY, LABEL, 0}
        }
    };

    Table_t table = {0};
    table.x = 100; 
    table.y = 220;
    table.width = 500; 
    table.height = 180;
    table.row = 4;
    table.col = 4;
    strcpy(table.text[0], "ID"); 
    strcpy(table.text[1], "姓名");
    strcpy(table.text[2], "角色"); 
    strcpy(table.text[3], "状态");

    int pageNow = 1;
    char searchKey[20] = {0};

    while (1) {
        //符合条件的过滤总数 
        int filteredCount = 0;
        LNode* tempP = userList->next;
        while (tempP != NULL) {
            User_t* u = (User_t*)tempP->data;
            if (strlen(searchKey) == 0 || strstr(u->name, searchKey)) {
                filteredCount++;
            }
            tempP = tempP->next;
        }

        // 计算总页数并校验当前页码
        int pageCount = (filteredCount + 2) / 3; 
        if(pageCount == 0) pageCount = 1;
        if(pageNow > pageCount) pageNow = pageCount;

        // 根据校验后的pageNow
        memset(table.data, 0, sizeof(table.data));
        LNode* p = userList->next;
        int count = 0;
        int skipped = 0;
        int skipLimit = (pageNow - 1) * 3;

        while (p != NULL && count < 3) {
            User_t* u = (User_t*)p->data;
            if (strlen(searchKey) == 0 || strstr(u->name, searchKey)) {
                if (skipped < skipLimit) {
                    skipped++;
                } else {
                    strncpy(table.data[count][0], u->ID, 19);
                    strncpy(table.data[count][1], u->name, 19);
                    strncpy(table.data[count][2], getRoleName(u->role), 19); 
                    strncpy(table.data[count][3], getStatusName(u->state), 19);
                    count++;
                }
            }
            p = p->next;
        }
        table.count = count;

        // 绘制展示
        putimage(0, 0, &g_bg); // 别忘了背景重绘
        window_show(selectuserWin);
        table_show(table, pageNow, pageCount);
        
        selectuserWin = window_run(selectuserWin);

        // 处理交互
        if (selectuserWin.current == -1 ) {
            if (pageNow > 1) pageNow--;
            else { showPopup(300, 250, 200, 80, "已经是第一页了"); Sleep(800); }
        }
        else if (selectuserWin.current == -2 ) {
            if (pageNow < pageCount) pageNow++;
            else { showPopup(300, 250, 200, 80, "已经是最后一页了"); Sleep(800); }
        }
        else if (selectuserWin.current == 2) { 
            strncpy(searchKey, selectuserWin.controls[1].text, 19);
            pageNow = 1;
        }
        else if (selectuserWin.current == 3) { // 退出
            return 4;
        }
    }
    return 0;
}
