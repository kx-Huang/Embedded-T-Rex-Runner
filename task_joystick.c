#include <task_joystick.h>

TaskHandle_t Task_Joystick_Handle = NULL;
TaskHandle_t Task_Joystick_Timer_Handle = NULL;

volatile dir_t joystick_dir = JOYSTICK_CENTER;
volatile uint32_t random = 0;

void _joystick_init(void)
{
    // Configure pin to x-axis Joystick as analog input.
    // P6.0 <-> J1.2 (A15)
    P6->DIR &= ~BIT0;
    P6->SEL0 |= BIT0;
    P6->SEL1 |= BIT0;

    // Configure CTL0 to sample 16-times in pulsed sample mode.
    ADC14->CTL0 = ADC14_CTL0_SHP |  // timer source
                  ADC14_CTL0_SHT02; // 16 samples

    // Configure ADC to return 12-bit values
    ADC14->CTL1 = ADC14_CTL1_RES_2;

    // Associate the x-axis Joystick signal with MEM[0]
    ADC14->MCTL[0] |= ADC14_MCTLN_INCH_15;

    // Enable interrupts AFTER a value is written into MEM[0].
    ADC14->IER0 |= ADC14_IER0_IE0;

    // Enable ADC Interrupt with priority 2.
    NVIC_EnableIRQ(ADC14_IRQn);
    NVIC_SetPriority(ADC14_IRQn, 2);

    // Turn ADC on.
    ADC14->CTL0 |= ADC14_CTL0_ON;

    __enable_irq();
}

// Task Joystick Timer
// Used to start an ADC14 Conversion
void Task_Joystick_Timer(void *pvParameters)
{
    while (1)
    {
        // Start ADC conversion.
        ADC14->CTL0 |= ADC14_CTL0_SC | ADC14_CTL0_ENC;

        // Delay for 10mS using vTaskDelay
        vTaskDelay(pdMS_TO_TICKS(5));
    }

}

/*
 * Joystick Bottom Half Sub-Task
 */
void Task_Joystick_Bottom_Half(void *pvParameters)
{
    uint32_t JOYSTICK_X_DIR = 0;

    while (1)
    {
        // Get/Wait for task notification with x-axis value
        if (xTaskNotifyWait(0, 0, &JOYSTICK_X_DIR, portMAX_DELAY) != pdPASS) {
            continue;
        }

        // Update the state of the Joystick
        if (JOYSTICK_X_DIR < VOLT_0P85)
        {
            joystick_dir = JOYSTICK_LEFT;
        }
        else if (JOYSTICK_X_DIR > VOLT_2P50)
        {
            joystick_dir = JOYSTICK_RIGHT;
        }
        else
        {
            joystick_dir = JOYSTICK_CENTER;
        }

        // Get random value from joystick hardware noise
        random = ((((JOYSTICK_X_DIR << 22) >> 22) * 6666 + (JOYSTICK_X_DIR << 26))) % 160;
    }
}

// initialize joystick, called in main.c
void Joystick_init()
{
    _joystick_init();

    xTaskCreate
    (   Task_Joystick_Timer,
        "Task Joystick Timer",
        configMINIMAL_STACK_SIZE,
        NULL,
        3,
        &Task_Joystick_Timer_Handle
    );

    // Create Joystick Bottom Half Task.
    xTaskCreate
    (   Task_Joystick_Bottom_Half,
        "Task Joystick",
        configMINIMAL_STACK_SIZE,
        NULL,
        4,
        &Task_Joystick_Handle
    );
}
