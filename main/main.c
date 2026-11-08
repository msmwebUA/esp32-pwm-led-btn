// FULL CODE AVAILABLE AT GITHUB
// https://github.com/msmwebUA/esp32-pwm-led-btn.git

// button: gpio4 - gnd (internal pull-up)
// led: gpio7 - 220 Ohm - led - gnd

#include "pwm.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include <stdio.h>

#define BUTTON_PIN GPIO_NUM_4
#define DEBOUNCE_DELAY_MS 50
#define BLINK_INTERVAL_US 500000 // 500 ms

// functions
void setup(void);
void switch_led_state(void);
void led_blink(void *arg);

// led states
typedef enum {
  OFF,
  ON,
  DIMMED,
  BLINK
} LedState;

LedState led_current_state = OFF;

static int led_level = 0;
static int btn_counter = 0;

// timer
const esp_timer_create_args_t timer_args = {
  .callback = led_blink,
  .name = "led_blink_callback"
};

esp_timer_handle_t esp_timer;

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
        btn_counter++;
        printf("Switched to state %d\n", led_current_state);
        printf("Clicks: %d\n", btn_counter);
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
  // init timer
  esp_timer_create(&timer_args, &esp_timer);
}

void switch_led_state(void) {
  switch(led_current_state) {
    case OFF:
      pwm_set_duty(100);
      led_current_state = ON;
      break;
    case ON:
      pwm_set_duty(25);
      led_current_state = DIMMED;
      break;
    case DIMMED:
      esp_timer_start_periodic(esp_timer, BLINK_INTERVAL_US);
      led_current_state = BLINK;
      break;
    case BLINK:
      esp_timer_stop(esp_timer);
      pwm_set_duty(0);
      led_current_state = OFF;
      break;
  }
}

void led_blink(void *arg) {
  led_level = (led_level) ? 0 : 100;
  pwm_set_duty(led_level);
}