/*
 * ADC.c
 *
 *  Created on: Apr 29, 2022
 *      Author: Kexuan Huang
 */

#include <task_accel.h>
#include <task_joystick.h>

/*
 * Interrupt Handler for ADC14.
 * Sends collected data from the Joystick and Accelerometer to their bottom half task.
 */
void ADC14_IRQHandler(void)
{
    uint32_t ACCEL_READ = ADC14->MEM[1]; // Read the accelerometer value
    uint32_t JOYSTICK_X_DIR = ADC14->MEM[0]; // Read the joystick value

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Send a task notification to Task_Accel_Handle
    xTaskNotifyFromISR(Task_Accel_Handle, ACCEL_READ, eSetValueWithOverwrite,
                       &xHigherPriorityTaskWoken);

    // Send a task notification to Task_Joystick_Handle
    xTaskNotifyFromISR(Task_Joystick_Handle, JOYSTICK_X_DIR,
                       eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
}
