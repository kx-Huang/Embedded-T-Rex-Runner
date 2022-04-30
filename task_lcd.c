/*
 * task_lcd.c
 *
 *  Created on: Apr 27, 2022
 *      Author: Kexuan Huang
 */

#include "task_lcd.h"

TaskHandle_t Task_LCD_Handle = NULL;

QueueHandle_t Queue_LCD;

/* ****************************************************************************
 * Used to configure the 5 pins that control the LCD interface on the MKII.
 *
 * CLK and MOSI will be configured as their Primary Module Function
 *
 * CS, C/D, and RST will be configured at output pins
 ******************************************************************************/
void HAL_LCD_PortInit(void)
{
    // LCD_SCK
    LCD_SCK_PORT->SEL0 |= LCD_SCK_PIN;
    LCD_SCK_PORT->SEL1 &= ~LCD_SCK_PIN;

    // LCD_MOSI
    LCD_MOSI_PORT->SEL0 |= LCD_MOSI_PIN;
    LCD_MOSI_PORT->SEL1 &= ~LCD_MOSI_PIN;

    // LCD_CS
    LCD_CS_PORT->DIR |= LCD_CS_PIN;

    // LCD_RST
    LCD_RST_PORT->DIR |= LCD_RST_PIN;

    // LCD_DC
    LCD_DC_PORT->DIR |= LCD_DC_PIN;
}

/* ****************************************************************************
 * Used to configure the eUSCIB Interface as a 3-Wire SPI Interface
 *
 ******************************************************************************/
void HAL_LCD_SpiInit(void)
{
    EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_SWRST; // Put eUSCI state machine in reset

    // define the behavior of the eUSCI_B0 as a SPI interface
    EUSCI_B0->CTLW0 = EUSCI_B_CTLW0_CKPH |   // Phase of 1
            EUSCI_B_CTLW0_MSB |   // MSB first
            EUSCI_B_CTLW0_MST |   // Set as SPI master
            EUSCI_B_CTLW0_MODE_0 |   // 3 Pin SPI Mode
            EUSCI_B_CTLW0_SYNC |   // Set as synchronous mode
            EUSCI_B_CTLW0_SSEL__SMCLK |   // Use SMCLK
            //EUSCI_B_CTLW0_STEM      |   // UCxSTE digital output
            EUSCI_B_CTLW0_SWRST;// Remain eUSCI state machine in reset

    // set the SPI Clock to 12MHz.
    // Divide clock speed by 2 (24MHz/2) = 12 MHz
    // fBitClock = fBRCLK/(UCBRx+1).
    EUSCI_B0->BRW = 1;

    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;    // Initialize USCI state machine

    // set the chip select low
    // The chip select will be held low forever! There is only 1 device (LCD)
    // connected to the SPI device, so we will leave it activated all the time.
    LCD_CS_PORT->OUT &= ~LCD_CS_PIN;

    // Set the DC pin high (put it in data mode)
    LCD_DC_PORT->OUT |= LCD_DC_PIN;

}

//*****************************************************************************
//
// Writes a command to the CFAF128128B-0145T.  This function implements the basic SPI
// interface to the LCD display.
//
//*****************************************************************************
void HAL_LCD_writeCommand(uint8_t command)
{
    // Set to command mode -- DC PIN Set to 0
    LCD_DC_PORT->OUT &= ~LCD_DC_PIN;

    // Busy wait while the data is being transmitted. Check the STATW register and see if it is busy
    while (EUSCI_B0->STATW & BIT0 == 1)
    {
        ;
    }
    // Transmit data
    EUSCI_B0->TXBUF = command;

    // Busy wait while the data is being transmitted. Check the STATW register and see if it is busy
    while (EUSCI_B0->STATW & BIT0 == 1)
    {
        ;
    }

    // Set back to data mode, set the DC pin high
    LCD_DC_PORT->OUT |= LCD_DC_PIN;
}

