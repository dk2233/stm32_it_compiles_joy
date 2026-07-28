#include "main.h" // dla definicji pinów i htim1
#include "port_class.h"
#include <cstddef>
#include <malloc.h>
#include <cstdio>
#include <stm32f1xx_hal.h>
#include <usbd_cdc_if.h>
#include "app_bridge.h"
#include <vector>
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "usbd_def.h"
#include "oled_class.h"
#include <string_view>
#include <charconv>
#include <array>
#include <etl/vector.h>
#include <etl/optional.h>


std::vector<int> *vec;
etl::vector<int, 20> vec_etl(0);

Ports LedPort(LED0_GPIO_Port) ;  

void PrintMallinfo();

etl::optional<Oled> screen;
extern USBD_HandleTypeDef hUsbDeviceFS;


#define ONE_LINE_6x8   30u
void PrintMallinfo() {
    std::array<char, ONE_LINE_6x8> message_a;
    message_a.fill(0);

    struct mallinfo mi = mallinfo();

    if (hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED)
    {
        printf("--- STATYSTYKI HEAP ---\r\n");
        printf("Whole Heap: %d B\r\n", mi.arena);    // Całkowita przestrzeń pobrana z systemu
        printf("aktualnie uzywana:   %d B\r\n", mi.uordblks); // Przydzielone przez malloc/new
        printf("wolna na stercie:     %d B\r\n", mi.fordblks); // Zwolnione przez free/delete (gotowe do ponownego użycia)
    }                                                             //

    std::string_view heap1{"used heap:"};
    
    std::copy(heap1.begin() , heap1.end(), message_a.begin());
    size_t offset = heap1.size();

    // 2. WSTAWIAMY LICZBĘ w konkretne miejsce bufora (od 'offset')
    auto [ptr, ec] = std::to_chars(message_a.data() + offset, message_a.data() + message_a.size(), mi.uordblks);
    
    if (ec == std::errc{}) {
        offset = ptr - message_a.data(); // Aktualizujemy długość całkowitą
        
        // Dopisujemy końcówkę
        std::string_view suffix = " bytes";
        std::copy(suffix.begin(), suffix.end(), message_a.data() + offset);
        offset += suffix.size();

        // 3. TWORZYMY STRING_VIEW na gotowy bufor
        std::string_view result(message_a.data(), offset);
    screen->OledPrint(result, 2 , 0 , Font_6x8, Black);
        
}
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


/*
 * simple methods to connect C++ with C units
 */
void App_Init(void)
{
    // Wyłączenie buforowania dla stdout - każdy printf natychmiast idzie do portu!
    setvbuf(stdout, NULL, _IONBF, 0);

     screen.emplace(White);
}

void App_Loop(void)
{

    if (Timer1Pass == 1) {
          Timer1Pass = 0;
          LedPort.Toggle( LED0_Pin); // Zmiana stanu diody
          
          //printf("Toggle LED, current state %d\n", LedPort.Read(LED0_Pin));

          PrintMallinfo();

      }

    if (Timer2Pass == 1) {
        //Timer2Pass = 0; 

        Timer2Pass = Timer2Pass + 1;
        vec = new std::vector<int>(10,0);
        if ((vec == NULL) && (hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED)) printf("Something wrong \n");
    
    if (hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED)    printf("Allocated new vector in address %p\n", vec);
    }
    else if (Timer2Pass == 3) {

        Timer2Pass = Timer2Pass + 1;
        for(size_t i = 0 ; i < vec->size(); i++)
        {
    if (hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED)
            printf("%d - %d , ", i, vec->at(i));
        }
    if (hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED)
        printf("\n");
    }
    else if (Timer2Pass == 5) {
        Timer2Pass = 0;
    if (hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED)   printf("delete vec\n");
        delete vec;

        PrintMallinfo();
    }

}


void App_OnTimerTick(void)
{

}
