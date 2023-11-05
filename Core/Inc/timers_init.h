#include "stm32f4xx.h"
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define PWM_freq 10000
#define ARR_PWM_MAX 10000

// 1 mks
#define COMPARE_FREQ 1000000

void timer1_PWM_init();

void timer3_PWM_input_init();