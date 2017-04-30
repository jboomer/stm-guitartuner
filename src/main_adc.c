#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_it.h"
#include "misc.h"

#include <stdbool.h>

#define ADC_BUFFERSIZE 4096

static void initialize_leds();
static void initialize_button();
static void initialize_adc();
static void handle_buffer_half_full();
static void handle_buffer_full();
static void handle_transfer_done();

/**
 * Buffer to hold the samples
 */
static __IO uint16_t buffer[ADC_BUFFERSIZE];
static __IO bool transferDone = false;

int main(void) 
{
	
    initialize_leds();
    initialize_button();
    initialize_adc();

    GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

    while(! GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
    {

    }

    GPIO_SetBits(GPIOD, GPIO_Pin_12);

    // Start ADC
    /* Enable ADC1 DMA */
    ADC_SoftwareStartConv(ADC1);
    ADC_DMACmd(ADC1, ENABLE);

    while (1) {
        // mainloop
        if (transferDone) {
            handle_transfer_done();
        }
    }

	return 0;
}

static void initialize_leds() 
{
	// Structure to hold GPIO settings
	GPIO_InitTypeDef GPIO_InitDef;

	// Enable clock on GPIOD port
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	// Settings
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP; // Push pull
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;

	// Initialize Pin
	GPIO_Init(GPIOD, &GPIO_InitDef);
}

static void initialize_button() {
	// Structure to hold GPIO settings
	GPIO_InitTypeDef GPIO_InitDef;

	// Enable clock on GPIOA port
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	// Settings
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP; // Push pull
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_2MHz;

	// Initialize Pin
	GPIO_Init(GPIOA, &GPIO_InitDef);
}

static void initialize_adc()
{

  // Clocks, GPIOC for ADC pin
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  // ADC Channel 11 -> PC1
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // Interrupt controller
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* TIM2 is clocked by 42 MHz APB1 bus, but since TIMPRE bit is not set
   * and APB1 prescaler is not 1, clock speed is 2x42 Mhz*/
  TIM_TimeBaseInitTypeDef TIM_InitStruct;

  TIM_TimeBaseStructInit(&TIM_InitStruct);
  TIM_InitStruct.TIM_Prescaler = (uint16_t)(84 - 1); // Gives 1 MHz
  TIM_InitStruct.TIM_Period = 100 - 1; // Gives 10 KHz
  TIM_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_InitStruct.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM2, &TIM_InitStruct);

  /* Enable TRGO event */
  TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

  /* Enable Timer 2 */
  TIM_Cmd(TIM2, ENABLE);

  // DMA
  DMA_InitTypeDef DMA_InitStructure;
  
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&buffer[0];
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = ADC_BUFFERSIZE; // Count of 16-bit words
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  
  /* Enable DMA Stream Half / Transfer Complete interrupt */
  DMA_ITConfig(DMA2_Stream0, DMA_IT_TC | DMA_IT_HT, ENABLE);
  
  /* DMA2_Stream0 enable */
  DMA_Cmd(DMA2_Stream0, ENABLE);

  // ADC
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  ADC_InitTypeDef ADC_InitStructure;
  
  /* ADC Common Init */
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);
  
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE; // 1 Channel
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // Conversions Triggered
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  
  /* ADC1 regular channel 11 configuration */
  ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_15Cycles); // PC1
  
  /* Enable DMA request after last transfer (Single-ADC mode) */
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  
}

void DMA2_Stream0_IRQHandler(void) // 10 kHz , buffersize 4096, about 250ms per half buffer
{
  /* Test on DMA Stream Half Transfer interrupt */
  if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_HTIF0))
  {
    /* Clear DMA Stream Half Transfer interrupt pending bit */
    DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_HTIF0);
  
    // Add code here to process first half of buffer (ping)
    handle_buffer_half_full();
  }
  
  /* Test on DMA Stream Transfer Complete interrupt */
  if(DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0))
  {
    /* Clear DMA Stream Transfer Complete interrupt pending bit */
    DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
    
    // Add code here to process second half of buffer (pong)
    handle_buffer_full();
  }
}

static void handle_buffer_half_full()
{
    GPIO_SetBits(GPIOD, GPIO_Pin_13);
}

static void handle_buffer_full()
{
    GPIO_SetBits(GPIOD, GPIO_Pin_14);
    transferDone = true;
}

static void handle_transfer_done()
{
    GPIO_SetBits(GPIOD, GPIO_Pin_15);
}
