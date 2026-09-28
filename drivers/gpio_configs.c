#include "stm32f446re.h"

#ifndef GPIO_CONFIGURATION
#define GPIO_CONFIGURATION

typedef struct gpio_config {
  volatile GPIO_Struct *address;
  uint8_t pin1;
  uint8_t pin2;
  uint32_t moder;
  uint32_t otyper;
  uint32_t ospeedr;
  uint32_t pupdr;
  uint32_t afrl;
} GPIO_Config;

typedef struct gpio_types {
  GPIO_Config Configure_GPIO;
} GPIO_Types;

GPIO_Types UsingI2C1 = {
  {
    .address = GPIOB,
    .pin1 = 6,
    .pin2 = 7,
    .moder = 2,
    .otyper = 1,
    .ospeedr = 0,
    .pupdr = 1,
    .afrl = 4
  }
};

GPIO_Types UsingUSART2 = {
  {
    .address = GPIOA,
    .pin1 = 2,
    .pin2 = 3,
    .moder = 2,
    .otyper = 0,
    .ospeedr = 1,
    .pupdr = 1,
    .afrl = 7
  }
};

#endif