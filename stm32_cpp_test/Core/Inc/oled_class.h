#ifndef OLED_CLASS_H
#define OLED_CLASS_H

#include "ssd1306_fonts.h"
#include "ssd1306.h"
#include <string_view>



char init_char[] {"Initialization..."};

class Oled {
    private: 

    public:
        Oled(SSD1306_COLOR bg_color = White) {

            ssd1306_Init();
            ssd1306_Fill(White);
            ssd1306_SetCursor(0, 0);
            ssd1306_WriteString(init_char, Font_16x24, Black);
            ssd1306_UpdateScreen();
            
            ssd1306_Fill(bg_color);
        }

        void OledPrint(const char * str_char, int x, int y, SSD1306_Font_t font, SSD1306_COLOR color) 
        {
            ssd1306_SetCursor(x, y);
            ssd1306_WriteString((char *)str_char, font, color);
            ssd1306_UpdateScreen();
        }

        void OledPrint(std::string_view &str, int x, int y, SSD1306_Font_t font, SSD1306_COLOR color) 
        {
            OledPrint(str.data(), x, y, font, color);
        }

};

#endif
