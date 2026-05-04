#ifndef DELAY_H_
#define DELAY_H_

#include "ti_msp_dl_config.h"

/**
 * @brief  ms延时函数
 * @param  uint32_t ms  要延时的毫秒
 * @retval 无
 * @note   通过系统提供的delay_cycles()函数延时
 */
void Delay_ms(uint32_t ms);

#endif