/*
 * This example demonstrates how to configure control clock
 * and shows how much FLASH might deteriorate execution speed
 */

#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_system.h"
#include "stm32f0xx_ll_bus.h"
#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_utils.h"
#include "stm32f0xx_ll_tim.h"
#include "stm32f0xx_ll_rtc.h"

#define FLASH_0LAT_DELAY0LAT
//#define FLASH_0LAT_DELAY1LAT
//#define FLASH_1LAT_DELAY0LAT
//#define FLASH_1LAT_DELAY1LAT

/**
  * System Clock Configuration
  * The system Clock is configured as follow :
  *    System Clock source            = PLL (HSI/2)
  *    SYSCLK(Hz)                     = 48000000
  *    HCLK(Hz)                       = 48000000
  *    AHB Prescaler                  = 1
  *    APB1 Prescaler                 = 1
  *    HSI Frequency(Hz)              = 8000000
  *    PLLMUL                         = 12
  *    Flash Latency(WS)              = 1
  */
static void rcc_config()
{
    /* Set FLASH latency */
#if defined(FLASH_0LAT_DELAY0LAT) || defined(FLASH_0LAT_DELAY1LAT)
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
#else
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
#endif

    /* Enable HSI and wait for activation*/
    LL_RCC_HSI_Enable();
    while (LL_RCC_HSI_IsReady() != 1);

    /* Main PLL configuration and activation */
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2,
                                LL_RCC_PLL_MUL_12);

    LL_RCC_PLL_Enable();
    while (LL_RCC_PLL_IsReady() != 1);

    /* Sysclk activation on the main PLL */
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);

    /* Set APB1 prescaler */
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

    /* Update CMSIS variable (which can be updated also
     * through SystemCoreClockUpdate function) */
    SystemCoreClock = 48000000;

    LL_RCC_LSE_Enable ();
}

/*
 * Clock on GPIOC and set pin with Blue led connected
 */
static void gpio_config(void)
{
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_8, LL_GPIO_MODE_OUTPUT);

    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_3, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_4, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_7, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_8, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_9, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_13, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_14, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_15, LL_GPIO_MODE_OUTPUT);

    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_6, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_8, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_11, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_12, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_13, LL_GPIO_MODE_OUTPUT);
    return;
}

RTC_TypeDef rtc_time;

static void rtc_config(void)
{
    LL_RCC_SetRTCClockSource (LL_RCC_RTC_CLKSOURCE_LSE);
    LL_RCC_EnableRTC ();
    LL_RTC_EnterInitMode (&rtc_time);
    LL_RTC_DisableWriteProtection (&rtc_time);
    LL_RTC_TIME_Config (&rtc_time, LL_RTC_TIME_FORMAT_AM_OR_24, 0x00, 0x00, 0x00);
    LL_RTC_DATE_SetMonth (&rtc_time, LL_RTC_MONTH_MARCH);
    LL_RTC_DATE_SetDay (&rtc_time, 0x16);
    LL_RTC_EnableWriteProtection (&rtc_time);
    LL_RTC_ExitInitMode (&rtc_time);
    LL_RTC_WaitForSynchro (&rtc_time);
}

enum { A = LL_GPIO_PIN_1,
       B = LL_GPIO_PIN_3,
       C = LL_GPIO_PIN_12,
       D = LL_GPIO_PIN_8,
       E = LL_GPIO_PIN_9,
       F = LL_GPIO_PIN_2,
       G = LL_GPIO_PIN_13,
       First_num = LL_GPIO_PIN_4,
       Second_num = LL_GPIO_PIN_6,
       Third_num = LL_GPIO_PIN_5,
       Fourth_num = LL_GPIO_PIN_7 };

