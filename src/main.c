#include "stm32f4xx.h"
#include "stm32f4xx_crc.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_spi.h"
#include "misc.h"
#include "pdm_filter.h"

static void initialize_leds(void);
static void initialize_crc(void);
static void initialize_i2s2(void);

int main(void) 
{
	
  initialize_leds();
  initialize_crc();
  initialize_i2s2();

  //TODO: Configure PB10 and PC3 Alternate Function as I2S
  //TODO: Init PDM Filter
  //TODO: Setup 1 msec timer to collect data from mic.


  while (1) {
	  // Main loop

  }

	return 0;
}

/// @brief Configure GPIO D pin 12-15 as output
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

/// @brief Initialize clocks for CRC peripheral
static void initialize_crc()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
}

/// @brief Initialize I2S peripheral
static void initialize_i2s2(void)
{
  I2S_InitTypeDef i2sInit;

  /* Enable the SPI clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
  
  /* Enable clocks to CLK & Data pin */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);

  /* Configure alternate function for GPIO Pins */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_SPI2); 
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_SPI2);

  /* Configure I2S */
  SPI_I2S_DeInit(SPI2);
  i2sInit.I2S_AudioFreq = 32000; // What should this be?
  i2sInit.I2S_Standard = I2S_Standard_LSB;
  i2sInit.I2S_DataFormat = I2S_DataFormat_16b;
  i2sInit.I2S_CPOL = I2S_CPOL_High;
  i2sInit.I2S_Mode = I2S_Mode_MasterRx;
  i2sInit.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
  I2S_Init(SPI2, &i2sInit);

  /* Configure & enable NVIC*/
  NVIC_InitTypeDef nvicInit;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
  nvicInit.NVIC_IRQChannel = SPI2_IRQn;
  nvicInit.NVIC_IRQChannelPreemptionPriority = 1;
  nvicInit.NVIC_IRQChannelSubPriority = 0;
  nvicInit.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init(&nvicInit);

  /* Enable the Rx buffer not empty interrupt */
  SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);

  /* TODO: Enable I2S w/ I2S_Cmd */

  /* TODO: Use DMA? */
}
