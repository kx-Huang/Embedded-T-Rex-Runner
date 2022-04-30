/*
 * task_game.h
 *
 *  Created on: Apr 27, 2022
 *      Author: Kexuan Huang, Alex North
 */

#ifndef TASK_GAME_H_
#define TASK_GAME_H_

// MSP library
#include "msp.h"
#include "msp432p401r.h"

// C library
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// RTOS library
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

// project library
#include <task_joystick.h>
#include <task_button.h>
#include <task_accel.h>
#include <task_lcd.h>
#include <image.h>

// get random number from hardware noise
#include <ADC.h>

// position restriction
#define DINO_X_MIN 24
#define DINO_X_MAX 104
#define DINO_Y_MIN 32
#define DINO_Y_MAX 110
#define CACTUS_X_MAX 120
#define CACTUS_X_MIN 0
#define CACTUS_Y 108

extern TaskHandle_t Task_Game_Handle;

// Task Game
void Task_Game(void *pvParameters);

// Called in main.c
void Game_init();

#endif /* TASK_GAME_H_ */