static void print_simple (char n) {
    switch (n) {
    case 0: 
        LL_GPIO_SetOutputPin(GPIOA, A);
        LL_GPIO_SetOutputPin(GPIOA, B);
        LL_GPIO_SetOutputPin(GPIOA, C);
        LL_GPIO_SetOutputPin(GPIOA, D);
        LL_GPIO_SetOutputPin(GPIOA, E);
        LL_GPIO_SetOutputPin(GPIOA, F);
        LL_GPIO_ResetOutputPin(GPIOA, G);
	break;
    case 1:
        LL_GPIO_ResetOutputPin(GPIOA, A);
        LL_GPIO_SetOutputPin(GPIOA, B);
        LL_GPIO_SetOutputPin(GPIOA, C);
        LL_GPIO_ResetOutputPin(GPIOA, D);
        LL_GPIO_ResetOutputPin(GPIOA, E);
        LL_GPIO_ResetOutputPin(GPIOA, F);
        LL_GPIO_ResetOutputPin(GPIOA, G);
	break;
    case 2:
        LL_GPIO_SetOutputPin(GPIOA, A);
        LL_GPIO_SetOutputPin(GPIOA, B);
        LL_GPIO_ResetOutputPin(GPIOA, C);
        LL_GPIO_SetOutputPin(GPIOA, D);
        LL_GPIO_SetOutputPin(GPIOA, E);
        LL_GPIO_ResetOutputPin(GPIOA, F);
        LL_GPIO_SetOutputPin(GPIOA, G);
	break;
    case 3:
        LL_GPIO_SetOutputPin(GPIOA, A);
        LL_GPIO_SetOutputPin(GPIOA, B);
        LL_GPIO_SetOutputPin(GPIOA, C);
        LL_GPIO_SetOutputPin(GPIOA, D);
        LL_GPIO_ResetOutputPin(GPIOA, E);
        LL_GPIO_ResetOutputPin(GPIOA, F);
        LL_GPIO_SetOutputPin(GPIOA, G);
	break;
    case 4:
        LL_GPIO_ResetOutputPin(GPIOA, A);
        LL_GPIO_SetOutputPin(GPIOA, B);
        LL_GPIO_SetOutputPin(GPIOA, C);
        LL_GPIO_ResetOutputPin(GPIOA, D);
        LL_GPIO_ResetOutputPin(GPIOA, E);
        LL_GPIO_SetOutputPin(GPIOA, F);
        LL_GPIO_SetOutputPin(GPIOA, G);
	break;
    case 5:
        LL_GPIO_SetOutputPin(GPIOA, A);
        LL_GPIO_ResetOutputPin(GPIOA, B);
        LL_GPIO_SetOutputPin(GPIOA, C);
        LL_GPIO_SetOutputPin(GPIOA, D);
        LL_GPIO_ResetOutputPin(GPIOA, E);
        LL_GPIO_SetOutputPin(GPIOA, F);
        LL_GPIO_SetOutputPin(GPIOA, G);
	break;
    case 6:
        LL_GPIO_SetOutputPin(GPIOA, A);
        LL_GPIO_ResetOutputPin(GPIOA, B);
        LL_GPIO_SetOutputPin(GPIOA, C);
        LL_GPIO_SetOutputPin(GPIOA, D);
        LL_GPIO_SetOutputPin(GPIOA, E);
        LL_GPIO_SetOutputPin(GPIOA, F);
        LL_GPIO_SetOutputPin(GPIOA, G);
	break;
    case 7:
        LL_GPIO_SetOutputPin(GPIOA, A);
        LL_GPIO_SetOutputPin(GPIOA, B);
        LL_GPIO_SetOutputPin(GPIOA, C);
        LL_GPIO_ResetOutputPin(GPIOA, D);
        LL_GPIO_ResetOutputPin(GPIOA, E);
        LL_GPIO_ResetOutputPin(GPIOA, F);
        LL_GPIO_ResetOutputPin(GPIOA, G);
	break;
    case 8:
        LL_GPIO_SetOutputPin(GPIOA, A);
        LL_GPIO_SetOutputPin(GPIOA, B);
        LL_GPIO_SetOutputPin(GPIOA, C);
        LL_GPIO_SetOutputPin(GPIOA, D);
        LL_GPIO_SetOutputPin(GPIOA, E);
        LL_GPIO_SetOutputPin(GPIOA, F);
        LL_GPIO_SetOutputPin(GPIOA, G);
	break;
    case 9:
        LL_GPIO_SetOutputPin(GPIOA, A);
        LL_GPIO_SetOutputPin(GPIOA, B);
        LL_GPIO_SetOutputPin(GPIOA, C);
        LL_GPIO_SetOutputPin(GPIOA, D);
        LL_GPIO_ResetOutputPin(GPIOA, E);
        LL_GPIO_SetOutputPin(GPIOA, F);
        LL_GPIO_SetOutputPin(GPIOA, G);
	break;
    case 10:
        LL_GPIO_SetOutputPin(GPIOA, A);
        LL_GPIO_SetOutputPin(GPIOA, B);
        LL_GPIO_SetOutputPin(GPIOA, C);
        LL_GPIO_ResetOutputPin(GPIOA, D);
        LL_GPIO_SetOutputPin(GPIOA, E);
        LL_GPIO_SetOutputPin(GPIOA, F);
        LL_GPIO_SetOutputPin(GPIOA, G);
	break;
    case 11:
        LL_GPIO_ResetOutputPin(GPIOA, A);
        LL_GPIO_ResetOutputPin(GPIOA, B);
        LL_GPIO_SetOutputPin(GPIOA, C);
        LL_GPIO_SetOutputPin(GPIOA, D);
        LL_GPIO_SetOutputPin(GPIOA, E);
        LL_GPIO_SetOutputPin(GPIOA, F);
        LL_GPIO_SetOutputPin(GPIOA, G);
	break;
    case 12:
        LL_GPIO_SetOutputPin(GPIOA, A);
        LL_GPIO_ResetOutputPin(GPIOA, B);
        LL_GPIO_ResetOutputPin(GPIOA, C);
        LL_GPIO_SetOutputPin(GPIOA, D);
        LL_GPIO_SetOutputPin(GPIOA, E);
        LL_GPIO_SetOutputPin(GPIOA, F);
        LL_GPIO_ResetOutputPin(GPIOA, G);
	break;
    case 13:
        LL_GPIO_ResetOutputPin(GPIOA, A);
        LL_GPIO_SetOutputPin(GPIOA, B);
        LL_GPIO_SetOutputPin(GPIOA, C);
        LL_GPIO_SetOutputPin(GPIOA, D);
        LL_GPIO_SetOutputPin(GPIOA, E);
        LL_GPIO_ResetOutputPin(GPIOA, F);
        LL_GPIO_SetOutputPin(GPIOA, G);
	break;
    case 14:
        LL_GPIO_SetOutputPin(GPIOA, A);
        LL_GPIO_ResetOutputPin(GPIOA, B);
        LL_GPIO_ResetOutputPin(GPIOA, C);
        LL_GPIO_SetOutputPin(GPIOA, D);
        LL_GPIO_SetOutputPin(GPIOA, E);
        LL_GPIO_SetOutputPin(GPIOA, F);
        LL_GPIO_SetOutputPin(GPIOA, G);
	break;
    case 15:
        LL_GPIO_SetOutputPin(GPIOA, A);
        LL_GPIO_ResetOutputPin(GPIOA, B);
        LL_GPIO_ResetOutputPin(GPIOA, C);
        LL_GPIO_ResetOutputPin(GPIOA, D);
        LL_GPIO_SetOutputPin(GPIOA, E);
        LL_GPIO_SetOutputPin(GPIOA, F);
        LL_GPIO_SetOutputPin(GPIOA, G);
	break;
    default:
        break;
    }
}

