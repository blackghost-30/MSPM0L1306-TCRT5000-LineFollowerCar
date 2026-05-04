#include "ti_msp_dl_config.h"
#include "TB6612.h"
#include "PWM.h"

/**
 * @brief  TB6612初始化函数
 * @param  无
 * @retval 无
 * @note   调用该函数不需要调用PWM_Init()函数
 */
void TB6612_Init(void)
{
    PWM_Init();
}

/**
 * @brief  方向和PWM占空比大小控制
 * @param  motor_id     设置的电机 0 / 1
 * @param  dir          方向 0 / 1 / 2
 * @param  dutyCycle    占空比
 * @retval 无
 */
void TB6612_Set_Motor(Motor_ID motor_id, Direction dir, float dutyCycle)
{
    /* 判断电机id的正确性 */
    if (motor_id < 0 || motor_id > 1)
    {
        return;
    }

    /* 判断占空比的合法性 */
    if (dutyCycle < 0.0f || dutyCycle > 100.0f)
    {
        return;
    }

    /* 电机1对应PA12路PWM */
    if (motor_id == 0)
    {
        PWM_setDutyCycle(0, dutyCycle);
        if (dir == 0)       /* 正转 */
        {
            DL_GPIO_setPins(TB6612_1_A_PORT, TB6612_1_A_AIN_1_1_PIN);
            DL_GPIO_clearPins(TB6612_1_A_PORT, TB6612_1_A_AIN_1_2_PIN);
        }
        else if (dir == 1)  /* 反转 */
        {
            DL_GPIO_clearPins(TB6612_1_A_PORT, TB6612_1_A_AIN_1_1_PIN);
            DL_GPIO_setPins(TB6612_1_A_PORT, TB6612_1_A_AIN_1_2_PIN);
        }
        else if (dir == 2)  /* 停止 */
        {
            DL_GPIO_clearPins(TB6612_1_A_PORT, TB6612_1_A_AIN_1_1_PIN);
            DL_GPIO_clearPins(TB6612_1_A_PORT, TB6612_1_A_AIN_1_2_PIN);
        }
    }

    /* 电机2对应PA13路PWM */
    else if (motor_id == 1)
    {
        PWM_setDutyCycle(1, dutyCycle);
        if (dir == 0)       /* 正转 */
        {
            DL_GPIO_setPins(TB6612_1_B_PORT, TB6612_1_B_BIN_1_1_PIN);
            DL_GPIO_clearPins(TB6612_1_B_PORT, TB6612_1_B_BIN_1_2_PIN);
        }
        else if (dir == 1)  /* 反转 */
        {
            DL_GPIO_clearPins(TB6612_1_B_PORT, TB6612_1_B_BIN_1_1_PIN);
            DL_GPIO_setPins(TB6612_1_B_PORT, TB6612_1_B_BIN_1_2_PIN);
        }
        else if (dir == 2)  /* 停止 */
        {
            DL_GPIO_clearPins(TB6612_1_B_PORT, TB6612_1_B_BIN_1_1_PIN);
            DL_GPIO_clearPins(TB6612_1_B_PORT, TB6612_1_B_BIN_1_2_PIN);
        }
    }
}
