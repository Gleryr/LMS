#ifndef _TIME_H
#define _TIME_H
#include <stdio.h>
#include <time.h>
struct DataTime{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	
	
};

DataTime getDataTime();
#endif