static void print_ind (unsigned n, unsigned time) {
    char cn;
    for (int i = 3; i >= 0; --i) {
        cn = n >> (4*i);
        cn &= 0xf;
        switch (i) {
        case 3:
            LL_GPIO_ResetOutputPin(GPIOA, First_num);
	    break;
        case 2:
            LL_GPIO_ResetOutputPin(GPIOA, Second_num);
	    break;
        case 1:
            LL_GPIO_ResetOutputPin(GPIOA, Third_num);
	    break;
        case 0:
            LL_GPIO_ResetOutputPin(GPIOA, Fourth_num);
	    break;
	}

        print_simple(cn);
        LL_mDelay(time);

        switch (i) {
        case 3:
            LL_GPIO_SetOutputPin(GPIOA, First_num);
	    break;
        case 2:
            LL_GPIO_SetOutputPin(GPIOA, Second_num);
	    break;
        case 1:
            LL_GPIO_SetOutputPin(GPIOA, Third_num);
	    break;
        case 0:
            LL_GPIO_SetOutputPin(GPIOA, Fourth_num);
	    break;
	}

    }
}

void show_month (uint8_t m) {
    (m == 0x10U) ? m -= 6 : m;
    (m == 0x11U) ? m -= 6 : m;
    (m == 0x12U) ? m -= 6 : m;
    uint8_t quarter = (m - 1) / 3;
    switch (quarter) {
    case (0):
        LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_15);
        LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_14);
	break;
    case (1):
        LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_15);
        LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_14);
	break;
    case (2):
        LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_15);
        LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_14);
	break;
    case (3):
        LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_15);
        LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_14);
	break;
    default:
	break;
    }

    switch ((m - 1) % 3) {
    case (2):
        LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_4);
        LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3);
        LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_13);
	break;
    case (1):
        LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_4);
        LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_3);
        LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_13);
	break;
    case (0):
        LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_4);
        LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3);
        LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_13);
	break;
    default:
	break;
    }
}

