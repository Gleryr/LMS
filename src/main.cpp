#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include "header/control.h"
#include "header/Win/loginWin.h"
#include "header/Win/startWin.h"
#include "header/Win/regWin.h"
#include "header/user.h"
#include "header/time.h"
#include "header/List.h"
#include "header/file.h"
#include "header/dispatch.h"
#include "header/storage.h"
#include "header/Win/adminapp.h"
#include "header/Win/adminsysteam.h"
#include "header/Win/adduserWin.h"
#include "header/Win/resetpwdWin.h"
#include "header/Win/resetroleWin.h"
#include "header/Win/ordersys.h"
#include "header/Win/selectuserwin.h"
#include "header/Win/createorderwin.h"
#include "header/Win/selectorderwin.h"
#include "header/Win/auditorder.h"
#include "header/order.h"
#include "header/goods.h"
#include "header/customer.h"
#include "header/Win/ordertrackwin.h"
#include "header/Win/storagewin.h"
#include "header/Win/dispatchwin.h"
#define g_width 	800
#define g_height 	600
char timestr[20] = {0};
IMAGE g_bg;
User_t *g_user = NULL;
LNode *userList;
LNode *orderList;
LNode *storageList;
LNode *goodsList;
LNode *customerList;
LNode *dispatchList;
LNode *driverList;
LNode *vehicleList;
FILE *User_fp;
FILE *Goods_fp;
FILE *Order_fp;
FILE *Customer_fp;
FILE *Storage_fp;
FILE *Driver_fp;
FILE *Dispatch_fp;
FILE *Vehicle_fp;

int main()
{
    initgraph(g_width, g_height);
    loadimage(&g_bg, "./image/LMS.png", g_width, g_height);
    putimage(0, 0, &g_bg);
    settextstyle(20, 0, "智能物流管理系统");
	int (*fun[50])() = {startWin, loginWin, regWin, adminapp, adminsys, adduserWin, resetpwdWin, resetroleWin, orderSys, selectuserWin, createOrdWin, selectorderWin, auditorderWin, 
	resetorderWin, orderTrackWin, storageWin, dispatchWin};//16
	int win_ID = 0;
	DataTime time = getDataTime();
	sprintf(timestr,"%d/%d/%d",time.year,time.month,time.day);
	userList = initList();
	orderList = initList();
	goodsList = initList();
	customerList = initList();
	storageList = initList();
	dispatchList = initList();
	driverList = initList();
	vehicleList = initList();
	User_fp = fileOpen("../bin/data/user.txt");
	Goods_fp = fileOpen("../bin/data/good.txt");
	Order_fp = fileOpen("../bin/data/order.txt");
	Customer_fp = fileOpen("../bin/data/customer.txt");
	Storage_fp = fileOpen("../bin/data/storage.txt");
	Driver_fp = fileOpen("../bin/data/driver.txt");
	Dispatch_fp = fileOpen("../bin/data/dispatch.txt");
	Vehicle_fp = fileOpen("../bin/data/vehicle.txt");
	//如果文件没有数据则存入管理员数据 
	if(fileGetSize("./data/user.txt") == 0){
		User_t* admin = (User_t*)malloc(sizeof(User_t));
        strcpy(admin->ID, "10001");
        strcpy(admin->name, "admin");
        strcpy(admin->pwd, "123456");
        admin->role = 0;
        admin->state = 1;
        insertTail(userList, admin, sizeof(User_t));
//      fileWriteFromLinkList("./data/user.txt", userList, sizeof(User_t));
	}
	fileReadToLinkList("../bin/data/user.txt", userList, sizeof(User_t));
	fileReadToLinkList("../bin/data/good.txt", goodsList, sizeof(Goods_t));
	fileReadToLinkList("../bin/data/order.txt", orderList, sizeof(Order_t));
	fileReadToLinkList("../bin/data/customer.txt", customerList, sizeof(Customer_t));
	fileReadToLinkList("../bin/data/storage.txt", storageList, sizeof(Storage_t));
	fileReadToLinkList("../bin/data/driver.txt", driverList, sizeof(Driver_t));
	fileReadToLinkList("../bin/data/dispatch.txt", dispatchList, sizeof(Dispatch_t));
	fileReadToLinkList("../bin/data/vehicle.txt", vehicleList, sizeof(Vehicle_t));
	while(1){
		cleardevice();
		putimage(0, 0, &g_bg);
		win_ID = fun[win_ID]();
		if(win_ID == -1){
			break;
		} 
	}
	fileWriteFromLinkList("../bin/data/user.txt", userList, sizeof(User_t));
    fileWriteFromLinkList("../bin/data/good.txt", goodsList, sizeof(Goods_t));
	fileWriteFromLinkList("../bin/data/order.txt", orderList, sizeof(Order_t));
    fileWriteFromLinkList("../bin/data/customer.txt", customerList, sizeof(Customer_t));
    fileWriteFromLinkList("../bin/data/storage.txt", storageList, sizeof(Storage_t));
    fileWriteFromLinkList("../bin/data/driver.txt", driverList, sizeof(Driver_t));
    fileWriteFromLinkList("../bin/data/dispatch.txt", dispatchList, sizeof(Dispatch_t));
    fileWriteFromLinkList("../bin/data/vehicle.txt", vehicleList, sizeof(Vehicle_t));
    closegraph();
    return 0;
}

