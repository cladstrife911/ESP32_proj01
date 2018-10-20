#include "log.h"
#include <stdio.h>

void LOG_vidTest(LOG_tenuLogLevel LogLevel)
{
	switch(LogLevel)
	{
	case LOG_enuLvlDEBUG:
		printf("DEBUG\r\n");
		break;
	case LOG_enuLvlINFO:
		printf("INFO\r\n");
		break;
	default:
		printf("DEFAULT\r\n");
		break;
	}
}
