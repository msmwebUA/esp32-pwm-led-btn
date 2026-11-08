#ifndef PWM_H
#define PWM_H

#include "driver/ledc.h"
#include "hal/ledc_types.h"
#include "soc/clk_tree_defs.h"

#define PWM_FREQ 5000
#define PWM_RESOLUTION LEDC_TIMER_10_BIT
#define PWM_MAX  1023

// pwm timer
ledc_timer_config_t timer = {
  .speed_mode = LEDC_LOW_SPEED_MODE,
  .timer_num = LEDC_TIMER_0,
  .duty_resolution = PWM_RESOLUTION,
  .freq_hz = PWM_FREQ,
  .clk_cfg = LEDC_AUTO_CLK
};

extern led_timer_config(&timer);

// pwm output channel
ledc_channel_config_t channel = {
  .gpio_num = LED_PIN,
  .speed_mode = LEDC_LOW_SPEED_MODE,
  .channel = LEDC_CHANNEL_0,
  .timer_sel = LEDC_TIMER_0,
  .duty = 0,
  .hpoint = 0
}

extern ledc_channel_config(&channel);

void set_pwm_duty(int level);

#endif