#include "timers_init.h"
#include "gpio.h"

void timer1_PWM_init()
{
    // Enable tacting of ports
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);

    alt_func_setup(GPIOA, 8, 1);

    // enable tacting of timer1
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN);

    // enable ARP
    SET_BIT(TIM1->CR1, TIM_CR1_ARPE);

    // APB2_Presc in form 2^APB2_Presc
    uint8_t APB2_Presc = APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos];

    // counting APB2 timer clock (if presc > 0 (div 2 or more) when timers clock multiply by 2)
    uint32_t APB2_Timer_Clock = SystemCoreClock >> APB2_Presc;
    if (APB2_Presc > 0)
    {
        APB2_Timer_Clock = APB2_Timer_Clock * 2;
    }

    // setup ARR
    TIM1->ARR = ARR_PWM_MAX - 1;

    // setup timer prescaler to setup PWM freq
    TIM1->PSC = APB2_Timer_Clock / (ARR_PWM_MAX * PWM_freq) - 1;

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

void timer3_PWM_input_init()
{
    // Enable tacting of ports
    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);

    alt_func_setup(GPIOA, 6, 2);

    // enable tacting of timer1
    SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM3EN);

    // enable ARP
    SET_BIT(TIM3->CR1, TIM_CR1_ARPE);

    // APB2_Presc in form 2^APB1_Presc
    uint8_t APB1_Presc = APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos];

    // counting APB1 timer clock (if presc > 0 (div 2 or more) when timers clock multiply by 2)
    uint32_t APB1_Timer_Clock = SystemCoreClock >> APB1_Presc;
    if (APB1_Presc > 0)
    {
        APB1_Timer_Clock = APB1_Timer_Clock << 1;
    }

    // setup timer prescaler to setup PWM freq
    TIM3->PSC = APB1_Timer_Clock / COMPARE_FREQ - 1;

    // setup ARR
    TIM3->ARR = 65535;

    // sellect trigger to timer input 1
    MODIFY_REG(TIM3->SMCR, TIM_SMCR_TS, TIM_SMCR_TS_2 | TIM_SMCR_TS_0);

    // reset the counter after trigger event
    MODIFY_REG(TIM3->SMCR, TIM_SMCR_SMS, TIM_SMCR_SMS_2);

    // conf ch1 as inpit and map on TI1
    SET_BIT(TIM3->CCMR1, 0b01 << TIM_CCMR1_CC1S_Pos);

    // conf ch2 as inpit and map on TI1
    SET_BIT(TIM3->CCMR1, 0b10 << TIM_CCMR1_CC2S_Pos);

    // enable capture of ch1 at rising edge
    CLEAR_BIT(TIM3->CCER, TIM_CCER_CC1P);
    // enable capture of ch2 at lowing edge
    SET_BIT(TIM3->CCER, TIM_CCER_CC2P);
    

    // enable capture
    SET_BIT(TIM3->CCER, TIM_CCER_CC1E);
    SET_BIT(TIM3->CCER, TIM_CCER_CC2E);

    // upcounting
    CLEAR_BIT(TIM3->CR1, TIM_CR1_DIR);

    // edge-aligned mode
    CLEAR_BIT(TIM3->CR1, TIM_CR1_CMS_Msk);

    // enable CC event
    SET_BIT(TIM3->EGR, TIM_EGR_CC1G);
    SET_BIT(TIM3->EGR, TIM_EGR_CC2G);

    // enable CC2 interrupt
    //SET_BIT(TIM3->DIER, TIM_DIER_CC1IE);
    SET_BIT(TIM3->DIER, TIM_DIER_CC2IE);

    // enable interrupts
    NVIC_EnableIRQ(TIM3_IRQn);

    // enable the counter
    SET_BIT(TIM3->CR1, TIM_CR1_CEN);
}


uint32_t pulse = 0;
uint32_t period = 0;
void TIM3_IRQHandler(void)
{
    // measure pulse and period of PWM signal as captured values in CCR1 and CCR2 registers
    if (READ_BIT(TIM3->SR, TIM_SR_CC2IF)) {
        period = TIM3->CCR2;
        pulse = TIM3->CCR1;
        CLEAR_BIT(TIM3->SR, TIM_SR_CC2IF);
    }
}