#include <task_button.h>

TaskHandle_t Task_Button_Handle = NULL;

// initialize the GPIO pin that control S2 on the MKII
void _MKII_S2_Init(void)
{
    P3->DIR &= ~BIT5;
}

// Return if S2 on MKII is currently pressed
bool _MKII_S2_pressed(void)
{
    if (P3->IN & BIT5)
        return false;
    else
        return true;
}

// De-bounce switch S2.
bool _MKII_S2_debounce(void)
{
    static uint8_t debounce_state = 0x00;

    // Shift the de-bounce variable to the left
    debounce_state = debounce_state << 1;

    // If S1 is being pressed, set the LSBit of debounce_state to a 1;
    if (_MKII_S2_pressed())
    {
        debounce_state |= 0x01;
    }

    // If the de-bounce variable is equal to 0x7F, return true
    if (debounce_state == 0x0F)
    {
        return true;
    }
    else
    {
        return false;
    }

}

// Task MKII Button 2
void Task_Button(void *pvParameters)
{
    // Detect button state
    while (1)
    {
        // Detect button pressed or not
        button_pressed = _MKII_S2_pressed();

        // Delay for 10mS using vTaskDelay
        vTaskDelay(pdMS_TO_TICKS(10));
    }

}

// initialize Button 2 on MKII, called in main.c
void Button_init()
{
    _MKII_S2_Init();

    xTaskCreate
    (   Task_Button,
        "Detect MKII Button 2 Status",
        configMINIMAL_STACK_SIZE,
        NULL,
        4,
        &Task_Button_Handle
    );
}

