#include <stdint.h>

#ifndef STM32Sleep

#define STM32Sleep

typedef struct {
  uint32_t CTRL;
  uint32_t LOAD;
  uint32_t VAL;
  uint32_t CALIB;
} STK_Struct;

#define SysTick ((volatile STK_Struct *) 0xE000E010)

void Systick_init() {
  SysTick->CTRL |= (1 << 2);
  SysTick->CTRL &= ~(1 << 1);
  SysTick->CTRL |= (1 << 0);
} 

// Using a 16MHz clock, 16,000,000 clocks happen every second
// 16,000,000 / 1000 = 16,000 clock cycles happen every millisecond
void delay(uint32_t milliseconds) {
  SysTick->LOAD = (milliseconds * 16000) - 1;
  SysTick->VAL = 0;

  Systick_init();

  while (!(SysTick->CTRL & (1 << 16))) {
    //Wait until the flag becomes 1
  }

}

#endif