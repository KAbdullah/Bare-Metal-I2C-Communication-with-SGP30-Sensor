#include <stdint.h>

extern volatile uint32_t _estack;
extern volatile uint32_t _Min_Heap_Size;
extern volatile uint32_t _Min_Stack_Size;
extern volatile uint32_t _stext;
extern volatile uint32_t _etext;
extern volatile uint32_t _srodata;
extern volatile uint32_t _erodata;
extern volatile uint32_t _sidata;
extern volatile uint32_t _sdata;
extern volatile uint32_t _edata;
extern volatile uint32_t _sbss;
extern volatile uint32_t _ebss;
extern volatile uint32_t _end;

extern int main(void);

void Reset_Handler(void) {
  uint32_t *src, *dst;

  src = (uint32_t *) &_sidata;
  dst = (uint32_t *) &_sdata;

  while (dst < (uint32_t *) &_edata) {
    *dst = *src;
    dst++;
    src++;
  }

  dst = (uint32_t *) &_sbss;

  while (dst < (uint32_t *) &_ebss) {
    *dst = 0;
    dst++;
  }

  main();
}

void Default_Handler(void) {
  while (1) {

  };
}

void NMI_Handler(void)          __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)    __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void)    __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void)     __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)          __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void)     __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)       __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)      __attribute__((weak, alias("Default_Handler")));

void WWDG_IRQHandler(void)                   __attribute__((weak, alias("Default_Handler")));
void PVD_IRQHandler(void)                    __attribute__((weak, alias("Default_Handler")));
void TAMP_STAMP_IRQHandler(void)             __attribute__((weak, alias("Default_Handler")));
void RTC_WKUP_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void FLASH_IRQHandler(void)                  __attribute__((weak, alias("Default_Handler")));
void RCC_IRQHandler(void)                    __attribute__((weak, alias("Default_Handler")));
void EXTI0_IRQHandler(void)                  __attribute__((weak, alias("Default_Handler")));
void EXTI1_IRQHandler(void)                  __attribute__((weak, alias("Default_Handler")));
void EXTI2_IRQHandler(void)                  __attribute__((weak, alias("Default_Handler")));
void EXTI3_IRQHandler(void)                  __attribute__((weak, alias("Default_Handler")));
void EXTI4_IRQHandler(void)                  __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream0_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream1_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream2_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream3_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream4_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream5_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream6_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void ADC_IRQHandler(void)                    __attribute__((weak, alias("Default_Handler")));
void CAN1_TX_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void CAN1_RX0_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void CAN1_RX1_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void CAN1_SCE_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void EXTI9_5_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void TIM1_BRK_TIM9_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void TIM1_UP_TIM10_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void TIM1_TRG_COM_TIM11_IRQHandler(void)     __attribute__((weak, alias("Default_Handler")));
void TIM1_CC_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void TIM2_IRQHandler(void)                   __attribute__((weak, alias("Default_Handler")));
void TIM3_IRQHandler(void)                   __attribute__((weak, alias("Default_Handler")));
void TIM4_IRQHandler(void)                   __attribute__((weak, alias("Default_Handler")));
void I2C1_EV_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void I2C1_ER_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void I2C2_EV_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void I2C2_ER_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void SPI1_IRQHandler(void)                   __attribute__((weak, alias("Default_Handler")));
void SPI2_IRQHandler(void)                   __attribute__((weak, alias("Default_Handler")));
void USART1_IRQHandler(void)                 __attribute__((weak, alias("Default_Handler")));
void USART2_IRQHandler(void)                 __attribute__((weak, alias("Default_Handler")));
void USART3_IRQHandler(void)                 __attribute__((weak, alias("Default_Handler")));
void EXTI15_10_IRQHandler(void)              __attribute__((weak, alias("Default_Handler")));
void RTC_Alarm_IRQHandler(void)              __attribute__((weak, alias("Default_Handler")));
void OTG_FS_WKUP_IRQHandler(void)            __attribute__((weak, alias("Default_Handler")));
void TIM8_BRK_TIM12_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
void TIM8_UP_TIM13_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void TIM8_TRG_COM_TIM14_IRQHandler(void)     __attribute__((weak, alias("Default_Handler")));
void TIM8_CC_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void DMA1_Stream7_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void FMC_IRQHandler(void)                    __attribute__((weak, alias("Default_Handler")));
void SDIO_IRQHandler(void)                   __attribute__((weak, alias("Default_Handler")));
void TIM5_IRQHandler(void)                   __attribute__((weak, alias("Default_Handler")));
void SPI3_IRQHandler(void)                   __attribute__((weak, alias("Default_Handler")));
void UART4_IRQHandler(void)                  __attribute__((weak, alias("Default_Handler")));
void UART5_IRQHandler(void)                  __attribute__((weak, alias("Default_Handler")));
void TIM6_DAC_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void TIM7_IRQHandler(void)                   __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream0_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream1_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream2_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream3_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream4_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void CAN2_TX_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void CAN2_RX0_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void CAN2_RX1_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void CAN2_SCE_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void OTG_FS_IRQHandler(void)                 __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream5_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream6_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void DMA2_Stream7_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void USART6_IRQHandler(void)                 __attribute__((weak, alias("Default_Handler")));
void I2C3_EV_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void I2C3_ER_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void OTG_HS_EP1_OUT_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
void OTG_HS_EP1_IN_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void OTG_HS_WKUP_IRQHandler(void)            __attribute__((weak, alias("Default_Handler")));
void OTG_HS_IRQHandler(void)                 __attribute__((weak, alias("Default_Handler")));
void DCMI_IRQHandler(void)                   __attribute__((weak, alias("Default_Handler")));
void FPU_IRQHandler(void)                    __attribute__((weak, alias("Default_Handler")));
void SPI4_IRQHandler(void)                   __attribute__((weak, alias("Default_Handler")));
void SAI1_IRQHandler(void)                   __attribute__((weak, alias("Default_Handler")));
void SAI2_IRQHandler(void)                   __attribute__((weak, alias("Default_Handler")));
void QuadSPI_IRQHandler(void)                __attribute__((weak, alias("Default_Handler")));
void HDMI_CEC_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void SPDIF_RX_IRQHandler(void)               __attribute__((weak, alias("Default_Handler")));
void FMPI2C1_Event_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void FMPI2C1_Error_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));


