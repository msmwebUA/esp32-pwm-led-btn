// button: gpio4 - gnd (internal pull-up)
// led: gpio7 - 220 Ohm - led - gnd

#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define BUTTON_PIN GPIO_NUM_4
#define LED_PIN GPIO_NUM_7

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
  int led_state = 0;
  int last_btn_state = 1;
  int btn_counter = 0;

  while(1) {
    int btn_state = gpio_get_level(BUTTON_PIN);
    if (btn_state == 0 && last_btn_state == 1) {
     vTaskDelay(pdMS_TO_TICKS(50)); // 50 ms delay
      if (gpio_get_level(BUTTON_PIN) == 0) {
        btn_counter++;
        led_state = !led_state;
        gpio_set_level(LED_PIN, led_state);
        printf("Clicks: %d\n", btn_counter);
      }
    }
    last_btn_state = btn_state;
    vTaskDelay(pdMS_TO_TICKS(10)); // 10 ms delay
  }
}