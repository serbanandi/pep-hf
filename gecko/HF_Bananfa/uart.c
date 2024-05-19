#include "em_usart.h"
#include "em_gpio.h"
#include "em_cmu.h"

#include "uart.h"
#include <FreeRTOS.h>
#include "init_game.h"


/***************************************************************************//**
 * ISRs
 ******************************************************************************/

uint8_t data;

void UART0_Init(void) {
      // Enable clock for UART0
      CMU_ClockEnable(cmuClock_UART0, true);

      // Configure UART0 TX pin
      GPIO_PinModeSet(gpioPortE, 0, gpioModePushPull, 1);

      // Configure UART0 RX pin
      GPIO_PinModeSet(gpioPortE, 1, gpioModeInput, 0);

      // Configure UART0
      USART_InitAsync_TypeDef uartInit = USART_INITASYNC_DEFAULT;
      uartInit.baudrate = 115200;
      USART_InitAsync(UART0, &uartInit);

      // Route to loc 1, rx tx en
      UART0->ROUTE |= USART_ROUTE_LOCATION_LOC1 | USART_ROUTE_RXPEN | USART_ROUTE_TXPEN;

      // Enable UART0 TX and RX interrupts
      USART_IntEnable(UART0,  USART_IEN_RXDATAV);

      // Enable UART0 interrupt vectors in NVIC
      //NVIC_ClearPendingIRQ(UART0_TX_IRQn);
      //NVIC_EnableIRQ(UART0_TX_IRQn);
      NVIC_ClearPendingIRQ(UART0_RX_IRQn);
      NVIC_EnableIRQ(UART0_RX_IRQn);

      // Redirecting UART
      GPIO_PinModeSet(gpioPortF, 7, gpioModePushPull, 1);

      // Enable UART0
      USART_Enable(UART0, usartEnable);
}

void UART0_RX_IRQHandler(void) {
  BaseType_t  switchReq;
  if (UART0->STATUS & USART_STATUS_RXDATAV)
      data = USART_Rx(UART0);
  USART_IntClear(UART0, USART_IF_RXDATAV);
  switchReq = xTaskResumeFromISR(UART_handle);
  portYIELD_FROM_ISR(switchReq);
}


void vTaskUART (void *pvParam  __attribute__((unused)) ){
  while(1){
      vTaskSuspend(NULL);

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
