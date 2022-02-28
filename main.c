#include "pico/stdlib.h"

const int STEPS[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
const int STEPS_FULL_ROTATION = 2060;

typedef struct Motor Motor;

struct Motor
{    
    int gpio_step, world_step, increment;
    uint min,max;
    uint GPIO_PINS[4];
};
void define_motor(Motor *motor, uint pin_0, uint pin_1, uint pin_2, uint pin_3, int increment)
{
    motor->GPIO_PINS[0] = pin_0;
    motor->GPIO_PINS[1] = pin_1;
    motor->GPIO_PINS[2] = pin_2;
    motor->GPIO_PINS[3] = pin_3;
    motor->increment = increment;
}
void init_motor(Motor *motor)
{
    for (size_t i = 0; i < 4; i++)
    {
        gpio_init(motor->GPIO_PINS[i]);    
        gpio_set_dir(motor->GPIO_PINS[i], GPIO_OUT);  
    }
}
void update_motor(Motor *motor)
{
    int *step = &motor->gpio_step;
    for (size_t i = 0; i < 4; i++)
    {
        gpio_put(motor->GPIO_PINS[i], STEPS[*step][i]);
    }  
    if(motor->increment < 0)
    {
        *step = (*step-1 < 0) ? 3:*step-1;
    }
    else
    {
        *step = (*step+1 > 3) ? 0:*step+1;
    } 
    motor->world_step += motor->increment;
}

int main() 
{       
    Motor motors[2];

    define_motor(&(motors[0]),18,19,20,21,1);
    define_motor(&(motors[1]),10,11,12,13,1);

    init_motor(&(motors[0]));
    init_motor(&(motors[1]));

    while(true)
    { 
        //update_motor(&(motors[0]));
        update_motor(&(motors[1]));
        sleep_ms(4); 
    }
}



