#include <stdint.h>
#include "../drivers/stm32f446re.h"
#include "../drivers/gpio.c"
#include "../drivers/delay.c"
#include "../drivers/rcc.c"
#include "../drivers/i2c.c"

void I2C1_EV_IRQHandler (void);

typedef enum {
  I2C_STATE_INIT_WRITE,
  I2C_STATE_MEASURE_WRITE,
  I2C_STATE_MEASURE_READ,
} i2c_state_t;

static volatile i2c_state_t current_i2c_state = I2C_STATE_INIT_WRITE;

//this will belong to the SGP30.c file
static volatile uint8_t receiver = 0;
static volatile uint8_t first_time_start = 1;

//SGP30 measurement command 
static uint8_t SGPCommandBuffer[] = {0x20, 0x08};
static volatile uint8_t SGPBufferIndex = 0;
static uint8_t SGPInnitBuffer[] = {0x20, 0x03};
static volatile uint8_t SGPInnitIndex = 0;

static volatile uint8_t currDataReceptionNumber = 0;
static volatile uint8_t i2c_in_progress = 0;


int main (void) {

  init_rcc_hsi();

  turn_on_gpiob();

  gpio_init(GPIOB);

  turn_on_i2c1();

  i2c_init_and_start(I2C1);

  __asm("CPSIE i"); // Change Processor State to enable interrupt 
  //0xE000E100 is the NVIC base address 
  *((volatile uint32_t *)0xE000E100) |= (1 << 31); //I2C interrupt is number 31, so enable it to 1.

  //Start condition to initialize the SGP30
  i2c_in_progress = 1;
  I2C1->CR1 |= (1 << 8);

  delay(10);

  while (1) {

    //Start condition to get into controller mode
    // 0 means write, 1 means read
    //Turn on ITBUFEN because it was turned off
    I2C1->CR2 |= (1 << 10);
    receiver = 0;

    //Reset measurement and Init indexes
    SGPBufferIndex = 0;
    SGPInnitIndex = 0;

    while (i2c_in_progress);

    //Start again
    current_i2c_state = I2C_STATE_MEASURE_WRITE;
    i2c_in_progress = 1;
    I2C1->CR1 |= (1 << 8);

    while (i2c_in_progress);

    //Measurement time is 12ms
    delay(12);

    //Turn on ITBUFEN because it was turned off
    I2C1->CR2 |= (1 << 10);
    I2C1->CR1 |= (1 << 10);
    //Send another start condition here basically to start reading data
    current_i2c_state = I2C_STATE_MEASURE_READ;
    receiver = 1;
    currDataReceptionNumber = 0;
    i2c_in_progress = 1;
    I2C1->CR1 |= (1 << 8);

    while (i2c_in_progress);

    //Pause for one second before starting again
    delay(1000);

  }
}

uint8_t log_trace[100];
uint8_t logtrace_index= 0;
uint8_t data_from_sensor[1000];
uint32_t data_from_sensor_index = 0;


void I2C1_EV_IRQHandler (void) {
  volatile uint16_t sr1 = I2C1->SR1;

  //I do only 1, then everything else is set to 0, so 0b0000000000000001
  if (sr1 & (1 << 0)) {
    (void)I2C1->SR1;
    //THE SGP30 uses 7 bits addressing, so we send the address starting at bit 1 and reserve the LSB as 0 (reset) to enter transmitter mode
    if (receiver) {
      if (logtrace_index< 10) log_trace[logtrace_index++] = 1;
      I2C1->DR = ((0x58 << 1) | (1 << 0));
    } else {
      if (logtrace_index< 10) log_trace[logtrace_index++] = 2;
      I2C1->DR = (0x58 << 1);
    }
    return;
  }

  // Data register is empty here
  if (sr1 & (1 << 1)) {
    //Reset the ADDR bit
    (void)I2C1->SR1;
    (void)I2C1->SR2;

    if (!receiver) {
      if (first_time_start) {
        if (logtrace_index< 10) log_trace[logtrace_index++] = 3;
        I2C1->DR = SGPInnitBuffer[SGPInnitIndex++];
      } else {
        if (logtrace_index< 10) log_trace[logtrace_index++] = 4;
        I2C1->DR = SGPCommandBuffer[SGPBufferIndex++];
      }
    }
    return;
  }

  //If data register is empty so TxE = 1 (because we are in transmitter mode) we write measurement command 
  //In transmitter mode, TxE
  if (sr1 & (1 << 7) && !receiver) {
    if (current_i2c_state == I2C_STATE_INIT_WRITE) {
      if (SGPInnitIndex < 2) {
        I2C1->DR = SGPInnitBuffer[SGPInnitIndex++];
        if (logtrace_index< 10) log_trace[logtrace_index++] = 5;
      } else {

        if (!i2c_in_progress) {
          return;  // already handled this transfer's completion, ignore re-entry
        }

        if (logtrace_index< 10) log_trace[logtrace_index++] = 6;
        //Turn off ITBUFEN so that TxE doesn't cause any more triggers

        i2c_in_progress = 0;

        first_time_start = 0;

        I2C1->CR2 &= ~(1 << 10);
        //STOP the sequence
        I2C1->CR1 |= (1 << 9);
      }
    } else if (current_i2c_state == I2C_STATE_MEASURE_WRITE) {
      if (SGPBufferIndex < 2) {
        if (logtrace_index< 10) log_trace[logtrace_index++] = 7;
          I2C1->DR = SGPCommandBuffer[SGPBufferIndex++];
        } else {

          if (!i2c_in_progress) {
            return;  // already handled this transfer's completion, ignore re-entry
          }

          if (logtrace_index< 10) log_trace[logtrace_index++] = 8;

          i2c_in_progress = 0;

          //STOP the sequence
          I2C1->CR1 |= (1 << 9);
          //Turn off ITBUFEN so that TxE doesn't cause any more triggers
          I2C1->CR2 &= ~(1 << 10);
      }
    } 

    return;
  } 
  
  //BTF
  if ((sr1 & (1 << 2)) && receiver) {

    if (!i2c_in_progress) {
      return;  // already handled this transfer's completion, ignore re-entry
    }

    if (currDataReceptionNumber == 0) {
      //Set ACK to low
      I2C1->CR1 &= ~(1 << 10);
      //Read Data N-2
      if (data_from_sensor_index < 1000) data_from_sensor[data_from_sensor_index++] = I2C1->DR;
      if (logtrace_index< 10) log_trace[logtrace_index++] = 9;
      currDataReceptionNumber++;
    } else {
      if (logtrace_index< 10) log_trace[logtrace_index++] = 10;

      //STOP the sequence
      I2C1->CR1 |= (1 << 9);

      if (data_from_sensor_index < 1000) data_from_sensor[data_from_sensor_index++] = I2C1->DR;
      if (data_from_sensor_index < 1000) data_from_sensor[data_from_sensor_index++] = I2C1->DR;
      
      currDataReceptionNumber = 0;

      i2c_in_progress = 0;

      //Turn off ITBUFEN so that TxE doesn't cause any more triggers
      I2C1->CR2 &= ~(1 << 10);
    }
    return;
  }

}