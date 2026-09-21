#include <stdint.h>

#ifndef STM32F446RE

#define STM32F446RE

typedef struct {
  uint32_t MODER;
  uint32_t OTYPER;
  uint32_t OSPEEDR;
  uint32_t PUPDR;
  uint32_t IDR;
  uint32_t ODR;
  uint32_t BSRR;
  uint32_t LCKR;
  uint32_t AFRL;
  uint32_t AFRH;
} GPIO_Struct;


#define GPIOA ((volatile GPIO_Struct *) 0x40020000)
#define GPIOB ((volatile GPIO_Struct *) 0x40020400)
#define GPIOC ((volatile GPIO_Struct *) 0x40020800)
#define GPIOD ((volatile GPIO_Struct *) 0x40020C00)
#define GPIOE ((volatile GPIO_Struct *) 0x40021000)
#define GPIOF ((volatile GPIO_Struct *) 0x40021400)
#define GPIOG ((volatile GPIO_Struct *) 0x40021800)
#define GPIOH ((volatile GPIO_Struct *) 0x40021C00)


#endif