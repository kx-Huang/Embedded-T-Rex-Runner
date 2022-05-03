# Embedded-T-Rex-Runner ![](https://visitor-badge.glitch.me/badge?page_id=kx-Huang.Embedded-T-Rex-Runner&left_color=gray&right_color=blue)
Google Chrome's Dinosaur T-rex Runner Game but an Embedded Version

![Demo](img/demo.jpg)

## Project Description
This project is a course project in *CS/ECE353 Microprocessor Systems* [@University of Wisconsin-Madison](https://www.wisc.edu). In general, we implement an embedded version of Google Chrome's T-rex Runner with some more interesting features using `FreeRTOS`.

## Game Rules
Basic Google Chrome's dinosaur T-rex runner game rules with following addional features:
  - Cactus: generate randomly with hardware noise picked up in `ADC`
  - Button: instant vertical jump (up/down)
  - Joystick: control dinosaur's horizontal positions (move left/right)
  - Light sensor: play without button (wave hand in front of light sensor to jump)
  - Accelerometer: contol dinosaur's running speed (tilt left/right)

## Hardware
- MSP-EXP432P401R with MKII Boosterpack from Texas Instruments.
- Peripherals:
  - LED
  - Button
  - Buzzer
  - Joystick
  - LCD Display
  - Light sensor
  - Accelerometer
- Working System Clock: `48 MHz`

## Software
- Programming Language: `C`
- IDE: `Code Composer Studio`
- Operating system transplant: `FreeRTOS`
  - Download source code from [FreeRTOS official website](https://www.freertos.org/a00104.html)
  - Version: `FreeRTOS 202112.00`
  - include path:
    - ${PROJECT_LOC}/FreeRTOS/Source/include
    - ${PROJECT_LOC}/FreeRTOS/Source/portable/CCS/ARM_CM4F
- Peripheral Communication Protocal: `SPI` and `i2c`

## Implementation Details
- Random cactus generation: Hardware noise converted by `ADC14` during periodic interrupts
- LCD update: handled with message queue `Queue_LCD`
  1. Future frame buffers are calculated in advanced and enqueued from `Task_Game`
  2. Current frame dequeued in `Task_LCD` to update LCD display
  3. 20ms delay is necessary in `Task_Game` to handle button/joystick/accelerometer data, otherwise delay would be observable.
  4. Dataflow controls by SPI synchronous communication protocol
- ACD notification:
  1. `Task_XX_Timer`: activate ADC to generate ADC interruption to step into interrupt handler `ADC14_IRQHandler`
  2. `ADC14_IRQHandler`: read ADC values sequence and turn on task notification for `Task_XX_Bottom_Half`
  3. `Task_XX_Bottom_Half`: waken by `ADC14_IRQHandler` and update joystick/accelerometer status
