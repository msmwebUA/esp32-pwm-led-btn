// button: gpio4 - gnd (internal pull-up)
// led: gpio7 - 220 Ohm - led - gnd
#include "pwm.h"
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BUTTON_PIN GPIO_NUM_4
#define LED_PIN GPIO_NUM_7
#define DEBOUNCE_DELAY_MS 50

// led states
typedef enum {
  OFF,
  ON,
  DIMMED,
  BLINK
} LedState;

LedState led_state = OFF;

void setup(void) {
  // button
  gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
  gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);

  // led
  gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
}

void app_main(void) {
  // pin init
  setup();
  
  // vars
  int btn_last_state = 1;
  int led_state = 0;

  // main loop
  while (1) {
    int btn_state = gpio_get_level(BUTTON_PIN);
    // state changed (btn pressed)
    if(btn_state == 0 && btn_last_state == 1) {
      vTaskDelay(pdMS_TO_TICKS(DEBOUNCE_DELAY_MS)); // debounce delay
      if(gpio_get_level(BUTTON_PIN) == 0) {
        // handle btn pressed
        printf("Switched to state %d\n", led_state);
      }
    }
    btn_last_state = btn_state;
    vTaskDelay(pdMS_TO_TICKS(10)); // 10 ms delay
  }
}