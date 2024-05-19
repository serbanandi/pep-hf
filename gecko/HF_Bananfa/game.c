#include <stdio.h>
#include <stdlib.h>
#include <FreeRTOS.h>
#include <init_game.h>
#include <task.h>
#include "em_usart.h"


void vTaskGame(void *pvParam  __attribute__((unused)) ) {
  while(1){
    if(game_started && !game_finished){
      if(score/100 + score%100 < 25)
        {
          switch (banana_y)
          {
            case 3:         //banan leert
              if(banana_x == basket)  //elkaptuk
                score = score+100;
              else          //nem kaptuk el
                score = score + 1;
              //banana_y = 4;     //virtualis allapot hogy csak egyszer noveljuk a pontokat
              break;
            case 4:
              banana_x = rand()%4;    //random ertek felvetele
              break;
            default: break;
          }
          banana_y = banana_y == 4 ? 0 : banana_y+1;
      }
      else{
          game_finished = true;
          uint8_t points = score/100;
          USART_Tx(UART0, 'e');
          USART_Tx(UART0, '0'+points/10);
          USART_Tx(UART0, '0'+points%10);
          USART_Tx(UART0, 'd');
          USART_Tx(UART0, '0'+level+1);

      }
    }
    vTaskDelay(configTICK_RATE_HZ-level*(configTICK_RATE_HZ/10)); // 1s - level*100ms
  }
}
