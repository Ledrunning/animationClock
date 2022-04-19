#include "delay.h"


#ifndef INLINE_DELAY

void delay_cycles(uint32_t cycles_count)
{
#define _DELAY_CYCLES_GETTING_LATENCY (14 + 2) // 2 - вызов функции.
#define _DELAY_CYCLES_COUNT_PER_LOOP_MIN 5
    
    register uint32_t cycles_per_loop = _DELAY_CYCLES_COUNT_PER_LOOP_MIN;
    
    if(FLASH->ACR & FLASH_ACR_PRFTBE){
        cycles_per_loop += FLASH->ACR & FLASH_ACR_LATENCY;
    }
    
    if(cycles_count > _DELAY_CYCLES_GETTING_LATENCY){
        
        cycles_count -= _DELAY_CYCLES_GETTING_LATENCY;
			
        register uint32_t r_cycles_count=cycles_count;
			  register uint32_t r_count=0;
			  register uint32_t r_cycles_per_loop=cycles_per_loop;
     __asm	{ 
            m2:
            CMP r_cycles_count, r_count
            BHS m1
            ADD r_count, r_cycles_per_loop
            B m2
            m1:            
         }
			 }
#undef _DELAY_CYCLES_COUNT_PER_LOOP_MIN
#undef _DELAY_CYCLES_GETTING_LATENCY
}


void delay_ns(uint32_t ns)
{
#define _DELAY_NS_CALCULATING_CYCLES (35 + 2) // 2 - вызов функции.
    
    register uint32_t cycles_to_delay = (SystemCoreClock / 1000000UL) * ns / 1000UL;
    
    if(cycles_to_delay > _DELAY_NS_CALCULATING_CYCLES){
        cycles_to_delay -= _DELAY_NS_CALCULATING_CYCLES;
        delay_cycles(cycles_to_delay);
    }
#undef _DELAY_NS_CALCULATING_CYCLES
}


//void delay_us(uint32_t us)
//{
//#define _DELAY_US_CALCULATING_CYCLES (21 + 2) // 2 - вызов функции.
//    
//    register uint32_t cycles_to_delay = SystemCoreClock / 1000000UL * us;
//    
//    if(cycles_to_delay > _DELAY_US_CALCULATING_CYCLES){
//        cycles_to_delay -= _DELAY_US_CALCULATING_CYCLES;
//        delay_cycles(cycles_to_delay);
//    }
//#undef _DELAY_US_CALCULATING_CYCLES
//}
// подпрограмма задержки
void delay_us(uint32_t ms)
{
    volatile uint32_t nCount;
    RCC_ClocksTypeDef RCC_Clocks;
    RCC_GetClocksFreq (&RCC_Clocks);
    nCount=(RCC_Clocks.HCLK_Frequency/10000000)*ms;
    for (; nCount!=0; nCount--);
}

// подпрограмма задержки
void delay_ms(uint32_t ms)
{
    volatile uint32_t nCount;
    RCC_ClocksTypeDef RCC_Clocks;
    RCC_GetClocksFreq (&RCC_Clocks);
    nCount=(RCC_Clocks.HCLK_Frequency/10000)*ms;
    for (; nCount!=0; nCount--);
}

//void delay_ms(uint32_t ms)
//{
//#define _DELAY_MS_CALCULATING_CYCLES (20 + 2) // 2 - вызов функции.
//    
//    register uint32_t cycles_to_delay = SystemCoreClock / 1000UL * ms;
//    
//    if(cycles_to_delay > _DELAY_MS_CALCULATING_CYCLES){
//        cycles_to_delay -= _DELAY_MS_CALCULATING_CYCLES;
//        delay_cycles(cycles_to_delay);
//    }
//#undef _DELAY_MS_CALCULATING_CYCLES
//}

#endif
