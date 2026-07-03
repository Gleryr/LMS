#ifndef _USER_H
#define _USER_H
#include "../header/List.h"
#include "../header/control.h"
struct User_t{
	char ID[10];
	char name[10];
	char pwd[10];
	int role;//用户角色 0->管理员, 1->仓储员, 2->调度员, 3->客服, 4->司机 
	int state;//用户状态 
	
};

struct Driver_t {
    char User_ID[10];
    char IDNum[20];         // 身份证号
    char DriverPhone[15];   // 联系电话
    char LicenceType[10];   // 驾驶证类型
    int  DriverStatus;      // 0->空闲 1->运输中 2->请假
};

void regUser(const char name[],const char pwd[], int role, LNode *userList);


#endif
