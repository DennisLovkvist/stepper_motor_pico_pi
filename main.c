#include "pico/stdlib.h"

const uint GPIO_PINS[4] = {18,19,20,21};

const int steps[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};


int main() 
{   
    for (size_t i = 0; i < 4; i++)
    {
        gpio_init(GPIO_PINS[i]);    
        gpio_set_dir(GPIO_PINS[i], GPIO_OUT);  
    }

    int s = 0;
    while(true)
    {
	    for (size_t i = 0; i < 4; i++)
        {
            gpio_put(GPIO_PINS[i], steps[step][i]);
        }  
        s = (s+1 == 4 ? 0: s+1);
        sleep_ms(5);        
    }
}
