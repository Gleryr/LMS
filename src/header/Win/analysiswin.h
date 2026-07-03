#ifndef _ANALYSISWIN_H
#define _ANALYSISWIN_H


#include "../../header/control.h"
#include "../time.h"
#include "../List.h"
#include "../user.h"
int analysiswin();
void exportOrderStatTXT(const char* timeStr, int total, int completed, float rate);
#endif
