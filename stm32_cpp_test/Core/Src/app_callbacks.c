#include "main.h" // dla definicji pinów i htim1
#include <stm32f1xx_hal.h>
                  //
static uint32_t timer_ticks = 0;
static uint32_t timer2_ticks = 0;
// Nadpisujemy funkcję słabą z biblioteki HAL
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

    if (htim->Instance == TIM1) {
        timer_ticks++;
        
        if (timer_ticks >= LED0_TOGGLE_PERIOD) {
            timer_ticks = 0;
            Timer1Pass = 1;
        }
    }

    if (htim->Instance == TIM2)
    {
        timer2_ticks++;

        if (timer2_ticks >= LED0_TOGGLE_PERIOD) {
            timer2_ticks = 0;
            Timer2Pass = Timer2Pass + 1;
        }

    }
}
