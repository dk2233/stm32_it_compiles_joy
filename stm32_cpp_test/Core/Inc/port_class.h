#ifndef PORT_CLASS_H
#define PORT_CLASS_H

#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include <cstdint>

class Ports {
    private:
       GPIO_TypeDef *port;
    public: 
        Ports(GPIO_TypeDef *port): port{port} {
        };

        void Toggle(uint16_t pin) { 
            HAL_GPIO_TogglePin(port, pin);
        }

        void Off(uint16_t pin) {
            HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
        }

        void On(uint16_t pin) {
            HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
        }


        GPIO_PinState Read(uint16_t pin) {
            return HAL_GPIO_ReadPin(port, pin );
        }
};

#endif
