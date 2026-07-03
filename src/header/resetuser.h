#ifndef _RESETUSER_H
#define _RESETUSER_H
#include "control.h"
#include "user.h"
#include "Win/resetpwdWin.h"
#ifdef __cplusplus
extern "C" {
#endif

int resetpwd(const char name[]);

int resetrole(const char name[], int role);

#ifdef __cplusplus
}
#endif

#endif
