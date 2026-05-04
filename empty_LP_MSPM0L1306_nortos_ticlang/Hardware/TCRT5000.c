#include "ti_msp_dl_config.h"
#include "TCRT5000.h"

/**
 * @brief  读取指定传感器的状态
 * @param  Sensor_Num   传感器编号(0~4)
 * @retval 1 线为黑色指示灯灭) | 0 线为白色(指示灯亮)
 * @note   读取每个引脚的电平
 */
uint8_t TCRT5000_Read_Sensor(uint8_t Sensor_Num)
{
    uint32_t Resistor1;
    uint32_t Resistor2;
    uint32_t Resistor3;
    uint32_t Resistor4;
    uint32_t Resistor5;

    uint8_t state = 0;

    /* 检查传感器编号的合法性 */
    if (Sensor_Num >= TCRT5000_SENSOR_COUNT)
    {
        return state = 0;   /* 错误返回白 */
    }

    /* 用简单的if判断进行引脚电平的读取 */
    if (Sensor_Num == 0)
    {
        Resistor1 = DL_GPIO_readPins(TCRT5000_PORT, TCRT5000_Sensor_1_PIN);
        state = (Resistor1 >> 24) & 1;  /* 将电平转换为1 */
        return state;    
    }
    if (Sensor_Num == 1)
    {
        Resistor2 = DL_GPIO_readPins(TCRT5000_PORT, TCRT5000_Sensor_2_PIN);
        state = (Resistor2 >> 15) & 1;
        return state;    
    }
    if (Sensor_Num == 2)
    {
        Resistor3 = DL_GPIO_readPins(TCRT5000_PORT, TCRT5000_Sensor_3_PIN);
        state = (Resistor3 >> 16) & 1;
        return state;    
    }
    if (Sensor_Num == 3)
    {
        Resistor4 = DL_GPIO_readPins(TCRT5000_PORT, TCRT5000_Sensor_4_PIN);
        state = (Resistor4 >> 17) & 1;
        return state;    
    }
    if (Sensor_Num == 4)
    {
        Resistor5 = DL_GPIO_readPins(TCRT5000_PORT, TCRT5000_Sensor_5_PIN);
        state = (Resistor5 >> 18) & 1;
        return state;    
    }

    return state;
}

/**
 * @brief  一次性读取所有传感器电平
 * @param  States   存储数据的数组
 * @retval 无
 */
void TCRT5000_Read_AllSensors(uint8_t *States)
{
    /* 检测参数的合法性 */
    if (States == NULL)
    {
        return;
    }

    /* 遍历所有传感器 */
    for (uint8_t i = 0; i < TCRT5000_SENSOR_COUNT; i++)
    {
        States[i] = TCRT5000_Read_Sensor(i);
    }
}