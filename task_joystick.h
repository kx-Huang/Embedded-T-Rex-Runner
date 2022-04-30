/*
 * task_joystickj.h
 *
 *  Created on: Apr 27, 2022
 *      Author: Kexuan Huang
 */

#ifndef TASK_JOYSTICK_H_
#define TASK_JOYSTICK_H_

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

// Joystick Parameters
#define VOLT_0P85  1056      // 0.85 /(3.3/4096)
#define VOLT_2P50  3103      // 2.50 /(3.3/4096)

// Possible states of the joystick.
typedef enum Joystick_States
{
    JOYSTICK_LEFT,
    JOYSTICK_CENTER,
    JOYSTICK_RIGHT,
} dir_t;

// Task Joystick Handles
extern TaskHandle_t Task_Joystick_Handle;
extern TaskHandle_t Task_Joystick_Timer_Handle;

// Joystick Direction
extern volatile dir_t joystick_dir;

// Get random from joystick hardware noise
extern volatile uint32_t random;

// private functions (only called in task_joystick.c)
void _joystick_init(void);

// Task Joystick
void Task_Joystick_Timer(void *pvParameters);
void Task_Joystick_Bottom_Half(void *pvParameters);

// Called in main.c
void Joystick_init();

#endif /* TASK_JOYSTICK_H_ */
