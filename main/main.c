// button: gpio4 - gnd (internal pull-up)
// led: gpio7 - 220 Ohm - led - gnd

#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/ledc.h"
#include "hal/ledc_types.h"
#include "soc/clk_tree_defs.h"

#define BUTTON_PIN GPIO_NUM_4
#define LED_PIN GPIO_NUM_7
#define DEBOUNCE_DELAY_MS 50

// PWM
#define PWM_FREQ 5000
#define PWM_RESOLUTION LEDC_TIMER_10_BIT
#define PWM_MAX  1023

void setup(void) {
  // button
  gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
  gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);

  // led
  gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

  // pwm timer
  ledc_timer_config_t timer = {
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .timer_num = LEDC_TIMER_0,
    .duty_resolution = PWM_RESOLUTION,
    .freq_hz = PWM_FREQ,
    .clk_cfg = LEDC_AUTO_CLK
  };
  led_timer_config(&timer);

  // pwm output channel
  ledc_channel_config_t channel = {
    .gpio_num = LED_PIN,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel = LEDC_CHANNEL_0,
    .timer_sel = LEDC_TIMER_0,
    .duty = 0,
    .hpoint = 0
  }
  ledc_channel_config(&channel);
}

void app_main(void) {
  // pin init
  setup();
  
  // vars
  int btn_last_state = 1;
  int led_state = 0;
  int btn_counter = 0;

  // main loop
  while (1) {
    int btn_state = gpio_get_level(BUTTON_PIN);
    // state changed (btn pressed)
    if(btn_state == 0 && btn_last_state == 1) {
      vTaskDelay(pdMS_TO_TICKS(DEBOUNCE_DELAY_MS)); // debounce delay
      if(gpio_get_level(BUTTON_PIN) == 0) {
        btn_counter++;
        led_state = !led_state;
        gpio_set_level(LED_PIN, led_state);
        printf("Clicks: %d\n", btn_counter);
      }
    }
    btn_last_state = btn_state;
    vTaskDelay(pdMS_TO_TICKS(10)); // 10 ms delay
  }
}