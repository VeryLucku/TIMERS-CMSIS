#include "stm32f4xx.h"
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define PWM_freq 1
#define ARR_MAX 10000

void timer1_PWM_init();