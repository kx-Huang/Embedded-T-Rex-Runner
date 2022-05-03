#ifndef TASK_LIGHT_H_
#define TASK_LIGHT_H_

#define LIGHT_THRESHOLD_LOW 30.0
#define LIGHT_THRESHOLD_HIGH 500.0

// MSP library
#include "msp.h"
#include "msp432p401r.h"

// C library
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

// RTOS library
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

// Project library
#include <i2c.h>

// Light sensor i2c slave address
#define OPT3001_SLAVE_ADDRESS 0x44

// Light sensor pin and registers
#define OPT_INTERRUPT_PIN 11
#define RESULT_REG 0x00
#define CONFIG_REG 0x01
#define LOWLIMIT_REG 0x02
#define HIGHLIMIT_REG 0x03
#define MANUFACTUREID_REG 0x7E
#define DEVICEID_REG 0x7F

// Possible states of the joystick.
typedef enum Light_Intensity
{
    LIGHT_HIGH,
    LIGHT_MID,
    LIGHT_LOW,
} light_t;

// Task Light Handle
extern TaskHandle_t Task_Light_Handle;

extern volatile light_t light_intensity;

// Private functions (only called in task_light.c)
// opt3001
void _opt3001_init(void);
float _opt3001_read_lux(void);
void _update_light_status(void);
// MKII RGB LED
void _MKII_RGB_LED_init(void);
void _LED_switch(bool red, bool green, bool blue);

// Called in main.c
void Light_init();

#endif /* TASK_LIGHT_H_ */
