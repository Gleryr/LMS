#include "../header/resetuser.h"
#include "../header/List.h"
#include "../header/user.h"
#include "../header/Win/adminsysteam.h"
#include "../header/file.h"
extern LNode *userList; 


int resetpwd(const char name[]){
	LNode *p = userList->next;
	while(p != NULL){
		User_t *user = (User_t *)p->data;
		if(strcmp(user->name, name) == 0){
			strcpy(user->pwd, "888888");
			fileWriteFromLinkList("./data/user.txt", userList, sizeof(User_t));
			showPopup(300, 220, 240, 80, "指定用户密码重置完成!");
			Sleep(2000);
			return 1; 
		}
		p = p->next;
	}
	showPopup(300, 220, 240, 80, "没找到目标用户!");
	Sleep(2000);
	return -1; 
}

int resetrole(const char name[], int role){
	LNode *p = userList->next;
	while(p != NULL){
		User_t *user = (User_t *)p->data;
		if(strcmp(user->name, name) == 0){
			user->role = role;
			fileWriteFromLinkList("./data/user.txt", userList, sizeof(User_t));

			return 1; 
		}
		p = p->next;
	}
	showPopup(300, 220, 240, 80, "没找到目标用户!");
	Sleep(2000);
	return -1; 
}
