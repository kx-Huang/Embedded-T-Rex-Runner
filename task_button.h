/*
 * task_button.h
 *
 *  Created on: Apr 27, 2022
 *      Author: Kexuan Huang
 */

#ifndef TASK_BUTTON_H_
#define TASK_BUTTON_H_

// MSP library
#include "msp.h"
#include "msp432p401r.h"

// C library
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// RTOS library
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

// Task Button Handle
extern TaskHandle_t Task_Button_Handle;

// global variable indicating button status
volatile bool button_pressed;

// private functions (only called in task_button.c)
void _MKII_S2_Init(void);
bool _MKII_S2_pressed(void);
bool _MKII_S2_debounce(void);

// Task Button
void Task_Button(void *pvParameters);

// Called in main.c
void Button_init();

#endif /* TASK_BUTTON_H_ */
