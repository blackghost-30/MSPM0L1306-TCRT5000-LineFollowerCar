#ifndef TB6612_H_
#define TB6612_H_

#include "ti_msp_dl_config.h"
#include "PWM.h"

/* 方向枚举 */
typedef enum {
    Foreward = 0,
    Backward = 1,
    Stop = 2
} Direction;

/* 电机编号枚举 */
typedef enum {
    Motor_1 = 0,
    Motor_2 = 1,
} Motor_ID;

/**
 * @brief  TB6612初始化函数
 * @param  无
 * @retval 无
 * @note   调用该函数不需要调用PWM_Init()函数
 */
void TB6612_Init(void);

/**
 * @brief  方向和PWM占空比大小控制
 * @param  motor_id     设置的电机 0 / 1
 * @param  dir          方向 0 / 1 / 2
 * @param  dutyCycle    占空比
 * @retval 无
 */
void TB6612_Set_Motor(Motor_ID motor_id, Direction dir, float dutyCycle);

#endif