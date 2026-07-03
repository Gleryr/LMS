
#include "../header/user.h"
#include "../header/file.h"

void regUser(const char name[],const char pwd[], int role, LNode *userList){
	LNode *p = userList->next;
	int i = 0; 
	char ID[10] = {0};
	User_t *newUser = (User_t *)malloc(sizeof(User_t));
	if (newUser == NULL){
		printf("开辟空间失败!");
		return;
	}
	while(p != NULL){
		User_t *user = (User_t*)p->data;
		if (strcmp(user->name, name) == 0)
        {
        	showPopup(300, 220, 240, 80, "用户名重复!");
			Sleep(2000);
            return;
        }
		p = p->next;
	}
	strcpy(newUser->name, name);
	strcpy(newUser->pwd, pwd);
	newUser->role = role;
	newUser->state = 1;
	i = getListNodeCount(userList);
	sprintf(ID, "%d", 10001+i);
	strcpy(newUser->ID, ID);
	printf("%s", newUser->name); 
	insertTail(userList, newUser, sizeof(User_t));
	fileWriteFromLinkList("./data/user.txt", userList, sizeof(User_t));
}



