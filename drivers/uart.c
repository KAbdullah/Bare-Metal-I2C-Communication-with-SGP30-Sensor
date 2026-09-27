#include <stdint.h>
#include "stm32f446re.h"

void uart_general_init() {
  //Enable UART UE bit 

  //Program M bit to define word length

  //Program the number of stop bits

  //Select DMA enable if multibuffer communication is to take place

  //Select Desired Buad rate 

  uart_tx_init();
  uart_rx_init();

  //Enable Interrupt - may have to move this somewhere else

}

void uart_tx_init() {
  //Set the TE bit in UART CR1 to send and idle frame as first transmission 

  //Write the data to send in the UART DR (which clears the TXE bit). - Will probably be moved to interrupt 

  //After writing the last data, wait until TC=1. this indicates the transmission of the last frame is complete. - Will probably be moved to interrupt 

}

void uart_rx_init() {
  //Set the RE bit - This enables the receiver that begins searching for a start bit

}