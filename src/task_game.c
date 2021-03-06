#include "task_game.h"

TaskHandle_t Task_Game_Handle = NULL;

// Task Game
// calculate next frame and send LCD parameters to Queue_LCD for Task_LCD to draw
void Task_Game(void *pvParameters)
{
    LCD_t nextFrame;

    // position of cactus
    float cactus_x = CACTUS_X_MAX;
    uint16_t cactus_y = CACTUS_Y;

    // position of dinosaur
    uint16_t dino_x = DINO_X_MIN + 10;
    uint16_t dino_y = DINO_Y_MAX;

    // game indicator
    bool jumping = false;
    uint16_t jumping_frame = 0;

    // speed of cactus
    float speed_cactus = 1.0;
    uint16_t speed_dino = 1;

    while (1)
    {
        // detect jumping ends
        if (jumping && dino_y >= DINO_Y_MAX)
        {
            dino_y = DINO_Y_MAX;
            jumping = false;
            jumping_frame = 1;
        }

        // button pressed, jump
        if (!jumping && (button_pressed | light_intensity == LIGHT_LOW))
        {
            jumping = true;
            jumping_frame = 1;

            // buzzer play sound according to speed
            if (accel_data == TILT_LEFT)
            {
                turn_Buzzer_On(BUTTON_SOUND_PITCH_LOW);
            }
            else if (accel_data == FLAT)
            {
                turn_Buzzer_On(BUTTON_SOUND_PITCH_MID);
            }
            else if (accel_data == TILT_RIGHT)
            {
                turn_Buzzer_On(BUTTON_SOUND_PITCH_HIGH);
            }
            T32_1_wait_period(BUTTON_SOUND_DURATION);
            turn_Buzzer_Off();

        }

        // if jumping, update dinosaur y position
        if (jumping)
        {
            dino_y = 78 / 1600.0 * pow(40 - jumping_frame, 2) + 32;
            jumping_frame = jumping_frame + 1;
        }

        // if joystick left and right, update dinosaur x position
        if (joystick_dir == JOYSTICK_LEFT)
        {
            dino_x = dino_x > DINO_X_MIN ? dino_x - speed_dino : DINO_X_MIN;
        }
        else if (joystick_dir == JOYSTICK_RIGHT)
        {
            dino_x = dino_x < DINO_X_MAX ? dino_x + speed_dino : DINO_X_MAX;
        }

        // draw dinosaur
        nextFrame.width_pixels = dinoWidthPixels;
        nextFrame.height_pixels = dinoHeightPixels;
        nextFrame.image = dinoBitmaps;
        if (accel_data == TILT_LEFT)
        {
            speed_dino = 1;
            nextFrame.fColor = BLUE_LIGHT;
        }
        else if (accel_data == TILT_RIGHT)
        {
            speed_dino = 2;
            nextFrame.fColor = PINK;
        }
        else if (accel_data == FLAT)
        {
            speed_dino = 1;
            nextFrame.fColor = YELLOW;
        }
        nextFrame.bColor = BLACK;
        nextFrame.x = dino_x;
        nextFrame.y = dino_y;

        // send next frame to Queue_LCD
        xQueueSendToBack(Queue_LCD, &nextFrame, portMAX_DELAY);

        // draw cactus if in the screen
        if ((uint16_t)cactus_x <= CACTUS_X_MAX)
        {
            nextFrame.width_pixels = cactusWidthPixels;
            nextFrame.height_pixels = cactusHeightPixels;
            nextFrame.image = cactusBitmaps;
            nextFrame.fColor = (uint16_t)cactus_x <= CACTUS_X_MIN ? BLACK : GREEN; // disappear if hit left boundary
            nextFrame.bColor = BLACK;
            nextFrame.x = (uint16_t)cactus_x;
            nextFrame.y = cactus_y;

            // send next frame to Queue_LCD
            xQueueSendToBack(Queue_LCD, &nextFrame, portMAX_DELAY);
        }

        // Simple collision detection
        if (((cactus_x - dino_x >= 0 && cactus_x - dino_x <= CACTUS_DINO_LIMIT_X) ||
             (dino_x - cactus_x >= 0 && dino_x - cactus_x <= DINO_CACTUS_LIMIT_X)) &&
             (cactus_y - dino_y <= DINO_CACTUS_LIMIT_Y))
        {
            _LED_switch(true, false, false); // turn on red LED indicating collision
        }
        else if (light_intensity != LIGHT_LOW)
        {
            _LED_switch(false, false, false); // turn off
        }

        // Update position of cactus
        if (accel_data == TILT_LEFT)
        {
            speed_cactus = 0.3;
        }
        else if (accel_data == TILT_RIGHT)
        {
            speed_cactus = 1.7;
        }
        else if (accel_data == FLAT)
        {
            speed_cactus = 1;
        }
        cactus_x = (uint16_t)cactus_x <= CACTUS_X_MIN ? CACTUS_X_MAX + random:
                                                        cactus_x - speed_cactus;

        // delay for 10mS using vTaskDelay
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

// start the game, called in main.c
void Game_init()
{
    xTaskCreate
    (   Task_Game,
        "Start the game",
        configMINIMAL_STACK_SIZE,
        NULL,
        4,
        &Task_Game_Handle
    );
}
