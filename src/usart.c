#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"

#include "usart.h"

int usart2_print(const char* str) {
  while(*str) {
    usart2_putchar(*str);
    str++;
  }

  return 0;
} 

uint16_t usart2_getchar (void){
    while ( USART_GetFlagStatus (USART2 , USART_FLAG_RXNE ) == RESET);
    return USART_ReceiveData(USART2);
}

int usart2_putchar (uint16_t c){
    while ( USART_GetFlagStatus (USART2 , USART_FLAG_TXE ) == RESET);
    USART_SendData(USART2, c);
    return 0;
}

int init_usart2(uint32_t baudrate) {
  GPIO_InitTypeDef GPIO_InitStruct;
  USART_InitTypeDef USART_InitStruct;

  // Enable peripherial clock for USART 2
  RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE);

  // Enable pins for TX / RX (GPIO PA2 PA3)
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE);

  // Initialize GPIO
  GPIO_StructInit(&GPIO_InitStruct);
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Set alternate function
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);


  // Initialize USART
  USART_StructInit(&USART_InitStruct);
  USART_InitStruct.USART_BaudRate = baudrate;
  USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_InitStruct.USART_StopBits = 1;
  USART_InitStruct.USART_Parity = USART_Parity_No;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_WordLength = USART_WordLength_8b;
  USART_Init(USART2, &USART_InitStruct);

  USART_Cmd(USART2, ENABLE);

  // Assume succes
  return 0;
}
