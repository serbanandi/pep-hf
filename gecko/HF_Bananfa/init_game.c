#include "init_game.h"
#include <stdbool.h>
#include <stdint.h>

volatile bool game_started = false;   //false: nehezseg allitasa, true: jatek
volatile bool game_finished = false;
volatile uint8_t level = 0;       //nehezseg: 0-7
volatile uint16_t score = 0;      //pontszam: /100 - elkapott, %100 - elejtett
volatile uint8_t banana_x = 0;      //banan melyik digiten esik le: 0-3
volatile uint16_t banana_y = 0;     //banan hol tart az esesben: 0-3, 4 (virtualis)
volatile uint8_t basket = 0;      //kosar melyik digiten van
