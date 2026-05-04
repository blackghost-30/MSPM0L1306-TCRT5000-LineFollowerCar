#ifndef TCRT5000_H_
#define TCRT5000_H_

#include <stdint.h>
#include "ti_msp_dl_config.h"

/* 一共5个TCRT5000 */
#define TCRT5000_SENSOR_COUNT   (5U)

/* 传感器编号 */
#define TCRT5000_SENSOR_1       (0U)  // PA.24
#define TCRT5000_SENSOR_2       (1U)  // PA.15
#define TCRT5000_SENSOR_3       (2U)  // PA.16
#define TCRT5000_SENSOR_4       (3U)  // PA.17
#define TCRT5000_SENSOR_5       (4U)  // PA.18

/* ti_msp_dl_config.h的引脚宏定义 */
#define TCRT5000_PORT                                                    (GPIOA)
#define TCRT5000_Sensor_1_PIN                                   (DL_GPIO_PIN_24)
#define TCRT5000_Sensor_2_PIN                                   (DL_GPIO_PIN_15)
#define TCRT5000_Sensor_3_PIN                                   (DL_GPIO_PIN_16)
#define TCRT5000_Sensor_4_PIN                                   (DL_GPIO_PIN_17)
#define TCRT5000_Sensor_5_PIN                                   (DL_GPIO_PIN_18)

/**
 * @brief  读取指定传感器的状态
 * @param  Sensor_Num   传感器编号(0~4)
 * @retval 1 线为黑色指示灯灭) | 0 线为白色(指示灯亮)
 * @note   读取每个引脚的电平
 */
uint8_t TCRT5000_Read_Sensor(uint8_t Sensor_Num);

/**
 * @brief  一次性读取所有传感器电平
 * @param  States   存储数据的数组
 * @retval 无
 */
void TCRT5000_Read_AllSensors(uint8_t *States);

#endif
