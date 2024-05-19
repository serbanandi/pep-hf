#ifndef UART_H_
#define UART_H_

void vTaskUART (void *pvParam  __attribute__((unused)) );
void USART0_RX_IRQHandler(void);

#endif /* UART_H_ */
