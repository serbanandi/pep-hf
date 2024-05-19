#include <em_usart.h>
#include "uart.h"
#include <FreeRTOS.h>
#include "init_game.h"


/***************************************************************************//**
 * ISRs
 ******************************************************************************/
void USART0_RX_IRQHandler(void) {
  BaseType_t  switchReq;

  switchReq = xTaskResumeFromISR(UART_handle);
  USART_IntClear(UART0, USART_IF_RXDATAV);
  portYIELD_FROM_ISR(switchReq);
}


void vTaskUART (void *pvParam  __attribute__((unused)) ){
  uint8_t data;
  while(1){
      vTaskSuspend(NULL);
      data = USART_Rx(UART0);

      switch (data){
        case 'w': if(!game_started) level = level==7 ? 0 : level+1; //szint novelese, atfordul
          break;
        case 's': if(!game_started) level = level==0 ? 7 : level-1; //szint csökkentés
           break;
        case 'a': if(game_started) basket = basket==0 ? 0 : basket-1; //balra, nem fordul at
          break;
        case 'd': if(game_started) basket = basket==3 ? 3 : basket+1;  //jobbra, nem fordul at
          break;
        case ' ': if(!game_started) game_started = true;  //játék elkezdődött
          break;
        default: //szint megadása számokkal
          if(!game_started){
              if('1'<=data && data <= '8'){
                  level = data-'0'-1;
              }
          }
          break;
      }


  }
};
