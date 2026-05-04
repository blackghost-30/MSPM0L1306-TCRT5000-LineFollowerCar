#include "ti_msp_dl_config.h"
#include "Delay.h"

/* 根据系统主频调整这个值，MSPM0L1306 默认是 32MHz */
#define SYSTEM_CORE_CLOCK 32000000

/**
 * @brief  ms延时函数
 * @param  uint32_t ms  要延时的毫秒
 * @retval 无
 * @note   通过系统提供的delay_cycles()函数延时
 */
void Delay_ms(uint32_t ms)
{
    while (ms --)
    {
        delay_cycles(32000);
    }
}
