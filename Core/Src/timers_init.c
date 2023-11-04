#include "timers_init.h"

void timer1_PWM_init()
{
    // Enable tacting of ports
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);

    // timer1 ports - enable alternate function mode
    MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE8, 0b10 << GPIO_MODER_MODE8_Pos);

    // change to timer1 mode
    MODIFY_REG(GPIOA->AFR[1], GPIO_AFRH_AFSEL8, 1 << GPIO_AFRH_AFSEL8_Pos);

    // enable tacting of timer1
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN);

    // enable ARP
    SET_BIT(TIM1->CR1, TIM_CR1_ARPE);

    // APB2_Presc in form 2^APB2_Presc
    uint8_t APB2_Presc = APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos];

    // counting APB2 timer clock (if presc > 0 (div 2 or more) when timers clock multiply by 2)
    uint32_t APB2_Timer_Clock = SystemCoreClock >> APB2_Presc;
    if (APB2_Presc > 0) {
        APB2_Timer_Clock = APB2_Timer_Clock * 2;
    }

    
    // setup ARR
    TIM1->ARR = ARR_MAX - 1;

    // setup timer prescaler to setup PWM freq
    TIM1->PSC = APB2_Timer_Clock / (ARR_MAX * PWM_freq) - 1;

    // setup duty cycle to 50 %
    TIM1->CCR1 = TIM1->ARR / 2 + 1;

    // setup 1 channel to output
    SET_BIT(TIM1->CCER, TIM_CCER_CC1E);

    // active level high (ater compare event changes to low)
    CLEAR_BIT(TIM1->CCER, TIM_CCER_CC1P);

    // enable using timer outputs as outputs
    SET_BIT(TIM1->BDTR, TIM_BDTR_MOE);

    // enable PWM mode 1
    SET_BIT(TIM1->CCMR1, TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);

    // fast mode disabled
    CLEAR_BIT(TIM1->CCMR1, TIM_CCMR1_OC1FE);

    // upcounting
    CLEAR_BIT(TIM1->CR1, TIM_CR1_DIR);

    // edge-aligned mode
    CLEAR_BIT(TIM1->CR1, TIM_CR1_CMS_Msk);

    // enable the counter
    SET_BIT(TIM1->CR1, TIM_CR1_CEN);
}