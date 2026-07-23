#ifndef _APP_USER_H
#define _APP_USER_H

#include <stdint.h>
typedef struct {
    uint32_t  ErrorCnt;         
    uint32_t  BusyCnt;
    uint32_t TimeoutCnt;

} I2C_ErrorCnt_t;

#endif
