/*
 * task_accel.h
 *
 *  Created on: Apr 29, 2022
 *      Author: Alex North, Kexuan Huang
 */

#ifndef TASK_ACCEL_H_
#define TASK_ACCEL_H_

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

// Possible states of the joystick.
typedef enum Accel_States
{
    TILT_LEFT,
    FLAT,
    TILT_RIGHT,
} tilt_t;

// Task Button Handle
extern TaskHandle_t Task_Accel_Handle;
extern TaskHandle_t Task_Accel_Timer_Handle;

extern volatile tilt_t accel_data;

// private functions (only called in task_joystick.c)
void _accel_init();

// Task Joystick
void Task_Accel_Timer(void *pvParameters);
void Task_Accel_Bottom_Half(void *pvParameters);

// Called in main.c
void Accel_init();

#endif /* TASK_BUTTON_H_ */