//*****************************************************************************
//
// Writes a data to the CFAF128128B-0145T. This function implements the basic SPI
// interface to the LCD display.
//
//*****************************************************************************
void HAL_LCD_writeData(uint8_t data)
{
    // Busy wait while the data is being transmitted. Check the STATW register and see if it is busy
    while (EUSCI_B0->STATW & BIT0 == 1)
    {
        ;
    }

    // Transmit data
    EUSCI_B0->TXBUF = data;

    // Busy wait while the data is being transmitted. Check the STATW register and see if it is busy
    while (EUSCI_B0->STATW & BIT0 == 1)
    {
        ;
    }
}

//*****************************************************************************
// Code adapted from TI LCD driver library
//*****************************************************************************
void Crystalfontz128x128_SetDrawFrame(uint16_t x0, uint16_t y0, uint16_t x1,
                                      uint16_t y1)
{
    // Write the CM_CASET command followed by the 4 bytes of data
    // used to set the Column start and end rows.
    HAL_LCD_writeCommand(CM_CASET);
    HAL_LCD_writeData(0);
    HAL_LCD_writeData(x0);
    HAL_LCD_writeData(0);
    HAL_LCD_writeData(x1);

    // Write the CM_RASET command followed by the 4 bytes of data
    // used to set the Row start and end rows.
    HAL_LCD_writeCommand(CM_RASET);
    HAL_LCD_writeData(0);
    HAL_LCD_writeData(y0);
    HAL_LCD_writeData(0);
    HAL_LCD_writeData(y1);
}

//*****************************************************************************
// Code adapted from TI LCD driver library
//
//! Initializes the display driver.
//!
//! This function initializes the ST7735 display controller on the panel,
//! preparing it to display data.
//!
//! \return None.
//
//*****************************************************************************
void Crystalfontz128x128_Init(void)
{
    int i;
    int j;
    uint8_t upper8 = 0x00;
    uint8_t lower8 = 0x00;

    HAL_LCD_PortInit();
    HAL_LCD_SpiInit();

    LCD_RST_PORT->OUT &= ~LCD_RST_PIN;
    HAL_LCD_delay(50);

    LCD_RST_PORT->OUT |= LCD_RST_PIN;
    HAL_LCD_delay(120);

    HAL_LCD_writeCommand(CM_SLPOUT);
    HAL_LCD_delay(200);

    HAL_LCD_writeCommand(CM_GAMSET);
    HAL_LCD_writeData(0x04);

    HAL_LCD_writeCommand(CM_SETPWCTR);
    HAL_LCD_writeData(0x0A);
    HAL_LCD_writeData(0x14);

    HAL_LCD_writeCommand(CM_SETSTBA);
    HAL_LCD_writeData(0x0A);
    HAL_LCD_writeData(0x00);

    HAL_LCD_writeCommand(CM_COLMOD);
    HAL_LCD_writeData(0x05);
    HAL_LCD_delay(10);

    HAL_LCD_writeCommand(CM_MADCTL);
    HAL_LCD_writeData(CM_MADCTL_MX | CM_MADCTL_MY | CM_MADCTL_BGR);

    HAL_LCD_writeCommand(CM_NORON);

    Crystalfontz128x128_SetDrawFrame(0, 0, LCD_HORIZONTAL_MAX,
    LCD_VERTICAL_MAX);
    HAL_LCD_writeCommand(CM_RAMWR);

    for (i = 0; i < LCD_VERTICAL_MAX; i++)
    {

        for (j = 0; j < LCD_HORIZONTAL_MAX; j++)
        {
            HAL_LCD_writeData(upper8);
            HAL_LCD_writeData(lower8);
        }
    }

    HAL_LCD_delay(10);

    HAL_LCD_writeCommand(CM_DISPON);

    HAL_LCD_writeCommand(CM_MADCTL);
    HAL_LCD_writeData(CM_MADCTL_MX | CM_MADCTL_MY | CM_MADCTL_BGR);
}

/*******************************************************************************
 * Function Name: lcd_draw_rectangle
 ********************************************************************************
 * Summary: draws a filled rectangle centered at the coordinates set by x, y
 * Returns:
 *  Nothing
 *******************************************************************************/
