// button: gpio4 - gnd (internal pull-up)
// led: gpio7 - 220 Ohm - led - gnd
#include "pwm.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <stdio.h>

#define BUTTON_PIN GPIO_NUM_4
#define DEBOUNCE_DELAY_MS 50
#define BLINK_FREQ 5 // 5 Hz

// led states
typedef enum {
  OFF,
  ON,
  DIMMED,
  BLINK
} LedState;

LedState led_current_state = OFF;

// function
void setup(void);
void switch_led_state(void);

void app_main(void) {
  // pins init
  setup();
  
  // vars
  int btn_last_state = 1;

  // main loop
  while (1) {
    int btn_state = gpio_get_level(BUTTON_PIN);
    // state changed (btn pressed)
    if(btn_state == 0 && btn_last_state == 1) {
      vTaskDelay(pdMS_TO_TICKS(DEBOUNCE_DELAY_MS)); // debounce delay
      if(gpio_get_level(BUTTON_PIN) == 0) {
        switch_led_state();
        printf("Switched to state %d\n", led_current_state);
      }
    }
    btn_last_state = btn_state;
    vTaskDelay(pdMS_TO_TICKS(10)); // 10 ms delay
  }
}

void setup(void) {
  // button pin
  gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
  gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);
  // init pwm
  pwm_init();
}

void switch_led_state(void) {
  switch(led_current_state) {
    case OFF:
      led_current_state = ON;
      pwm_set_duty(100, 0);
      break;
    case ON:
      led_current_state = DIMMED;
      pwm_set_duty(25, 0);
      break;
    case DIMMED:
      led_current_state = BLINK;
      pwm_set_duty(100, BLINK_FREQ);
      break;
    case BLINK:
      led_current_state = OFF;
      pwm_set_duty(0, 0);
      break;
  }
}