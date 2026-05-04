#include "PWM.h"
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/dl_timerg.h>

/**
 * @brief  PWM引脚初始化函数
 * @param  无
 * @retval 无
 * @note   底层调用定时器启动函数
 */
void PWM_Init(void)
{
    /* 启动 TimerG0 实例，它包含 PWM_CH_MOTOR1_A 和 PWM_CH_MOTOR1_B */
    DL_TimerG_startCounter(PWM_0_INST);
}

/**
 * @brief  占空比设置函数
 * @param  channel      设置的通道
 * @param  dutyCycle    要设置的占空比
 * @retval 0 / 1
 * @note   底层调用设置比较寄存器的库函数
 */
bool PWM_setDutyCycle(PWM_Channel_e channel, float dutyCycle)
{
    /* 判断占空比的合法性 */
    if (dutyCycle < 0.0f || dutyCycle > 100.0f)
    {
        return false;
    }

    uint32_t periodCount;   /* 重装载寄存器的值 */
    uint32_t pulseCount;    /* 写入比较寄存器的值 */

    /* 通道判断 */
    switch (channel)
    {
        case PWM_CH_MOTOR1_A:
            periodCount = 16000;    /* 重装载寄存器的值 */
            pulseCount = (uint32_t)(periodCount * (dutyCycle / 100.0f));    /* 写入比较寄存器的值 */
            DL_TimerG_setCaptureCompareValue(PWM_0_INST, pulseCount, DL_TIMERG_CAPTURE_COMPARE_0_INDEX);    /* 将值写入比较寄存器 */
            break;

        case PWM_CH_MOTOR1_B:
            periodCount = 16000;
            pulseCount = (uint32_t)(periodCount * (dutyCycle / 100.0f));
            DL_TimerG_setCaptureCompareValue(PWM_0_INST, pulseCount, DL_TIMERG_CAPTURE_COMPARE_1_INDEX);    /* 将值写入比较寄存器 */
            break;

        default:
            return false;
    }
    return true;
}

/**
 * @brief  停止PWM输出
 * @param  channel  PWM通道号
 * @retval 无
 * @note   停止PWM_0定时器计数器，PWM波形立即停止输出
 */
void PWM_stop(PWM_Channel_e channel)
{
    switch (channel)
    {
        /* 无论传入的是哪个通道都直接停止 */
        case PWM_CH_MOTOR1_A:
        case PWM_CH_MOTOR1_B:
            DL_TimerG_stopCounter(PWM_0_INST);
            break;
        
        default:
            break;
    }
}