void show_day (uint32_t day) {
	day -= (day >> 4) * 6;
	(day & 0x1) ? LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_9) : LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_9);
	((day >> 1) & 0x1) ? LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_8) : LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_8);
	((day >> 2) & 0x1) ? LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7) : LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7);
	((day >> 3) & 0x1) ? LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6) : LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6);
	((day >> 4) & 0x1) ? LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_5) : LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_5);
}

/*
 * Just set of commands to waste CPU power for a second
 * (basically it is a simple cycle with a predefined number
 * of loops)
 */
__attribute__((naked)) static void delay(void)
{
    asm ("push {r7, lr}");
    asm ("ldr r6, [pc, #8]");
    asm ("sub r6, #1");
    asm ("cmp r6, #0");
    asm ("bne delay+0x4");
    asm ("pop {r7, pc}");
#if defined(FLASH_0LAT_DELAY0LAT) || defined(FLASH_1LAT_DELAY0LAT)
    asm (".word 0x927c00"); //9600000
#else
    asm (".word 0x5b8d80"); //6000000
#endif
}

/*
 * Here we call configure all peripherals we need and
 * start blinking upon current mode
 */
int main(void)
{
    rcc_config();
    gpio_config();
    rtc_config();
    LL_Init1msTick(48000000);

    unsigned counter = 0, bcounter = 0, delay_time = 5;
    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_4);
    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5);
    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_6);
    LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_7);

    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_4);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_5);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_8);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_9);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_13);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_14);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_15);
    //delay();
    
    uint32_t time_now = LL_RTC_TIME_Get (&rtc_time), day;

    uint8_t month;

    while (1) {
	/*
        if (counter == 10) {
		++bcounter;
		counter = 0;
	}
	*/

	time_now = LL_RTC_TIME_Get (&rtc_time);

	//Seconds
	(!(time_now % 2)) ? LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_8) : LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_8);
	
	//Hours and Minuets
	time_now = time_now >> 2;
	print_ind (time_now, delay_time);

	//Month
	month = LL_RTC_DATE_GetMonth (&rtc_time);
	show_month (month);

	//Day
	day = LL_RTC_DATE_GetDay (&rtc_time);
	show_day (day);

	/*
        bcounter = bcounter % 0x10000;
	(!(bcounter % 2)) ? LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_8) : LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_8);
	print_ind (bcounter, delay_time);
	//delay();
	LL_mDelay(3);
	++counter;
	*/
    }
    return 0;
}
