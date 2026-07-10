#include "main.h" // dla definicji pinów i htim1

static volatile uint32_t timer_ticks = 0;

// Nadpisujemy funkcję słabą z biblioteki HAL
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    // Sprawdzamy, czy przerwanie pochodzi z właściwego Timera
    if (htim->Instance == TIM1) {
        timer_ticks++;
        
        if (timer_ticks >= LED0_TOGGLE_PERIOD) {
            timer_ticks = 0;
            HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin); // Zmiana stanu diody
        }
    }
}
