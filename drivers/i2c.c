#include "stm32f446re.h"
#include "delay.c"

void i2c_init_and_start(volatile I2C_Struct * i2caddress) {
  
  // Force a complete hardware reset of the i2caddress peripheral
  i2caddress->CR1 |= (1 << 15);  // Set SWRST

  // Small delay
  delay(1);

  i2caddress->CR1 &= ~(1 << 15); // Clear SWRST

  //STARTING THE I2C
  //Program the peripheral input clock
  i2caddress->CR2 &= ~(0b11111 << 0);
  i2caddress->CR2 |= (0b10000 << 0);
  
  //Configure the clock control registers
  //reset
  i2caddress->CCR &= ~(0xFFFF << 0);
  //Select FM mode
  i2caddress->CCR |= (1 << 15);
  
  //Select DUTY, since 16MHz not multiple of 10Mhz, we do 2
  i2caddress->CCR &= ~(1 << 14);
  i2caddress->CCR |= (0 << 14);

  //Set CCR, how? A few things to clear up: 
  // 1) Tlow and Thigh indicate how many ticks aka clocks that they need to stay at their respective levels, so Tlow = 2Thigh means Tlow will be at that level for two PCLK (peripheral clock cycles)
  // 2) Ttotal = Tlow + Thigh 
  // 3) CCR = Ttotal / TPCLK => CCR = Total / 3 * TPCLK
  i2caddress->CCR |= (14 << 0);

  //Set TRISE -> 300 / 62.5 = 4.8 => 4 + 1 => 5; 4 * 62.5  = 250ns or 4 SYSCLK ticks is the max safe limit to rise from 0 to 1
  i2caddress->TRISE &= ~(0b111111 << 0);
  i2caddress->TRISE |= (0b000101);

  //Enable interrupt ITEVTEN and ITBUFEN
  i2caddress->CR2 |= (1 << 9) | (1 << 10);

  //Peripheral start
  i2caddress->CR1 |= (1 << 0);
}