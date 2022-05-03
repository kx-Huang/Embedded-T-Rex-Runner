#ifndef BUZZER_H_
#define BUZZER_H_

// MSP library
#include "msp.h"
#include "msp432p401r.h"

// C library
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// Buzzer pitch and duration
#define BUTTON_SOUND_PITCH_LOW 40000
#define BUTTON_SOUND_PITCH_MID 30000
#define BUTTON_SOUND_PITCH_HIGH 20000

#define BUTTON_SOUND_DURATION 360000

// called in task_game.c
void turn_Buzzer_On(uint16_t ticks_period);
void turn_Buzzer_Off(void);
void turn_Buzzer_Off(void);
void T32_1_wait_period(uint32_t period);

// called in main.c
void Buzzer_init(void);

#endif /* BUZZER_H_ */
