#include <stdio.h>
#include <app.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "init_game.h"
#include "segmentlcd.h"
#include "lcd.h"
#include "uart.h"
#include "game.h"
#include <em_usart.h>

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
TaskHandle_t display_handle;
TaskHandle_t UART_handle;
TaskHandle_t game_handle;


/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  SegmentLCD_Init(false);

  UART0_Init();

  USART_Tx(UART0, 's'); //start jel küldése a számítógép felé

  xTaskCreate(
      vTaskDisplay,
      "Display",
      configMINIMAL_STACK_SIZE,
      NULL,
      tskIDLE_PRIORITY + 1,
      &display_handle);

  xTaskCreate(
      vTaskGame,
      "Game",
      configMINIMAL_STACK_SIZE,
      NULL,
      tskIDLE_PRIORITY + 2,
      &game_handle);

  xTaskCreate(
        vTaskUART,
        "UART",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 3,
        &UART_handle);
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
}

