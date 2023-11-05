#include "gpio.h"

void alt_func_setup(GPIO_TypeDef *GPIOx, uint8_t port_num, uint8_t alt_func_num)
{
    // timer1 ports - enable alternate function mode
    MODIFY_REG(GPIOA->MODER, 0x3UL << (2*port_num), 0b10 << (2*port_num));

    // change to timer1 mode
    if (port_num < 8) {
        MODIFY_REG(GPIOA->AFR[0], 0xFUL << (4*port_num), alt_func_num << (4*port_num));
    } else {
        port_num = port_num - 8;
        MODIFY_REG(GPIOA->AFR[1], 0xFUL << (4*port_num), alt_func_num << (4*port_num));
    }

    
    
}