__attribute__((section(".isr_vector"), used))
void (*IV[])(void) = {
  (void (*) (void)) &_estack,
  Reset_Handler,                // 2. Reset Handler
  NMI_Handler,                  // 3. NMI Handler
  HardFault_Handler,            // 4. Hard Fault Handler
  MemManage_Handler,            // 5. MPU Fault Handler
  BusFault_Handler,             // 6. Bus Fault Handler
  UsageFault_Handler,           // 7. Usage Fault Handler
  0, 0, 0, 0,                   // 8-11. Reserved Slots
  SVC_Handler,                  // 12. SVCall Handler
  DebugMon_Handler,             // 13. Debug Monitor Handler
  0,                            // 14. Reserved Slot
  PendSV_Handler,               // 15. PendSV Handler
  SysTick_Handler,              // 16. SysTick Handler

  // Peripheral Interrupts (STM32F446xx Hardware Layout)
  WWDG_IRQHandler,                   
  PVD_IRQHandler,                    
  TAMP_STAMP_IRQHandler,             
  RTC_WKUP_IRQHandler,               
  FLASH_IRQHandler,                  
  RCC_IRQHandler,                    
  EXTI0_IRQHandler,                  
  EXTI1_IRQHandler,                  
  EXTI2_IRQHandler,                  
  EXTI3_IRQHandler,                  
  EXTI4_IRQHandler,                  
  DMA1_Stream0_IRQHandler,           
  DMA1_Stream1_IRQHandler,           
  DMA1_Stream2_IRQHandler,           
  DMA1_Stream3_IRQHandler,           
  DMA1_Stream4_IRQHandler,           
  DMA1_Stream5_IRQHandler,           
  DMA1_Stream6_IRQHandler,           
  ADC_IRQHandler,                    
  CAN1_TX_IRQHandler,                
  CAN1_RX0_IRQHandler,               
  CAN1_RX1_IRQHandler,               
  CAN1_SCE_IRQHandler,               
  EXTI9_5_IRQHandler,                
  TIM1_BRK_TIM9_IRQHandler,          
  TIM1_UP_TIM10_IRQHandler,          
  TIM1_TRG_COM_TIM11_IRQHandler,     
  TIM1_CC_IRQHandler,                
  TIM2_IRQHandler,                   
  TIM3_IRQHandler,                   
  TIM4_IRQHandler,                   
  I2C1_EV_IRQHandler,                
  I2C1_ER_IRQHandler,                
  I2C2_EV_IRQHandler,                
  I2C2_ER_IRQHandler,                
  SPI1_IRQHandler,                   
  SPI2_IRQHandler,                   
  USART1_IRQHandler,                 
  USART2_IRQHandler,                 
  USART3_IRQHandler,                 
  EXTI15_10_IRQHandler,              
  RTC_Alarm_IRQHandler,              
  OTG_FS_WKUP_IRQHandler,            
  TIM8_BRK_TIM12_IRQHandler,         
  TIM8_UP_TIM13_IRQHandler,          
  TIM8_TRG_COM_TIM14_IRQHandler,     
  TIM8_CC_IRQHandler,                
  DMA1_Stream7_IRQHandler,           
  FMC_IRQHandler,                    
  SDIO_IRQHandler,                   
  TIM5_IRQHandler,                   
  SPI3_IRQHandler,                   
  UART4_IRQHandler,                  
  UART5_IRQHandler,                  
  TIM6_DAC_IRQHandler,               
  TIM7_IRQHandler,                   
  DMA2_Stream0_IRQHandler,           
  DMA2_Stream1_IRQHandler,           
  DMA2_Stream2_IRQHandler,           
  DMA2_Stream3_IRQHandler,           
  DMA2_Stream4_IRQHandler,           
  0, 0,                              // Reserved Slots
  CAN2_TX_IRQHandler,                
  CAN2_RX0_IRQHandler,               
  CAN2_RX1_IRQHandler,               
  CAN2_SCE_IRQHandler,               
  OTG_FS_IRQHandler,                 
  DMA2_Stream5_IRQHandler,           
  DMA2_Stream6_IRQHandler,           
  DMA2_Stream7_IRQHandler,           
  USART6_IRQHandler,                 
  I2C3_EV_IRQHandler,                
  I2C3_ER_IRQHandler,                
  OTG_HS_EP1_OUT_IRQHandler,         
  OTG_HS_EP1_IN_IRQHandler,          
  OTG_HS_WKUP_IRQHandler,            
  OTG_HS_IRQHandler,                 
  DCMI_IRQHandler,                   
  0, 0,                              // Reserved Slots
  FPU_IRQHandler,                    
  0, 0,                              // Reserved Slots
  SPI4_IRQHandler,                   
  0, 0,                              // Reserved Slots
  SAI1_IRQHandler,                   
  0, 0, 0,                           // Reserved Slots
  SAI2_IRQHandler,                   
  QuadSPI_IRQHandler,                
  HDMI_CEC_IRQHandler,               
  SPDIF_RX_IRQHandler,               
  FMPI2C1_Event_IRQHandler,          
  FMPI2C1_Error_IRQHandler
};