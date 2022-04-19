/**
 * @file rtc.h
 * Библиотека для работы со встроенными часами реального времени.
 */

#ifndef RTC_H
#define	RTC_H

#include <stm32f10x.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
//#include <sys/time.h>
#include "errors/errors.h"
#include "defs/defs.h"


typedef struct
{
    uint8_t RTC_Hours;
    uint8_t RTC_Minutes;
    uint8_t RTC_Seconds;
    uint8_t RTC_Date;
    uint8_t RTC_Wday;
    uint8_t RTC_Month;
    uint16_t RTC_Year;
} RTC_DateTimeTypeDef;

unsigned char RTC_Init(void);
void RTC_GetDateTime(uint32_t RTC_Counter, RTC_DateTimeTypeDef* RTC_DateTimeStruct);
uint32_t RTC_GetRTC_Counter(RTC_DateTimeTypeDef* RTC_DateTimeStruct) ;
//void RTC_GetMyFormat(RTC_DateTimeTypeDef* RTC_DateTimeStruct, char * buffer) ;


#endif	/* RTC_H */
