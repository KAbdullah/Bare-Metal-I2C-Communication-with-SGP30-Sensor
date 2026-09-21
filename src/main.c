#include <stdint.h>
#include "../drivers/stm32f446re.h"
#include "../drivers/gpio.c"
#include "../drivers/delay.c"

void I2C1_EV_IRQHandler (void);

//RCC - over here when we put the outer *, we're saying that I want to immediately start working in the memory
// and not work with a pointer that points to a memory, so in subsequent uses of RCC, I'm working with memory 
#define RCC 0x40023800

#define RCC_CR (*((volatile uint32_t *)(RCC + 0x00)))
#define RCC_CFGR (*((volatile uint32_t *)(RCC + 0x08)))
#define RCC_AHB1ENR (*((volatile uint32_t *)(RCC + 0x30)))
#define RCC_APB1ENR (*((volatile uint32_t *)(RCC + 0x40)))


typedef struct {
  uint32_t I2C_CR1;
  uint32_t I2C_CR2;
  uint32_t I2C_OAR1;
  uint32_t I2C_OAR2;
  uint32_t I2C_DR;
  uint32_t I2C_SR1;
  uint32_t I2C_SR2;
  uint32_t I2C_CCR;
  uint32_t I2C_TRISE;
  uint32_t I2C_FLTR;
} I2C_Struct;

typedef enum {
  I2C_STATE_INIT_WRITE,
  I2C_STATE_MEASURE_WRITE,
  I2C_STATE_MEASURE_READ,
} i2c_state_t;

static volatile i2c_state_t current_i2c_state = I2C_STATE_INIT_WRITE;

#define I2C1 ((volatile I2C_Struct *) 0x40005400)

//this will belong to the SGP30.c file
static volatile uint8_t readorwrite = 0;
static volatile uint8_t first_time_start = 1;

//SGP30 measurement command 
static uint8_t SGPCommandBuffer[] = {0x20, 0x08};
static volatile uint8_t SGPBufferIndex = 0;
static uint8_t SGPInnitBuffer[] = {0x20, 0x03};
static volatile uint8_t SGPInnitIndex = 0;

static volatile uint8_t currDataReceptionNumber = 0;
static volatile uint8_t i2c_in_progress = 0;