void lcd_clear_screen(void)
{
    uint16_t x = 0;
    uint16_t y = 0;
    uint16_t width_pixels = 120;
    uint16_t height_pixels = 120;
    uint16_t fColor = 0;

    uint16_t i, j;
    uint16_t x0;
    uint16_t x1;
    uint16_t y0;
    uint16_t y1;

    // Set the left Col to be the center point minus half the width
    x0 = x - (width_pixels / 2);

    // Set the Right Col to be the center point plus half the width
    x1 = x + (width_pixels / 2);

    // Account for a width that is an even number
    if ((width_pixels & 0x01) == 0x00)
    {
        x1--;
    }

    // Set the upper Row to be the center point minus half the height
    y0 = y - (height_pixels / 2);

    // Set the upper Row to be the center point minus half the height
    y1 = y + (height_pixels / 2);

    // Account for a height that is an  number
    if ((height_pixels & 0x01) == 0x00)
    {
        y1--;
    }

    // Set the boundary of the image to draw
    Crystalfontz128x128_SetDrawFrame(x0, y0, x1, y1);

    // Inform the LCD we are going to send image data
    HAL_LCD_writeCommand(CM_RAMWR);

    // Write out the image data
    for (i = 0; i < height_pixels; i++)
    {
        for (j = 0; j < width_pixels; j++)
        {
            HAL_LCD_writeData(fColor >> 8);
            HAL_LCD_writeData(fColor);
        }
    }

}

/*******************************************************************************
 * Function Name: lcd_draw_image
 ********************************************************************************
 * Summary: Prints an image centered at the coordinates set by x, y
 * Returns:
 *  Nothing
 *******************************************************************************/
void lcd_draw_image(const LCD_t par)
{
    uint16_t x = par.x;
    uint16_t y = par.y;
    uint16_t image_width_pixels = par.width_pixels;
    uint16_t image_height_pixels = par.height_pixels;
    uint8_t *image = par.image;
    uint16_t fColor = par.fColor;
    uint16_t bColor = par.bColor;

    uint16_t i, j;
    uint8_t data;
    uint16_t byte_index;
    uint16_t bytes_per_row;
    uint16_t x0;
    uint16_t x1;
    uint16_t y0;
    uint16_t y1;

    x0 = x - (image_width_pixels / 2);
    x1 = x + (image_width_pixels / 2);
    if ((image_width_pixels & 0x01) == 0x00)
    {
        x1--;
    }

    y0 = y - (image_height_pixels / 2);
    y1 = y + (image_height_pixels / 2);
    if ((image_height_pixels & 0x01) == 0x00)
    {
        y1--;
    }

    Crystalfontz128x128_SetDrawFrame(x0, y0, x1, y1);
    HAL_LCD_writeCommand(CM_RAMWR);
    //lcd_set_pos(x0, x1, y0, y1);

    bytes_per_row = image_width_pixels / 8;
    if ((image_width_pixels % 8) != 0)
    {
        bytes_per_row++;
    }

    for (i = 0; i < image_height_pixels; i++)
    {
        for (j = 0; j < image_width_pixels; j++)
        {
            if ((j % 8) == 0)
            {
                byte_index = (i * bytes_per_row) + j / 8;
                data = image[byte_index];
            }
            if (data & 0x80)
            {
                HAL_LCD_writeData(fColor >> 8);
                HAL_LCD_writeData(fColor);
            }
            else
            {
                HAL_LCD_writeData(bColor >> 8);
                HAL_LCD_writeData(bColor);
            }
            data = data << 1;
        }
    }

}

// Task LCD
// receive parameters from Queue_LCD and update LCD screen
void Task_LCD(void *pvParameters)
{
    LCD_t par; // parameters for drawing LCD

    while (1)
    {
        // receive parameters for drawing LCD from queue
        xQueueReceive(Queue_LCD, &par, portMAX_DELAY);

        // draw LCD with received parameters
        lcd_draw_image(par);
    }
}

// initialize LCD and Queue_LCD, called in main.c
void LCD_init()
{
        // Create LCD Queue
        Queue_LCD = xQueueCreate(QUEUE_LCD_LEN, sizeof(LCD_t));

        // ADD CODE to initialize the LCD
        Crystalfontz128x128_Init();

        // Create task for LCD
        xTaskCreate
        (   Task_LCD,
            "Update LCD frame",
            configMINIMAL_STACK_SIZE,
            NULL,
            4,
            &Task_LCD_Handle
        );
}
