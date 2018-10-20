#ifndef LOG_H
#define LOG_H


typedef enum
{
  LOG_enuLvlDEBUG = 0x01,
  LOG_enuLvlINFO = 0x02,
  
}LOG_tenuLogLevel;

void LOG_vidTest(LOG_tenuLogLevel LogLevel);

#endif /*LOG_H*/
