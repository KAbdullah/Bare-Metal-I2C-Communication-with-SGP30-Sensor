#include <stdint.h>
#include "stm32f446re.h"

void uart_general_init(volatile UART_Struct * uartaddress) {
  //Enable UART UE bit 
  uartaddress->CR1 |= (1 << 13);

  //Program M bit to define word length
  uartaddress->CR1 |= (1 << 12);

  //Program the number of stop bits
  uartaddress->CR2 |= (0 << 12);

  //Select DMA enable if multibuffer communication is to take place
  uartaddress->CR3 |= (0 << 6);
  uartaddress->CR3 |= (0 << 7);

  //Select Desired Buad rate of 115.2 KBps
  uartaddress->BRR = (8 << 4) | (11 << 0); 

  //Set the TE bit in UART CR1 to send and idle frame as first transmission
  

  //Write the data to send in the UART DR (which clears the TXE bit). - Will probably be moved to interrupt 

  //After writing the last data, wait until TC=1. this indicates the transmission of the last frame is complete. - Will probably be moved to interrupt

  //Enable Interrupt - may have to move this somewhere else, will probably put this in  a circular buffer function to allow it to take in bytes when it meets certain conditions

}

void UART4_IRQHandler(void) {

}