int main (void) {

  //Got to get the clock started before touching the pins, otherwise you exceptions flags called with interrupt 
  //Reset
  RCC_CR &= ~((0b111111 << 24) | (0b1111 << 16) | (0b10000 << 3) | (0b11 << 0));

  //Turn HSIO clock on
  RCC_CR |= (1 << 0);

  //Blocking until HSI starts
  while (!(RCC_CR & (1 << 1)));

  //Choose system clock to use HSI
  RCC_CFGR |= (0b00 << 0);

  //Blocking until sys switched to HSI
  while (1) {
    uint32_t address = RCC_CFGR;
    uint32_t extractsws = (address >> 2) & 0x3;
    if (extractsws == 0x00) {
      break;
    }
  }

  //Turn on GPIOB clock
  RCC_AHB1ENR |= (1 << 1);


  gpio_init(GPIOB);


  //Turn on I2C clock
  RCC_APB1ENR |= (1 << 21);

  // Force a complete hardware reset of the I2C1 peripheral
  I2C1->I2C_CR1 |= (1 << 15);  // Set SWRST
  for (volatile int i = 0; i < 1000; i++); // Small delay
  I2C1->I2C_CR1 &= ~(1 << 15); // Clear SWRST

  //STARTING THE I2C
  //Program the peripheral input clock
  I2C1->I2C_CR2 &= ~(0b11111 << 0);
  I2C1->I2C_CR2 |= (0b10000 << 0);
  
  //Configure the clock control registers
  //reset
  I2C1->I2C_CCR &= ~(0xFFFF << 0);
  //Select FM mode
  I2C1->I2C_CCR |= (1 << 15);
  
  //Select DUTY, since 16MHz not multiple of 10Mhz, we do 2
  I2C1->I2C_CCR &= ~(1 << 14);
  I2C1->I2C_CCR |= (0 << 14);

  //Set CCR, how? A few things to clear up: 
  // 1) Tlow and Thigh indicate how many ticks aka clocks that they need to stay at their respective levels, so Tlow = 2Thigh means Tlow will be at that level for two PCLK (peripheral clock cycles)
  // 2) Ttotal = Tlow + Thigh 
  // 3) CCR = Ttotal / TPCLK => CCR = Total / 3 * TPCLK
  I2C1->I2C_CCR |= (14 << 0);

  //Set TRISE -> 300 / 62.5 = 4.8 => 4 + 1 => 5; 4 * 62.5  = 250ns or 4 SYSCLK ticks is the max safe limit to rise from 0 to 1
  I2C1->I2C_TRISE &= ~(0b111111 << 0);
  I2C1->I2C_TRISE |= (0b000101);

  //Enable interrupt ITEVTEN and ITBUFEN
  I2C1->I2C_CR2 |= (1 << 9) | (1 << 10);

  //Peripheral start
  I2C1->I2C_CR1 |= (1 << 0);

  __asm("CPSIE i"); // Change Processor State to enable interrupt 
  //0xE000E100 is the NVIC base address 
  *((volatile uint32_t *)0xE000E100) |= (1 << 31); //I2C interrupt is number 31, so enable it to 1.

  //Start condition to initialize the SGP30
  i2c_in_progress = 1;
  I2C1->I2C_CR1 |= (1 << 8);

  //Start condition to get into controller mode
  // 0 means write, 1 means read
  //Turn on ITBUFEN because it was turned off
  I2C1->I2C_CR2 |= (1 << 10);
  readorwrite = 0;

  //Reset measurement and Init indexes
  SGPBufferIndex = 0;
  SGPInnitIndex = 0;

  while (i2c_in_progress);

  delay(10);

  while (1) {

    //Start again
    current_i2c_state = I2C_STATE_MEASURE_WRITE;
    i2c_in_progress = 1;
    I2C1->I2C_CR1 |= (1 << 8);

    while (i2c_in_progress);

    //Measurement time is 12ms
    delay(12);

    //Turn on ITBUFEN because it was turned off
    I2C1->I2C_CR2 |= (1 << 10);
    I2C1->I2C_CR1 |= (1 << 10);
    //Send another start condition here basically to start reading data
    current_i2c_state = I2C_STATE_MEASURE_READ;
    readorwrite = 1;
    currDataReceptionNumber = 0;
    i2c_in_progress = 1;
    I2C1->I2C_CR1 |= (1 << 8);

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
  volatile uint16_t sr1 = I2C1->I2C_SR1;

  //I do only 1, then everything else is set to 0, so 0b0000000000000001
  if (sr1 & (1 << 0)) {
    (void)I2C1->I2C_SR1;
    //THE SGP30 uses 7 bits addressing, so we send the address starting at bit 1 and reserve the LSB as 0 (reset) to enter transmitter mode
    if (readorwrite) {
      if (logtrace_index< 10) log_trace[logtrace_index++] = 1;
      I2C1->I2C_DR = ((0x58 << 1) | (1 << 0));
    } else {
      if (logtrace_index< 10) log_trace[logtrace_index++] = 2;
      I2C1->I2C_DR = (0x58 << 1);
    }
    return;
  }

  // Data register is empty here
  if (sr1 & (1 << 1)) {
    //Reset the ADDR bit
    (void)I2C1->I2C_SR1;
    (void)I2C1->I2C_SR2;

    if (!readorwrite) {
      if (first_time_start) {
        if (logtrace_index< 10) log_trace[logtrace_index++] = 3;
        I2C1->I2C_DR = SGPInnitBuffer[SGPInnitIndex++];
      } else {
        if (logtrace_index< 10) log_trace[logtrace_index++] = 4;
        I2C1->I2C_DR = SGPCommandBuffer[SGPBufferIndex++];
      }
    }
    return;
  }

  //If data register is empty so TxE = 1 (because we are in transmitter mode) we write measurement command 
  //In transmitter mode, TxE
  if (sr1 & (1 << 7) && !readorwrite) {
    if (current_i2c_state == I2C_STATE_INIT_WRITE) {
      if (SGPInnitIndex < 2) {
        I2C1->I2C_DR = SGPInnitBuffer[SGPInnitIndex++];
        if (logtrace_index< 10) log_trace[logtrace_index++] = 5;
      } else {

        if (!i2c_in_progress) {
          return;  // already handled this transfer's completion, ignore re-entry
        }

        if (logtrace_index< 10) log_trace[logtrace_index++] = 6;
        //Turn off ITBUFEN so that TxE doesn't cause any more triggers

        i2c_in_progress = 0;

        first_time_start = 0;

        I2C1->I2C_CR2 &= ~(1 << 10);
        //STOP the sequence
        I2C1->I2C_CR1 |= (1 << 9);
      }
    } else if (current_i2c_state == I2C_STATE_MEASURE_WRITE) {
      if (SGPBufferIndex < 2) {
        if (logtrace_index< 10) log_trace[logtrace_index++] = 7;
          I2C1->I2C_DR = SGPCommandBuffer[SGPBufferIndex++];
        } else {

          if (!i2c_in_progress) {
            return;  // already handled this transfer's completion, ignore re-entry
          }

          if (logtrace_index< 10) log_trace[logtrace_index++] = 8;

          i2c_in_progress = 0;

          //STOP the sequence
          I2C1->I2C_CR1 |= (1 << 9);
          //Turn off ITBUFEN so that TxE doesn't cause any more triggers
          I2C1->I2C_CR2 &= ~(1 << 10);
      }
    } 

    return;
  } 
  
  //BTF
  if ((sr1 & (1 << 2)) && readorwrite) {

    if (!i2c_in_progress) {
      return;  // already handled this transfer's completion, ignore re-entry
    }

    if (currDataReceptionNumber == 0) {
      //Set ACK to low
      I2C1->I2C_CR1 &= ~(1 << 10);
      //Read Data N-2
      if (data_from_sensor_index < 1000) data_from_sensor[data_from_sensor_index++] = I2C1->I2C_DR;
      if (logtrace_index< 10) log_trace[logtrace_index++] = 9;
      currDataReceptionNumber++;
    } else {
      if (logtrace_index< 10) log_trace[logtrace_index++] = 10;

      //STOP the sequence
      I2C1->I2C_CR1 |= (1 << 9);

      if (data_from_sensor_index < 1000) data_from_sensor[data_from_sensor_index++] = I2C1->I2C_DR;
      if (data_from_sensor_index < 1000) data_from_sensor[data_from_sensor_index++] = I2C1->I2C_DR;
      
      currDataReceptionNumber = 0;

      i2c_in_progress = 0;

      //Turn off ITBUFEN so that TxE doesn't cause any more triggers
      I2C1->I2C_CR2 &= ~(1 << 10);
    }
    return;
  }

  
  
  // //   //Write the first byte of the measurement command
  // //   I2C1->I2C_DR = 0x20;
  // //   if (I2C1->I2C_SR1 & (1 << 7)) {
  // //     I2C1->I2C_DR = 0x08;
  // //   }
  // // }
}