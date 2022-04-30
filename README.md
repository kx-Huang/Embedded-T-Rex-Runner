# Embedded-T-Rex-Runner
Google Chrome's Dinosaur T-rex Runner Game but an Embedded Version

## Project Description
This project is a course project in CS/ECE353 Microprocessor Systems at University of Wisconsin-Madison.

## Game Rules
Basic Google Chrome's dinosaur T-rex runner game rules with following addional features:
  - Button: instant vertical jump (up/down)
  - Joystick: control dinosaur's horizontal positions (move left/right)
  - Accelerometer: contol dinosaur's running speed (tilt left/right)

## Hardware
- MSP-EXP432P401R with MKII Boosterpack from Texas Instruments.
- Peripherals:
  - Joystick
  - Button
  - LCD Display
  - Accelerometer
- Working System Clock: `48 MHz`

## Software
- Programming Language: `C`
- Operating system transplant: `FreeRTOS`
- Random cactus generation: Hardware noise converted by `ADC14` during periodic interrupts
- LCD update: handled with message queue `Queue_LCD`
  1. Future frame buffers are calculated in advanced and enqueued from `Task_Game`
  2. Current frame dequeued in `Task_LCD` to update LCD display
  3. 20ms delay is necessary in `Task_Game` to handle button/joystick/accelerometer data, otherwise delay would be observable.
- ACD notification:
  1. `Task_XX_Timer`: activate ADC to generate ADC interruption to step into interrupt handler `ADC14_IRQHandler`
  2. `ADC14_IRQHandler`: read ADC values sequence and turn on task notification for `Task_XX_Bottom_Half`
  3. `Task_XX_Bottom_Half`: waken by `ADC14_IRQHandler` and update joystick/accelerometer status
