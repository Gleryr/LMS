#include "../header/time.h"

DataTime getDataTime(){
	DataTime datatime = {0};
	time_t rawtime = time(NULL);
	struct tm *local_time = localtime(&rawtime);
	
	datatime.year=local_time->tm_year + 1900;
	datatime.month=local_time->tm_mon + 1;
	datatime.day=local_time->tm_mday;
	datatime.hour=local_time->tm_hour;
	datatime.minute=local_time->tm_min;
	datatime.second=local_time->tm_sec;
	
	return datatime;
}
