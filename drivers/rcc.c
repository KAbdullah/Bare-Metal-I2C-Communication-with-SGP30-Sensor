#include <stdint.h>

//RCC - over here when we put the outer *, we're saying that I want to immediately start working in the memory
// and not work with a pointer that points to a memory, so in subsequent uses of RCC, I'm working with memory 
#define RCC 0x40023800

#define RCC_CR (*((volatile uint32_t *)(RCC + 0x00)))
#define RCC_CFGR (*((volatile uint32_t *)(RCC + 0x08)))

//GPIOB
#define RCC_AHB1ENR (*((volatile uint32_t *)(RCC + 0x30)))
//I2C
#define RCC_APB1ENR (*((volatile uint32_t *)(RCC + 0x40)))

void init_rcc_hsi(void) {
  //Turn HSIO clock on
  RCC_CR |= (1 << 0);

  //Blocking until HSI starts
  while (!(RCC_CR & (1 << 1)));

  //Choose system clock to use HSI
  RCC_CFGR |= (0b00 << 0);

  //Blocking until sys switched to HSI
  while (1) {
    if (((RCC_CFGR >> 2) & 0x3) == 0x00) {
      break;
    }
  }
  
}

void turn_on_gpiob(void) {
  //Turn on GPIOB clock
  RCC_AHB1ENR |= (1 << 1);
}

void turn_on_i2c1(void) {
  //Turn on I2C clock
  RCC_APB1ENR |= (1 << 21);
}