#include "main.h" // dla definicji pinów i htim1
#include "stm32f1xx_hal_def.h"
#include <stdint.h>
#include "app_user.h"

static volatile uint32_t timer_ticks = 0;


const char tab_hello[] = "Hello Friend";

static I2C_ErrorCnt_t I2C_ErrorCnt = {0};


// Nadpisujemy funkcję słabą z biblioteki HAL
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    
    // Sprawdzamy, czy przerwanie pochodzi z właściwego Timera
    if (htim->Instance == TIM1) {
        timer_ticks++;
        
        if (timer_ticks >= LED0_TOGGLE_PERIOD) {
            timer_ticks = 0;
            HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin); // Zmiana stanu diody
                                                          
// data - wskaźnik do tablicy z bajtami (np. komendy dla LCD)
// size - ile bajtów chcesz przepchnąć
        HAL_StatusTypeDef status = HAL_I2C_Master_Transmit_DMA(&hi2c1, I2C_SLAVE_ADDR << 1, (uint8_t *)tab_hello, sizeof(tab_hello) - 1);

        if (HAL_ERROR == status ) I2C_ErrorCnt.ErrorCnt++;
        else if (HAL_BUSY == status ) I2C_ErrorCnt.BusyCnt++;
        else if (HAL_TIMEOUT == status) I2C_ErrorCnt.TimeoutCnt++;

        
        }
    }
}
