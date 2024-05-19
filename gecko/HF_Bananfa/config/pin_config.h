#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

// $[ACMP0]
// [ACMP0]$

// $[ACMP1]
// [ACMP1]$

// $[ADC0]
// [ADC0]$

// $[BU]
// [BU]$

// $[CMU]
// [CMU]$

// $[DAC0]
// [DAC0]$

// $[DBG]
// DBG SWO on PF2
#ifndef DBG_SWO_PORT                            
#define DBG_SWO_PORT                             gpioPortF
#endif
#ifndef DBG_SWO_PIN                             
#define DBG_SWO_PIN                              2
#endif
#ifndef DBG_ROUTE_LOC                           
#define DBG_ROUTE_LOC                            0
#endif

// [DBG]$

// $[EBI]
// [EBI]$

// $[ETM]
// [ETM]$

// $[GPIO]
// [GPIO]$

// $[HFXO]
// [HFXO]$

// $[I2C0]
// [I2C0]$

// $[I2C1]
// [I2C1]$

// $[LCD]
// [LCD]$

// $[LESENSE]
// [LESENSE]$

// $[LETIMER0]
// [LETIMER0]$

// $[LEUART0]
// [LEUART0]$

// $[LEUART1]
// [LEUART1]$

// $[LFXO]
// [LFXO]$

// $[PCNT0]
// [PCNT0]$

// $[PCNT1]
// [PCNT1]$

// $[PCNT2]
// [PCNT2]$

// $[PRS.CH0]
// [PRS.CH0]$

// $[PRS.CH1]
// [PRS.CH1]$

// $[PRS.CH2]
// [PRS.CH2]$

// $[PRS.CH3]
// [PRS.CH3]$

// $[TIMER0]
// [TIMER0]$

// $[TIMER1]
// [TIMER1]$

// $[TIMER2]
// [TIMER2]$

// $[TIMER3]
// [TIMER3]$

// $[UART0]
// UART0 RX on PE1
#ifndef UART0_RX_PORT                           
#define UART0_RX_PORT                            gpioPortE
#endif
#ifndef UART0_RX_PIN                            
#define UART0_RX_PIN                             1
#endif
#ifndef UART0_ROUTE_LOC                         
#define UART0_ROUTE_LOC                          1
#endif

// UART0 TX on PE0
#ifndef UART0_TX_PORT                           
#define UART0_TX_PORT                            gpioPortE
#endif
#ifndef UART0_TX_PIN                            
#define UART0_TX_PIN                             0
#endif

// [UART0]$

// $[UART1]
// [UART1]$

// $[USART0]
// [USART0]$

// $[USART1]
// [USART1]$

// $[USART2]
// [USART2]$

// $[USB]
// [USB]$

// $[CUSTOM_PIN_NAME]
#ifndef _PORT                                   
#define _PORT                                    gpioPortA
#endif
#ifndef _PIN                                    
#define _PIN                                     0
#endif

// [CUSTOM_PIN_NAME]$

#endif // PIN_CONFIG_H

