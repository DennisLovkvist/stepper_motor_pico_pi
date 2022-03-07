#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include <stdbool.h>
#include "pico/multicore.h"

const int STEPS[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
const int STEPS_FULL_ROTATION = 2060;

typedef struct Motor Motor;
typedef struct Joystick Joystick;

struct Motor
{    
    int gpio_step, world_step, increment;
    uint GPIO_PINS[4];
};
void define_motor(Motor *motor, uint pin_0, uint pin_1, uint pin_2, uint pin_3, int increment)
{
    motor->GPIO_PINS[0] = pin_0;
    motor->GPIO_PINS[1] = pin_1;
    motor->GPIO_PINS[2] = pin_2;
    motor->GPIO_PINS[3] = pin_3;
    motor->increment = increment;
    motor->world_step = 0;
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
    int *step = &(motor->gpio_step);
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
int control_motor(int adc, int invert)
{
    adc_select_input(adc);
    uint16_t t1 = adc_read();

    if(abs(t1 -2048) <400)
    {
        return 0;
    }
    else
    {
        if(t1 < 2010)
        {
            return (invert) ? -1:1;
        }
        else
        {
            return (invert) ? 1:-1;
        }
    }
}

//core1
void core1_entry()
{
    bool lol = true;

    while (1)
    {        
        
        gpio_put(1,lol);
        lol = !lol;
        sleep_ms(100);
    }
    
}
//core0
int main() 
{          
    stdio_init_all();

    Motor motor_1;
    define_motor(&motor_1,18,19,20,21,1);
    init_motor(&motor_1);
    
    Motor motor_2;
    define_motor(&motor_2,13,12,11,10,1);
    init_motor(&motor_2); 
    
    adc_init();
    adc_gpio_init(27);
    adc_gpio_init(28);

    gpio_init(1);    
    gpio_set_dir(1, 1);     

    multicore_launch_core1(core1_entry);

    while (1) 
    {    
           
        sleep_ms(3);
        if(motor_1.increment != 0)update_motor(&motor_1);
        motor_1.increment = control_motor(1,0);
        
        sleep_ms(3);
        if(motor_2.increment != 0)update_motor(&motor_2);        
        motor_2.increment = control_motor(2,0);
       
    }
}


