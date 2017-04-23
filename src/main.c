#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

static void initialize_leds(void);
static void initialize_button(void);

int main(void) {
	
	initialize_leds();
	initialize_button();

  while (1) {
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)) {
			GPIO_SetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14);
		} else {
			GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14);
		}
  }

	return 0;
}

static void initialize_leds(void) {
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

static void initialize_button(void) {
	// Structure to hold GPIO settings
	GPIO_InitTypeDef GPIO_InitDef;

	// Enable clock on GPIOA port
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	// Settings
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP; // Push pull
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;

	// Initialize Pin
	GPIO_Init(GPIOA, &GPIO_InitDef);

}
