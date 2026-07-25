#include "main.h" // dla definicji pinów i htim1
#include "port_class.h"
#include <cstddef>
#include <malloc.h>
#include <cstdio>
#include <stm32f1xx_hal.h>
#include <usbd_cdc_if.h>
#include "app_bridge.h"
#include <vector>

static uint32_t timer_ticks = 0;
static uint32_t timer2_ticks = 0;

std::vector<int> *vec;

Ports LedPort(LED0_GPIO_Port) ;  

void PrintMallinfo();





// Nadpisujemy funkcję słabą z biblioteki HAL
extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    // Sprawdzamy, czy przerwanie pochodzi z właściwego Timera
    if (htim->Instance == TIM1) {
        timer_ticks++;
        
        if (timer_ticks >= LED0_TOGGLE_PERIOD) {
            timer_ticks = 0;
            LedPort.Toggle( LED0_Pin); // Zmiana stanu diody
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

void PrintMallinfo() {
    struct mallinfo mi = mallinfo();

    printf("--- STATYSTYKI HEAP ---\r\n");
    printf("Calkowity przydzielony Heap: %d B\r\n", mi.arena);    // Całkowita przestrzeń pobrana z systemu
    printf("Pamiac aktualnie uzywana:   %d B\r\n", mi.uordblks); // Przydzielone przez malloc/new
    printf("Pamiec wolna na stercie:     %d B\r\n", mi.fordblks); // Zwolnione przez free/delete (gotowe do ponownego użycia)
}

extern "C" int _write(int file, char *ptr, int len) {
    (void)file;
    
    uint8_t result;

    do {
        result = CDC_Transmit_FS(reinterpret_cast<uint8_t*>(ptr), static_cast<uint16_t>(len));
    } while (result == USBD_BUSY); // Ponawiaj próbę, dopóki USB jest zajęte!

    if (result != USBD_OK) {
        // Jeśli wystąpił inny błąd (np. kabelek wypięty), przerwij
        return -1; 
    }

    return len;
}

void class_init()
{


}

/*
 * simple methods to connect C++ with C units
 */
void App_Init(void)
{
    // Wyłączenie buforowania dla stdout - każdy printf natychmiast idzie do portu!
    setvbuf(stdout, NULL, _IONBF, 0);

}

void App_Loop(void)
{

    if (Timer1Pass == 1) {
          Timer1Pass = 0;
          //printf("Toggle LED, current state %d\n", LedPort.Read(LED0_Pin));

          PrintMallinfo();

      }

    if (Timer2Pass == 1) {
        //Timer2Pass = 0; 

        Timer2Pass = Timer2Pass + 1;
        vec = new std::vector<int>(10,0);
        if (vec == NULL) printf("Something wrong \n");
        printf("Allocated new vector in address %p\n", vec);
    }
    else if (Timer2Pass == 3) {

        Timer2Pass = Timer2Pass + 1;
        for(size_t i = 0 ; i < vec->size(); i++)
        {
            printf("%d - %d , ", i, vec->at(i));
        }
        printf("\n");
    }
    else if (Timer2Pass == 5) {
        Timer2Pass = 0;
        printf("delete vec\n");
        delete vec;

        PrintMallinfo();
    }

}


void App_OnTimerTick(void)
{

}
