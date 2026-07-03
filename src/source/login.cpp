#include "../header/login.h"

extern LNode *userList; 
int login(const char *inputName, const char *inputPwd){
    // 遍历链表
    LNode *p = userList->next;
    while (p != NULL)
    {
        User_t *user = (User_t*)p->data;

        // 用户名匹配
        if (strcmp(user->name, inputName) == 0)
        {
            // 密码匹配
            if (strcmp(user->pwd, inputPwd) == 0)
            {
                if (user->state == 1) {
                    return 1;
                } else {
                    return -1;
                }
            }
            else {
                return -2;
            }
        }
		else{
			return 0; 
		} 

        p = p->next;
    }

    return 0; // 用户名不存在
}
