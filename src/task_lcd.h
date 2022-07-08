#ifndef TASK_LCD_H_
#define TASK_LCD_H_

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

// LCD parameter
#define LCD_SCK_PORT    P1
#define LCD_SCK_PIN     BIT5

#define LCD_MOSI_PORT   P1
#define LCD_MOSI_PIN    BIT6

#define LCD_CS_PORT     P5
#define LCD_CS_PIN      BIT0

#define LCD_RST_PORT    P5
#define LCD_RST_PIN     BIT7

#define LCD_DC_PORT     P3
#define LCD_DC_PIN      BIT7

// LCD Screen Dimensions
#define LCD_VERTICAL_MAX      132
#define LCD_HORIZONTAL_MAX    132

#define LCD_ORIENTATION_UP    0
#define LCD_ORIENTATION_LEFT  1
#define LCD_ORIENTATION_DOWN  2
#define LCD_ORIENTATION_RIGHT 3

// ST7735 LCD controller Command Set
#define CM_NOP             0x00
#define CM_SWRESET         0x01
#define CM_RDDID           0x04
#define CM_RDDST           0x09
#define CM_SLPIN           0x10
#define CM_SLPOUT          0x11
#define CM_PTLON           0x12
#define CM_NORON           0x13
#define CM_INVOFF          0x20
#define CM_INVON           0x21
#define CM_GAMSET          0x26
#define CM_DISPOFF         0x28
#define CM_DISPON          0x29
#define CM_CASET           0x2A
#define CM_RASET           0x2B
#define CM_RAMWR           0x2C
#define CM_RGBSET          0x2d
#define CM_RAMRD           0x2E
#define CM_PTLAR           0x30
#define CM_MADCTL          0x36
#define CM_COLMOD          0x3A
#define CM_SETPWCTR        0xB1
#define CM_SETDISPL        0xB2
#define CM_FRMCTR3         0xB3
#define CM_SETCYC          0xB4
#define CM_SETBGP          0xb5
#define CM_SETVCOM         0xB6
#define CM_SETSTBA         0xC0
#define CM_SETID           0xC3
#define CM_GETHID          0xd0
#define CM_SETGAMMA        0xE0
#define CM_MADCTL_MY       0x80
#define CM_MADCTL_MX       0x40
#define CM_MADCTL_MV       0x20
#define CM_MADCTL_ML       0x10
#define CM_MADCTL_BGR      0x08
#define CM_MADCTL_MH       0x04

#define HAL_LCD_delay(x)      __delay_cycles(x * 48)

// LCD related functions
extern void Crystalfontz128x128_Init(void);
extern void Crystalfontz128x128_SetDrawFrame(uint16_t x0, uint16_t y0,
                                             uint16_t x1, uint16_t y1);
void HAL_LCD_PortInit(void);
void HAL_LCD_SpiInit(void);
void HAL_LCD_writeCommand(uint8_t command);
void HAL_LCD_writeData(uint8_t data);
void Crystalfontz128x128_SetDrawFrame(uint16_t x0, uint16_t y0, uint16_t x1,
                                      uint16_t y1);
void Crystalfontz128x128_Init(void);

// Data structure for LCD drawing functions
typedef struct LCD_DRAW_PAR
{
    uint16_t x;
    uint16_t y;
    uint16_t width_pixels;
    uint16_t height_pixels;
    const uint8_t *image;
    uint16_t fColor;
    uint16_t bColor;
} LCD_t;

// LCD drawing functions
void lcd_clear_screen(void);
void lcd_draw_image(const LCD_t par);

// Handle for LCD task
extern TaskHandle_t Task_LCD_Handle;

// Queue of LCD frames to display
extern QueueHandle_t Queue_LCD;

// Length of Queue_LCD
#define QUEUE_LCD_LEN 250

// Task LCD
void Task_LCD(void *pvParameters);

// Called in main.c
void LCD_init();

#endif /* TASK_LCD_H_ */
