/* Michael Pratt <michael@pratt.im> */

/* Generic system call library for STM32F4-DISCOVERY
 * Eliminates need for the huge STM libraries */

#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

/* We will be wanting some typedefs */
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;

/* System Initialization */
extern void SystemInit(void);

/* Exception Handlers */
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

/* Memory map defines */

/* Flash Map */
#define FLASH_BASE          (uint32_t) (0x08000000)                     /* Flash Base Address */

/* Peripheral Map */
#define PERIPH_BASE         (uint32_t) (0x40000000)                     /* Peripheral base address */
#define APB1PERIPH_BASE     (PERIPH_BASE)
#define AHB1PERIPH_BASE     (PERIPH_BASE + 0x00020000)

#define PWR_BASE            (APB1PERIPH_BASE + 0x7000)                  /* Power Control base address */
#define RCC_BASE            (AHB1PERIPH_BASE + 0x3800)                  /* Reset and Clock Control base address */
#define FLASH_R_BASE        (AHB1PERIPH_BASE + 0x3C00)                  /* Flash registers base address */

/* System Control Map */
#define SCS_BASE            (uint32_t) (0xE000E000)                     /* System Control Space Base Address */
#define SCB_BASE            (SCS_BASE + 0x0D00)                         /* System Control Block Base Address */

/* Power Control (PWR) */
#define PWR_CR              *(volatile uint32_t *) (PWR_BASE + 0x00)    /* Power Control Register */
#define PWR_CSR             *(volatile uint32_t *) (PWR_BASE + 0x04)    /* Power Control/Status Register */

/* Reset and Clock Control (RCC) */
#define RCC_CR              *(volatile uint32_t *) (RCC_BASE + 0x00)    /* Clock Control Register */
#define RCC_PLLCFGR         *(volatile uint32_t *) (RCC_BASE + 0x04)    /* PLL Configuration Register */
#define RCC_CFGR            *(volatile uint32_t *) (RCC_BASE + 0x08)    /* Clock Configuration Register */
#define RCC_CIR             *(volatile uint32_t *) (RCC_BASE + 0x0C)    /* Clock Interrupt Register */
#define RCC_APB1ENR         *(volatile uint32_t *) (RCC_BASE + 0x40)    /* APB1 Peripheral Clock Enable Register */

/* Flash Registers (FLASH) */
#define FLASH_ACR           *(volatile uint32_t *) (FLASH_R_BASE + 0x00)/* Flash Access Control Register */

/* System Control Block (SCB) */
#define SCB_VTOR            *(volatile uint32_t *) (SCB_BASE + 0x008)   /* Vector Table Offset Register */
#define SCB_CPACR           *(volatile uint32_t *) (SCB_BASE + 0x088)   /* Coprocessor Access Control Register */



/* Bit Masks - See RM0090 Reference Manual for STM32F4 for details */
#define  PWR_CR_VOS             (uint16_t) (0x4000)         /* Regulator voltage scaling output selection */

#define  RCC_CR_HSEON           (uint32_t) (0x00010000)     /* Enable HSE */
#define  RCC_CR_HSERDY          (uint32_t) (0x00020000)     /* HSE Ready */
#define  RCC_CR_PLLON           (uint32_t) (0x01000000)     /* Main PLL Enable */
#define  RCC_CR_PLLRDY          (uint32_t) (0x02000000)     /* Main PLL clock ready */

#define  RCC_CFGR_SW            (uint32_t) (0x00000003)     /* SW[1:0] bits (System clock Switch) */
#define  RCC_CFGR_SW_PLL        (uint32_t) (0x00000002)     /* PLL selected as system clock */
#define  RCC_CFGR_SWS           (uint32_t) (0x0000000C)     /* SWS[1:0] bits (System Clock Switch Status) */
#define  RCC_CFGR_SWS_PLL       (uint32_t) (0x00000008)     /* PLL used as system clock */
#define  RCC_CFGR_HPRE_DIV1     (uint32_t) (0x00000000)     /* SYSCLK not divided (highest frequency) */
#define  RCC_CFGR_PPRE2_DIV2    (uint32_t) (0x00008000)     /* HCLK divided by 2 */
#define  RCC_CFGR_PPRE1_DIV4    (uint32_t) (0x00001400)     /* HCLK divided by 4 */

#define  RCC_PLLCFGR_PLLSRC_HSE (uint32_t) (0x00400000)     /* HSE oscillator selected as clock entry */

#define  RCC_APB1ENR_PWREN      (uint32_t) (0x10000000)     /* Power Interface Clock Enable */

#define  FLASH_ACR_ICEN         (uint32_t) (0x00000200)     /* Instruction Cache Enable */
#define  FLASH_ACR_DCEN         (uint32_t) (0x00000400)     /* Data Cache Enable */
#define  FLASH_ACR_LATENCY_5WS  (uint32_t) (0x00000005)     /* 5 Wait States Latency */


#endif /* SYSTEM_H_INCLUDED */
