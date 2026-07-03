#include "../../header/Win/analysiswin.h" 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>  
#define TO_INT(y, m, d) ((y) * 10000 + (m) * 100 + (d))

extern LNode *orderList;
extern IMAGE g_bg;

void exportOrderStatTXT(const char* dateRangeStr, int total, int completed, float rate) {
    FILE* fp = fopen("Order_Statistics_Report.txt", "w");
    if (fp == NULL) return;
    fprintf(fp, "=================================\n");
    fprintf(fp, "        物流订单统计分析报表       \n");
    fprintf(fp, "=================================\n");
    fprintf(fp, "%s\n", dateRangeStr);
    fprintf(fp, "---------------------------------\n");
    fprintf(fp, "累计订单总数: %d 单\n", total);
    fprintf(fp, "已完成订单数: %d 单\n", completed);
    fprintf(fp, "整体订单完成率: %.2f %%\n", rate);
    fprintf(fp, "=================================\n");
    fclose(fp);
}

int analysiswin() {
    WINDOW_T analysiswin = {100, 80, 600, 420, WHITE, 10, {
        {100, 80, 100, 40, "今日", CYAN, LIGHTCYAN, WHITE, BUTTON, 0},   
        {230, 80, 100, 40, "本周", WHITE, LIGHTCYAN, BLACK, BUTTON, 0},  
        {360, 80, 100, 40, "本月", WHITE, LIGHTCYAN, BLACK, BUTTON, 0},  
        {100, 140, 500, 30, "================= 订单统计结果 =================", WHITE, WHITE, BLACK, LABEL, 0}, 
        {100, 190, 400, 30, "统计时间: ", WHITE, LIGHTCYAN, BLACK, LABEL, 0}, 
        {150, 230, 300, 30, "总订单数: 0 单", WHITE, WHITE, RED, LABEL, 0},     
        {150, 270, 300, 30, "已完成数: 0 单", WHITE, WHITE, RED, LABEL, 0},     
        {150, 310, 300, 30, "完成比例: 0.00 %", WHITE, WHITE, RED, LABEL, 0},   
        {120, 350, 150, 45, "导出TXT报表", CYAN, LIGHTCYAN, BLACK, BUTTON, 0}, 
        {330, 350, 150, 45, "返  回", CYAN, LIGHTCYAN, WHITE, BUTTON, 0}     
    }};

    int currentMode = 0;
    int needRecalculate = 1; 
    int totalOrders = 0;
    int completedOrders = 0;
    float completionRate = 0.0f;
    char dateRangeStr[100] = {0};

    // 时间计算初始化
    DataTime now = getDataTime(); 
    int todayInt = TO_INT(now.year, now.month, now.day);

    time_t raw_t = time(NULL); 
    struct tm tm_calc = *localtime(&raw_t);
    
    // 计算本周范围
    int wday = (tm_calc.tm_wday == 0) ? 7 : tm_calc.tm_wday; 
    tm_calc.tm_mday -= (wday - 1); 
    mktime(&tm_calc); 
    int mondayInt = TO_INT(tm_calc.tm_year + 1900, tm_calc.tm_mon + 1, tm_calc.tm_mday);
    tm_calc.tm_mday += 6;
    mktime(&tm_calc);
    int sundayInt = TO_INT(tm_calc.tm_year + 1900, tm_calc.tm_mon + 1, tm_calc.tm_mday);

    // 计算本月范围
    tm_calc = *localtime(&raw_t);
    tm_calc.tm_mday -= 30;
    mktime(&tm_calc);
    int monthStartInt = TO_INT(tm_calc.tm_year + 1900, tm_calc.tm_mon + 1, tm_calc.tm_mday);

    while (1) {
        if (needRecalculate) {
            for(int i=0; i<3; i++) {
                analysiswin.controls[i].bgColor1 = (currentMode == i) ? CYAN : WHITE;
                analysiswin.controls[i].textColor = (currentMode == i) ? WHITE : BLACK;
            }

            // 更新
            if (currentMode == 0){
            	sprintf(dateRangeStr, "统计时间: %d%02d%02d (今日)", now.year, now.month, now.day);
			} 
            else if (currentMode == 1){
            	sprintf(dateRangeStr, "统计时间: %d 至 %d (本周)", mondayInt, sundayInt);
			} 
            else{
            	sprintf(dateRangeStr, "统计时间: %d 至 %d(三十天内)", monthStartInt, todayInt);
			} 
            strcpy(analysiswin.controls[4].text, dateRangeStr);

            // 统计逻辑
            totalOrders = 0; completedOrders = 0;
            LNode* p = orderList->next;
            while (p) {
                Order_t* o = (Order_t*)p->data;
                char dateStr[9] = {0};
                strncpy(dateStr, o->ID + 2, 8);
                int orderDateInt = atoi(dateStr); 

                int isMatch = 0;
                if (currentMode == 0 && orderDateInt == todayInt) isMatch = 1;
                else if (currentMode == 1 && orderDateInt >= mondayInt && orderDateInt <= sundayInt) isMatch = 1;
                else if (currentMode == 2 && orderDateInt >= monthStartInt && orderDateInt <= todayInt) isMatch = 1;

                if (isMatch) {
                    totalOrders++;
                    if (o->status == 5) completedOrders++;
                }
                p = p->next;
            }
            completionRate = (totalOrders > 0) ? ((float)completedOrders / totalOrders * 100.0f) : 0.0f;
            sprintf(analysiswin.controls[5].text, "总订单数: %d 单", totalOrders);
            sprintf(analysiswin.controls[6].text, "已完成数: %d 单", completedOrders);
            sprintf(analysiswin.controls[7].text, "完成比例: %.2f %%", completionRate);
            needRecalculate = 0;
        }

        putimage(0, 0, &g_bg);
        window_show(analysiswin);
        analysiswin = window_run(analysiswin);

        if (analysiswin.current >= 0 && analysiswin.current <= 2) {
            currentMode = analysiswin.current;
            needRecalculate = 1;
        } 
        else if (analysiswin.current == 8) { 
            exportOrderStatTXT(dateRangeStr, totalOrders, completedOrders, completionRate);
            showPopup(300, 250, 250, 80, "报表已导出！");
            Sleep(1000);
        }
        else if (analysiswin.current == 9) return 1; 
    }
}
