#include "stm32f446re.h"
#include "gpio_configs.c"

void gpio_init(GPIO_Types type) {

  GPIO_Config gpio = type.Configure_GPIO;

  //Configuring the GPIOB Pins 6 and 7, because those are the I2C AF ones
  (gpio.address)->MODER &= ~((3 << (gpio.pin1 * 2)) | (3 << (gpio.pin2 * 2)));
  (gpio.address)->MODER |= ((gpio.moder << (gpio.pin1 * 2)) | (gpio.moder << (gpio.pin2 *2)));

  //Set to Open Drain
  (gpio.address)->OTYPER &= ~((1 << gpio.pin1) | (1 << gpio.pin2));
  (gpio.address)->OTYPER |= (gpio.otyper << gpio.pin1);
  //UART4 is on GPIOA, and RX is input only so it doesn't get PMOS and NMOS (push-pull) functionality
  if (gpio.address != GPIOA) {
    (gpio.address)->OTYPER |= (gpio.otyper << gpio.pin2);
  }

  //Set speed of tranmission of the GPIO
  (gpio.address)->OSPEEDR &= ~((0b11 << (gpio.pin1 * 2)) | (0b11 << (gpio.pin2 *2)));
  (gpio.address)->OSPEEDR |= ((gpio.ospeedr << (gpio.pin1 * 2)) | (gpio.ospeedr << (gpio.pin2 *2)));

  //Set the voltage driving force
  (gpio.address)->PUPDR &= ~((3 << (gpio.pin1 * 2)) | (3 << (gpio.pin2 *2)));
  (gpio.address)->PUPDR |= (((gpio.address != GPIOA) ? (gpio.pupdr << (gpio.pin1 * 2)) : 0) | (gpio.pupdr << (gpio.pin2 *2)));

  //Which pins to set as alternative functions
  (gpio.address)->AFRL &= ~((15 << (gpio.pin1 * 4)) | (15 << (gpio.pin2 * 4)));
  (gpio.address)->AFRL |= ((gpio.afrl << (gpio.pin1 * 4)) | (gpio.afrl << (gpio.pin2 * 4)));

  //Sequence to lock the configuration
  (gpio.address)->LCKR = (1 << 16) | (1 << gpio.pin1) | (1 << gpio.pin2);
  (gpio.address)->LCKR = (0 << 16) | (1 << gpio.pin1) | (1 << gpio.pin2);
  (gpio.address)->LCKR = (1 << 16) | (1 << gpio.pin1) | (1 << gpio.pin2);
  (void)(gpio.address)->LCKR;

}
