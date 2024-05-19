#ifndef INIT_GAME_H_
#define INIT_GAME_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <app.h>
#include "app.h"
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

/***************************************************************************//**
 * Global variables
 ******************************************************************************/


extern volatile bool game_started;
extern volatile bool game_finished;
extern volatile uint8_t level;
extern volatile uint16_t score;
extern volatile uint8_t banana_x;
extern volatile uint16_t banana_y;
extern volatile uint8_t basket;


#endif
