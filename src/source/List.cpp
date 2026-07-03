#include "../header/list.h"
#include <string.h>
#include "../header/user.h"
//初始化链表 
LNode* initList() {
    LNode* head = (LNode*)malloc(sizeof(LNode));
    if (head == NULL) {
        printf("开辟空间失败!\n");
        exit(-1);
    }
    head->next = NULL;
    head->data = NULL;
    return head;
}

//创建新节点 
LNode* creatNode(void* data, int size) {
    LNode* newNode = (LNode*)malloc(sizeof(LNode));
    newNode->data = malloc(size);
    if (newNode == NULL) {
        printf("开辟空间失败!\n");
        exit(-1);
    }
    memcpy(newNode->data, data, size);
    newNode->next = NULL;
    return newNode;
}

//表尾插入节点 
void insertTail(LNode* head, void* data, int size) {
    LNode* p = head;
    LNode* newNode = creatNode(data, size);

    while (p->next != NULL) {
        p = p->next;
    }

    p->next = newNode;
}

//指定位置插入节点 
int insertPos(LNode* head, int pos, void* data) {
    if (pos < 0) return 0;

    LNode* p = head;
    int i = 0;

    while (p->next != NULL && i < pos) {
        p = p->next;
        i++;
    }

    if (i == pos) {
        LNode* newNode = creatNode(data, sizeof(User_t));
        newNode->next = p->next;
        p->next = newNode;
        return 1;
    }
    return 0;
}

//查找指定位置节点 
void* findNode(LNode* head, int pos) {
    if (pos < 0) return NULL;

    LNode* p = head->next;
    int i = 0;

    while (p != NULL && i < pos) {
        p = p->next;
        i++;
    }

    if (p == NULL) return NULL;
    return p->data;
}

//删除指定位置节点 
int deleteNode(LNode* head, int pos) {
    if (pos < 0) return 0;

    LNode* p = head;
    LNode* q = head->next;
    int i = 0;

    while (q != NULL && i < pos) {
        p = q;
        q = q->next;
        i++;
    }

    if (q == NULL) return 0;

    p->next = q->next;
    free(q->data);
    free(q);
    return 1;
}

//释放链表 
void freeList(LNode* head) {
    LNode* p = head;
    while (head != NULL) {
        p = head;
        head = head->next;
        if (p->data != NULL) {
            free(p->data);
        }
        free(p);
    }
}

//获取节点数量 
int getListNodeCount(LNode* head) {
    int count = 0;
    LNode* p = head->next;
    while (p != NULL) {
        count++;
        p = p->next;
    }
    return count;
}
