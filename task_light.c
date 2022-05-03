#include <task_light.h>

TaskHandle_t Task_Light_Handle = NULL;

volatile light_t light_intensity = LIGHT_MID;

// Initialize the opt3001 ambient light sensor on the MKII.
// This function assumes I2C interface has already been configured to 100KHz.
void _opt3001_init(void)
{
    // Parameter: 0xC610 (please refer to opt3001 technical reference)
    // 1. Conversion time: 100ms
    // 2. Mode of conversion: Continuous conversion
    // Write to configure register
    i2c_write_16(OPT3001_SLAVE_ADDRESS, CONFIG_REG, 0xC610);
}

// Returns the current ambient light in Lux
// Refer to opt3001 technical reference for conversion
float _opt3001_read_lux(void)
{
    // Read the Result register of OPT3001
    uint16_t read = i2c_read_16(OPT3001_SLAVE_ADDRESS, RESULT_REG);

    // convert to lux and return
    return (double)0.01 * (double)pow(2, read>>12) * (double)(read & 0x0FFF);
}

void _MKII_RGB_LED_init(void)
{
    // Configures the RED, GREEN, and BLUE LEDs to be outputs
    P2->DIR |= BIT6 | BIT4;
    P5->DIR |= BIT6;
    // Turn off LEDs
    P2->OUT &= ~BIT6 & ~BIT4;
    P5->OUT &= ~BIT6;
}

void _LED_switch(bool red, bool green, bool blue)
{
    if (red)
        P2->OUT |= BIT6;
    else
        P2->OUT &= ~BIT6;

    if (green)
        P2->OUT |= BIT4;
    else
        P2->OUT &= ~BIT4;

    if (blue)
        P5->OUT |= BIT6;
    else
        P5->OUT &= ~BIT6;
}

// Get light intensity status
void _update_light_status(void)
{
    float lux = _opt3001_read_lux();

    if (lux < LIGHT_THRESHOLD_LOW)
    {
        light_intensity = LIGHT_LOW;
        _LED_switch(false, true, false); // turn on GREEN
    }
    else if (lux < LIGHT_THRESHOLD_HIGH)
    {
        light_intensity = LIGHT_MID;
    }
    else
    {
        light_intensity = LIGHT_HIGH;
    }
}

// Task MKII Button 2
void Task_Light(void *pvParameters)
{
    // Detect light intensity
    while (1)
    {
        // Get light intensity status
        _update_light_status();

        // Delay for 10mS using vTaskDelay
        vTaskDelay(pdMS_TO_TICKS(50));
    }

}

// initialize light sensor on MKII, called in main.c
void Light_init()
{
    i2c_init();

    _MKII_RGB_LED_init();
    _opt3001_init();

    xTaskCreate
    (   Task_Light,
        "Detect Light Intensity",
        configMINIMAL_STACK_SIZE,
        NULL,
        4,
        &Task_Light_Handle
    );
}
