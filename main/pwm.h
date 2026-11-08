#ifndef PWM_H
#define PWM_H

#include "driver/gpio.h"
#include "driver/ledc.h"

#define LED_PIN GPIO_NUM_7
#define PWM_FREQ 5000
#define PWM_RESOLUTION LEDC_TIMER_10_BIT
#define PWM_MAX  1023

// pwm timer and output channel configurations
extern ledc_timer_config_t timer;
extern ledc_channel_config_t channel;

void pwm_init(void);
void pwm_set_duty(int level);

#endif