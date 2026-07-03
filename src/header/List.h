#ifndef _LIST_H
#define _LIST_H
#include <stdio.h>
#include <stdlib.h> 

typedef struct node{
	void *data;
	struct node *next;
}LNode;

LNode *initList();
LNode *creatNode(void *data, int size);
void insertTail(LNode *head, void *data, int size);
int insertPos(LNode *head, int pos, void *data);
void *findNode(LNode *head, int pos);
void freeList(LNode *head);
int getListNodeCount(LNode *head);
int deleteNode(LNode *head, int pos);
#endif
