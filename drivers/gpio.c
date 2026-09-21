#include "stm32f446re.h"

typedef enum {
  input,
  general,
  alternative,
  analog
} moder_states;

typedef enum {
  low,
  medium,
  fast,
  high
} ospeedr_speeds;

typedef enum {
  nopull,
  pullup,
  pulldown,
  reserved
} pupdr_configuration;

void gpio_init(volatile GPIO_Struct * gpio_address) {
  //Configuring the GPIOB Pins 5 and 6, because those are the I2C AF ones
  gpio_address->MODER |= ((2 << 12) | (2 << 14));

  //Set to Open Drain
  gpio_address->OTYPER |= (1 << 6);
  gpio_address->OTYPER |= (1 << 7);

  //Set speed of tranmission of the GPIO
  gpio_address->OSPEEDR &= ~((0b11 << 12) | (0b11 << 14));
  gpio_address->OSPEEDR |= ((0b00 << 12) | (0b00 << 14));

  //Set the voltage driving force
  gpio_address->PUPDR &= ~((3 << 12) | (3 << 14));
  gpio_address->PUPDR |= ((1 << 12) | (1 << 14));

  //Which pins to set as alternative functions
  gpio_address->AFRL |= ((0b0100 << 24) | (0b0100 << 28));

  //Sequence to lock the configuration
  gpio_address->LCKR = (1 << 16) | (1 << 6) | (1 << 7);
  gpio_address->LCKR = (0 << 16) | (1 << 6) | (1 << 7);
  gpio_address->LCKR = (1 << 16) | (1 << 6) | (1 << 7);
  (void)gpio_address->LCKR;

}
