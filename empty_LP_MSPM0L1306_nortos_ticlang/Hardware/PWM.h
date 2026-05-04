#ifndef PWM_H_
#define PWM_H_

#include "ti_msp_dl_config.h"

/* 用数字代替长文字 */
typedef enum {
    PWM_CH_MOTOR1_A = 0,
    PWM_CH_MOTOR1_B = 1,
    NUM_PWM_CHANNELS        // 用于计数
} PWM_Channel_e;

/**
 * @brief  PWM引脚初始化函数
 * @param  无
 * @retval 无
 * @note   底层调用定时器启动函数
 */
void PWM_Init(void);

/**
 * @brief  占空比设置函数
 * @param  channel      设置的通道
 * @param  dutyCycle    要设置的占空比
 * @retval 0 / 1
 * @note   底层调用设置比较寄存器的库函数
 */
bool PWM_setDutyCycle(PWM_Channel_e channel, float dutyCycle);

/**
 * @brief  停止PWM输出
 * @param  channel  PWM通道号
 * @retval 无
 * @note   停止PWM_0定时器计数器，PWM波形立即停止输出
 */
void PWM_stop(PWM_Channel_e channel);

#endif /* PWM_H_ */
