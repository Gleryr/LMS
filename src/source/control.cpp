#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include "../header/control.h"

extern IMAGE g_bg; 

void control_show(CONTROL_T ctrl){
    int i = 0;
    char pwdStr[64] = {0};
    
    // 统一设置基础属性
    setbkmode(TRANSPARENT);
    settextcolor(ctrl.textColor); 

    // 计算垂直居中的起始 Y 坐标
    int ty = ctrl.y + (ctrl.height - textheight(ctrl.text)) / 2;

    // 绘制边框和背景
    if(ctrl.type == BUTTON || ctrl.type == EDIT || ctrl.type == EDIT_PWD || ctrl.type == SELECT){
        setfillcolor(ctrl.status == 1 ? ctrl.bgColor2 : ctrl.bgColor1);
        fillrectangle(ctrl.x, ctrl.y, ctrl.x + ctrl.width, ctrl.y + ctrl.height);
    }

    // 分类型输出文本
    if(ctrl.type == LABEL || ctrl.type == EDIT){
        outtextxy(ctrl.x + 5, ty, ctrl.text);
    }
    else if(ctrl.type == BUTTON){
        // 水平和垂直同时居中
        int tx = ctrl.x + (ctrl.width - textwidth(ctrl.text)) / 2;
        outtextxy(tx, ty, ctrl.text);
    }
    else if(ctrl.type == EDIT_PWD){
        for(i = 0; i < (int)strlen(ctrl.text); i++) pwdStr[i] = '*';
        int py = ctrl.y + (ctrl.height - textheight(pwdStr)) / 2;
        outtextxy(ctrl.x + 5, py, pwdStr);
    }
    else if(ctrl.type == SELECT){
        char showText[20] = {0};
        sscanf(ctrl.text, "%[^,]", showText);
        int sy = ctrl.y + (ctrl.height - textheight(showText)) / 2;
        outtextxy(ctrl.x + 5, sy, showText);
        outtextxy(ctrl.x + ctrl.width - 20, sy, "v");
    }
}

void window_show(WINDOW_T win){
    int i = 0;
    //绘制窗口背景 
    setfillcolor(win.bgColor);
    fillrectangle(win.x, win.y, win.x+win.width, win.y+win.height);
    
    //显示所有控件 
    for(i = 0; i < win.count; i++){
        control_show(win.controls[i]);
    }
}

