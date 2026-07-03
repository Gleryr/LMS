#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

int fileReadToLinkList(const char *fileName, LNode *listHead, int elemSize);

int fileWriteFromLinkList(const char *fileName, LNode *listHead, int elemSize);

FILE *fileOpen(const char *fileName);

int fileGetSize(const char *fileName);
#endif
