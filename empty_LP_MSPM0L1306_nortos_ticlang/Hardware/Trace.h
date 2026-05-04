#ifndef TRACE_H_
#define TRACE_H_

#include "ti_msp_dl_config.h"

/* ti_msp_dl_config.h的引脚宏定义 */
#define Get_CircleNum_PORT                                               (GPIOA)
#define Get_CircleNum_Num_1_PIN                                 (DL_GPIO_PIN_26)
#define Get_CircleNum_Num_2_PIN                                 (DL_GPIO_PIN_27)
#define Get_CircleNum_Num_3_PIN                                 (DL_GPIO_PIN_21)
#define Get_CircleNum_Num_4_PIN                                 (DL_GPIO_PIN_22)
#define Get_CircleNum_Num_5_PIN                                  (DL_GPIO_PIN_3)

/**
 * @brief  循迹初始化
 * @param  无
 * @retval 无
 * @note   调用该函数不需要调用TB6612_Init()函数
 */
void Trace_Init(void);

/**
 * @brief  获取需要循迹的圈数的函数
 * @param  无
 * @retval Circle_Num   要循迹的圈数
 * @note   读取PA26、PA27、PA21、PA22、PA2五个引脚电平决定圈数
 */
int Get_CircleNum(void);

/**
 * @brief  获取小车的偏移情况
 * @param  无
 * @retval numerator / denominator
 * @note   < 0 表示黑线在左小车偏右，> 0 表示黑线在右小车偏左
 */
int Get_Line_Error(void);

/**
 * @brief  PID循迹函数
 * @param  无
 * @retval 无
 * @note   PID实现循迹
 */
void PID_LineFollow(void);

/**
 * @brief  直行逻辑
 * @param  无
 * @retval 无
 */
void Drive_One_Side_PID(void);

/**
 * @brief  左转逻辑
 * @param  无
 * @retval 无
 */
void Turn_Left_By_Sensor(void);

/**
 * @brief  1个完整正方形的循迹逻辑
 * @param  Num  要循迹的圈数
 * @retval 无
 */
void Track_Square(uint8_t Num);

#endif
