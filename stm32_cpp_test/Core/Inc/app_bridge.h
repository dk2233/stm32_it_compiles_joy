#ifndef APP_BRIDGE_H
#define APP_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

    

void App_Init(void);

void App_Loop(void);

void App_OnTimerTick(void);

#ifdef __cplusplus
}
#endif

#endif // APP_BRIDGE_H
