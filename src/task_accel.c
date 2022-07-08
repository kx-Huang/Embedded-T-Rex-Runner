#include <task_accel.h>

TaskHandle_t Task_Accel_Handle = NULL;
TaskHandle_t Task_Accel_Timer_Handle = NULL;

volatile tilt_t accel_data = FLAT;

void _accel_init()
{
    // Set accelerometer x axis as analog input
    // P6.1 <-> J3.23 (A14)
    P6->DIR &= ~BIT1;
    P6->SEL0 |= BIT1;
    P6->SEL1 |= BIT1;

    // Configure CTL0 to sample 16-times in pulsed sample mode.
    ADC14->CTL0 = ADC14_CTL0_SHP |      // timer source
                  ADC14_CTL0_SHT02 |    // 16 samples
                  ADC14_CTL0_CONSEQ_1;  // sequence-of-channels

    // Configure ADC to return 12-bit values
    ADC14->CTL1 = ADC14_CTL1_RES_2;

    // Associate the accelerometer signal with MEM[1]
    ADC14->MCTL[1] |= ADC14_MCTLN_INCH_14;

    // Indicate this is the end of a sequence.
    ADC14->MCTL[1] |= ADC14_MCTLN_EOS;

    // Enable interrupts AFTER a value is written into MEM[1].
    ADC14->IER0 |= ADC14_IER0_IE1;

    // Enable ADC Interrupt with priority 2.
    NVIC_EnableIRQ(ADC14_IRQn);
    NVIC_SetPriority(ADC14_IRQn, 2);

    // Turn ADC on.
    ADC14->CTL0 |= ADC14_CTL0_ON;

    __enable_irq();
}

void Task_Accel_Timer(void *pvParameters)
{
    while (1)
    {
        // Start ADC conversion.
        ADC14->CTL0 |= ADC14_CTL0_SC | ADC14_CTL0_ENC;

        // Delay for 10mS using vTaskDelay
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void Task_Accel_Bottom_Half(void *pvParameters)
{
    uint32_t ACCEL_READ = 0;

    while (1)
    {
        // Get/Wait for task notification with x-axis value
        if (xTaskNotifyWait(0, 0, &ACCEL_READ, portMAX_DELAY) != pdPASS)
        {
            continue;
        }

        // Update the state of the accelerometer
        if (ACCEL_READ < 1750)
        {
            accel_data = TILT_LEFT;
        }
        else if (ACCEL_READ > 2150)
        {
            accel_data = TILT_RIGHT;
        }
        else
        {
            accel_data = FLAT;
        }
    }
}

// initialize Accelerometer, called in main.c
void Accel_init()
{
    _accel_init();

    xTaskCreate
    (   Task_Accel_Timer,
        "Task Accelerometer Timer",
        configMINIMAL_STACK_SIZE,
        NULL,
        4,
        &Task_Accel_Timer_Handle
    );

    // Create Accelerometer Bottom Half Task.
    xTaskCreate
    (   Task_Accel_Bottom_Half,
        "Task Accelerometer Read",
        configMINIMAL_STACK_SIZE,
        NULL,
        4,
        &Task_Accel_Handle
    );
}


