#ifndef _CONTROL_H
#define _CONTROL_H
#define BUTTON   1
#define EDIT     2
#define EDIT_PWD 3
#define LABEL    4
#define SELECT 	 5 


#include <graphics.h>
#include <conio.h>
#define KEY_UP 		72
#define KEY_DOWN 	80
#define KEY_LEFT	75
#define KEY_RIGHT	77

struct CONTROL_T{
	int x;
	int y;
	int width;
	int height;
	char text[100];//文本内容 
	COLORREF bgColor1;//默认背景颜色 
	COLORREF bgColor2;//选中后背景颜色 
	COLORREF textColor;//文本颜色 
	int type;//控件种类 
	int status;//控件状态 
};

struct WINDOW_T{
	int x;
	int y;
	int width;
	int height;
	COLORREF bgColor;//背景色 
	int count;//控件数量 
	CONTROL_T controls[30];
	int current;//判断焦点 
};

struct Table_t{
	int x;
	int y;
	int width;
	int height;
	int row;//行数
	int col;//列数
	char text[10][20];//首行
	char data[5][10][20];//表的数据
	int count;//实际数据的行数
};

struct Goods_t{
	char ID[10];
	char name[10];
	int type; // 0普通,1易碎,2化工,3冷链
	int nums;
	int weight;
	int volume;
};

struct Order_t{
	char ID[20];
	char custumerID[10];
	char goodsID[10];
	char sendAddress[20];
	char receiveAddress[20];
	int status;//0->待审核, 1->已驳回, 2->待出库, 3->待运输, 4->运输中, 5->已送达, 6->已完成 
	char reason[256]; 
};

//UI显示 
void control_show(CONTROL_T ctrl);
//窗口显示 
void window_show(WINDOW_T win);
//窗口运行驱动
WINDOW_T window_run(WINDOW_T win);
//显示弹窗消息 
void showPopup(int x, int y, int w, int h, const char* text);
//输出表格
void table_show(Table_t table, int pageNow, int pageCount);
#endif
