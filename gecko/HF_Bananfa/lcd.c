#include <stdint.h>
#include "lcd.h"
#include "init_game.h"
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"

#include "segmentlcd.h"
#include "segmentlcd_individual.h"



//a globalis valtozok segitsegevl mukodo megjelenito fuggveny
void vTaskDisplay (void *pvParam  __attribute__((unused)) )
{
  while(1){


    //also 14 szegmenses kijelzo plusz vezerlesehez letrehozott valtozo, kezdetben megszabadulunk a memoria szemettol
    SegmentLCD_LowerCharSegments_TypeDef lowerseg [7] = {0};

    SegmentLCD_AllOff(); //torles, a delayek miatt nem lathatoan "villog"

    for(uint8_t i=0; i<=level; i++)
    {
      SegmentLCD_ARing(i, 1); //nehezsegi szintnek megfelelo szamu szegmens
    }

    //jelenlegi pontszamok megjelenitese
    SegmentLCD_Number(score);
    SegmentLCD_Symbol(LCD_SYMBOL_COL10, 1); //pontszamokhoz kettospont

    if(game_started && !game_finished)  //mar megy a jatek
    {
      //banan megjelenitese
      switch (banana_y)
      {
        case 0:
          lowerseg[banana_x].raw = 0b0000000000000001;  //A szegmens
          break;
        case 1:
          lowerseg[banana_x].raw = 0b000000100000000;   //J szegmens
          break;
        case 2:
          lowerseg[banana_x].raw = 0b001000000000000;   //P szegmens
          break;
      }

      //kosar megjelenitese
      if(banana_x == basket)  //kosar es banan ugyanazon a digit
      {
        lowerseg[banana_x].raw = lowerseg[banana_x].raw |  0b0000000000001000; //mindket szegmenst fel kell villantani
      }
      else
      {
        lowerseg[basket].raw = 0b0000000000001000;    //D szegmens
      }

      SegmentLCD_LowerSegments(lowerseg); //beallitott adatokkal frissites

    }
    else if(game_finished){
      SegmentLCD_Write(" E N D ");
    }
    vTaskDelay(configTICK_RATE_HZ/10); // 100ms
  }
}

