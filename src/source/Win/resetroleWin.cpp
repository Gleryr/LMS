#include "../../header/resetuser.h"
#include <graphics.h>
#include <conio.h>
#include <stdio.h>

int resetroleWin(){
    WINDOW_T resetroleWin = { 255, 180, 290, 240, WHITE, 6,
        {
            {265, 210, 70, 35, "用户名:", WHITE, LIGHTCYAN, BLACK, LABEL, 0},
            {265, 255, 70, 35, "身  份:", WHITE, LIGHTCYAN, BLACK, LABEL, 0},
            {340, 210, 170, 35, "", WHITE, LIGHTCYAN, BLACK, EDIT, 1},
            {340, 255, 170, 35, "请选择数据,管理员,仓储员,调度员,客服,司机", WHITE, LIGHTCYAN, BLACK, SELECT, 0},
            {270, 305, 100, 40, "确认", CYAN, LIGHTCYAN, BLACK, BUTTON, 0},
            {380, 305, 100, 40, "返回", CYAN, LIGHTCYAN, BLACK, BUTTON, 0}
        }
    };
    
    window_show(resetroleWin);
    resetroleWin = window_run(resetroleWin);
    
    if(resetroleWin.current == 4){
        int role = -1; 
        if(strlen(resetroleWin.controls[2].text) == 0){
            showPopup(300, 220, 240, 80, "请输入用户名");
            Sleep(2000);
            return 7; 
        }
        else{
            // --- 关键修复代码开始 ---
            char selectedText[50] = {0};
            // 提取第一个逗号前的内容，比如 "司机,请选择..." 变成 "司机"
            sscanf(resetroleWin.controls[3].text, "%[^,]", selectedText);

            if(strcmp(selectedText, "管理员") == 0){
                role = 0;
            }
            else if(strcmp(selectedText, "仓储员") == 0){
                role = 1;
            }
            else if(strcmp(selectedText, "调度员") == 0){
                role = 2;
            }
            else if(strcmp(selectedText, "客服") == 0){
                role = 3;
            }
            else if(strcmp(selectedText, "司机") == 0){
                role = 4;
            }
            // --- 关键修复代码结束 ---

            if(role == -1){
                showPopup(300, 220, 240, 80, "请选择身份!"); 
                Sleep(2000);
                return 7; 
            }
            
            int i = resetrole(resetroleWin.controls[2].text, role);
            if(i == 1){
                showPopup(300, 220, 240, 80, "修改成功");
                Sleep(1000);
                return 4; 
            }
            else {
                showPopup(300, 220, 240, 80, "修改失败，用户不存在");
                Sleep(2000);
                return 7; 
            }
        }
    }
    else if(resetroleWin.current == 5){
        return 4;
    }
    return 4; // 增加默认返回
}