WINDOW_T window_run(WINDOW_T win){
    char ch1, ch2;
    int i = 0;
    int found_active = 0;

    for(int k = 0; k < win.count; k++){
        if(win.controls[k].status == 1) {
            i = k;
            found_active = 1;
            break;
        }
    }
    
    if(!found_active) {
        i = 0;
        while(win.controls[i].type == LABEL && i < win.count){
            i++;
        }
        if(i < win.count) {
            win.controls[i].status = 1; 
            control_show(win.controls[i]); // 刷新显示它的高亮
        }
    }

    while(1){
        ch1 = getch();
        if(ch1 == '\r'){
            if(win.controls[i].type == BUTTON){
                win.current = i;
                return win;
            }
            else if(win.controls[i].type == SELECT){
                char *buf[20] = {0};
                int k = 0;
                int j = 0;
                char tmp[256];
                
                strcpy(tmp, win.controls[i].text);
                char *token = strtok(tmp, ",");
                while(token != NULL){
                    buf[k++] = token;
                    token = strtok(NULL, ",");
                } 
                WINDOW_T selectWin = {
                   win.controls[i].x,
                   win.controls[i].y + win.controls[i].height,
                   win.controls[i].width,
                   win.controls[i].height * (k - 1),
                   LIGHTGRAY
                };

                memset(selectWin.controls, 0, sizeof(selectWin.controls));
                selectWin.count = k-1;
                
                //绘制下拉菜单
                for(j = 1; j <= k-1; j++){
                    CONTROL_T tempCtrl = { win.controls[i].x,
                        win.controls[i].y + win.controls[i].height * j,
                        win.controls[i].width,
                        win.controls[i].height,
                        "",
                        CYAN, LIGHTCYAN, BLACK,
                        BUTTON,
                        0};
                    strcpy(tempCtrl.text, buf[j]);
                    selectWin.controls[j-1] = tempCtrl;
                    if(j == 1){
                        selectWin.controls[j-1].status = 1;
                    }
                }
                
                window_show(selectWin);
                selectWin = window_run(selectWin);
                if(selectWin.current >= 0 && selectWin.current < selectWin.count){
		            char* optionsTail = strchr(win.controls[i].text, ','); 
		            
		            if (optionsTail != NULL) {
		                char newText[256] = {0};
		                sprintf(newText, "%s%s", selectWin.controls[selectWin.current].text, optionsTail);
		                strcpy(win.controls[i].text, newText);
		            }
		        }
                
                // 选项选择完后，焦点跳到下一个控件
                win.controls[i].status = 0;
                do{
                    i++;
                    if(i >= win.count) i = 0;
                }while(win.controls[i].type == LABEL);
                win.controls[i].status = 1;
                
                putimage(0, 0, &g_bg);
                window_show(win);
            }
        }
        else if(ch1 >= 32 && ch1 <= 126){
            if(win.controls[i].type == EDIT || win.controls[i].type == EDIT_PWD){
                int len = strlen(win.controls[i].text);
                if (len < sizeof(win.controls[i].text) - 1) { // 防越界
                    win.controls[i].text[len] = ch1;
                    win.controls[i].text[len + 1] = '\0';
                    control_show(win.controls[i]); // 实时重绘文字
                }
            }
        }
        else if(ch1 == '\b'){
            if(win.controls[i].type == EDIT || win.controls[i].type == EDIT_PWD){
                int len = strlen(win.controls[i].text);
                if(len > 0){
                    win.controls[i].text[len-1] = '\0';
                    control_show(win.controls[i]); // 实时抹除文字
                }
            }
        }
        else if(ch1 == -32 || ch1 == 224){ //判断上下移动 
            ch2 = getch();
            
            if(ch2 == KEY_DOWN){
                win.controls[i].status = 0;
                control_show(win.controls[i]);
                
                do{
                    i++;
                    if(i >= win.count){ 
                        i = 0;
                    }
                }while(win.controls[i].type == LABEL);
                
                win.controls[i].status = 1;
                control_show(win.controls[i]);
            }
            else if(ch2 == KEY_UP){
                win.controls[i].status = 0;
                control_show(win.controls[i]);
                
                do{
                    i--;
                    if (i < 0) i = win.count - 1;
                } while (win.controls[i].type == LABEL);

                win.controls[i].status = 1;
                control_show(win.controls[i]);
            }
            else if(ch2 == KEY_LEFT){
                win.current = -1;
                return win;
            }
            else if(ch2 == KEY_RIGHT){
                win.current = -2;
                return win;
            }
        }
    }
}

void showPopup(int x, int y, int w, int h, const char* text){
    setfillcolor(LIGHTGRAY);
    solidrectangle(x, y, x + w, y + h);
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    int tx = x + (w - textwidth(text)) / 2;
    int ty = y + (h - textheight(text)) / 2;
    outtextxy(tx, ty, text);
}

void table_show(Table_t table, int pageNow, int pageCount){
    int i, j;
    int w = table.width/table.col;
    int h = table.height/table.row; //矩形的宽高 
    //设置单元格底色 
    setfillcolor(WHITE);
    setlinecolor(BLACK);
    settextcolor(BLACK);
    //遍历输出单元格 
    for(i = 0; i < table.row; i++){
        for(j = 0; j < table.col; j++){
            fillrectangle(table.x + w*j, table.y + h*i, table.x + w*j+w, table.y + h*i+h);
            rectangle(table.x + w*j, table.y + h*i, table.x + w*j+w, table.y + h*i+h);
        }
    }
    //输出表头的内容
    for(j = 0; j < table.col; j++){
        outtextxy(table.x + w*j + 5, table.y + (h/2 - 10), table.text[j]);
    } 
    //输出单元格内容
    for(i = 1; i < table.row; i++){
        for(j = 0; j < table.col; j++){
            if(strlen(table.data[i-1][j]) > 0) {
                outtextxy(table.x+w*j+5, table.y+h*i+15, table.data[i-1][j]);   
            }
        }
    }
    
    if(pageCount > 1) {
        char str[50] = {0};
        sprintf(str, "<-上一页 当前第%d页/共%d页 下一页->", pageNow, pageCount);
        outtextxy(table.x+10, table.y+h*table.row+5, str); 
    }
}
