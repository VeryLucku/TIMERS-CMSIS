#include "stm32f4xx.h"
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define PWM_freq 1
#define ARR_PWM_MAX 10000

#define COMPARE_FREQ 10000

void timer1_PWM_init();

void timer3_PWM_input_init();