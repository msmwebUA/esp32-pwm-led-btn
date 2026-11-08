#include "pwm.h"
#include "driver/ledc.h"
#include "hal/ledc_types.h"
#include "soc/clk_tree_defs.h"

// pwm timer and channel configs
ledc_timer_config_t timer = {
  .speed_mode = LEDC_LOW_SPEED_MODE,
  .timer_num = LEDC_TIMER_0,
  .duty_resolution = PWM_RESOLUTION,
  .freq_hz = PWM_FREQ,
  .clk_cfg = LEDC_AUTO_CLK
};

ledc_channel_config_t channel = {
  .gpio_num = LED_PIN,
  .speed_mode = LEDC_LOW_SPEED_MODE,
  .channel = LEDC_CHANNEL_0,
  .timer_sel = LEDC_TIMER_0,
  .duty = 0,
  .hpoint = 0
};

// init pwm timer and channel
void pwm_init(void) {
  ledc_timer_config(&timer);
  ledc_channel_config(&channel);
}

// Set PWM duty cycle and interval for blinking
// Level: 0..100%, freq: pwm frequency in Hz, pass 0 for default
void pwm_set_duty(int level) {
  // duty cycle
  int duty_cycle = 0;
  if (level <= 0) {
    duty_cycle = 0;
  } else if (level >= 100) {
    duty_cycle = PWM_MAX;
  } else {
    duty_cycle = PWM_MAX * level / 100;
  }
  // set pwm
  ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, duty_cycle);
  ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
}