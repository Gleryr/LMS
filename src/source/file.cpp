#include "../header/file.h"

// 读取文件到链表 
int fileReadToLinkList(const char *fileName, LNode *listHead, int elemSize)
{
    if (fileName == NULL || listHead == NULL || elemSize <= 0)
        return 0;

    FILE *fp = fopen(fileName, "rb");
    if (fp == NULL)
        return 0;
    while (1)
    {
        void *data = malloc(elemSize);
        if (data == NULL)
            break;
        memset(data, 0, elemSize);
        if (fread(data, elemSize, 1, fp) != 1)
        {
            free(data);
            break;
        }
        // 尾插链表
        insertTail(listHead, data, elemSize);
    }

    fclose(fp);
    return 1;
}

// 链表写入文件
int fileWriteFromLinkList(const char *fileName, LNode *listHead, int elemSize)
{
    if (fileName == NULL || listHead == NULL || elemSize <= 0)
        return 0;

    FILE *fp = fopen(fileName, "wb");
    if (fp == NULL)
        return 0;

    LNode *p = listHead->next;
    while (p != NULL)
    {
        fwrite(p->data, elemSize, 1, fp);
        p = p->next;
    }

    fclose(fp);
    return 1;
}

//打开文件 
FILE *fileOpen(const char *fileName){
	FILE *fp;
	fp = fopen(fileName, "r+");
	if(fp == 0){
		perror("文件不存在!\n");
		fp = fopen(fileName, "w+");
	}
	printf("文件成功打开!\n"); 
	return fp; 
} 

//获取文件大小 
int fileGetSize(const char *fileName){
	FILE *fp;
	fp = fopen(fileName, "rb");
	if(fp == 0){
		perror("文件打开失败!\n");
		return 0;
	}
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fclose(fp); 
	return size;
}
