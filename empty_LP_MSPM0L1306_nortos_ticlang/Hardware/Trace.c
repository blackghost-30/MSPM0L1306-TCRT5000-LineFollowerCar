#include "ti_msp_dl_config.h"
#include "Trace.h"
#include "TB6612.h"
#include "TCRT5000.h"
#include "Delay.h"

/* 基准速度 */
#define BASE_SPEED  30.0f

/* 初始化PID参数 */
float Kp = 10.0;
float Ki = 0.0;
float Kd = 6.0;

static int last_error = 0;  /* 上一次误差 */
static int integral = 0;    /* 历史积累值，即PID的I，为积分值 */

float MAX_SPEED = 100.0f;
float MIN_SPEED = 0.0f;

/* 权值数组,越往外权重越大 */
static const int weights[5] = {-2, -1, 0, 1, 2};

/**
 * @brief  循迹初始化
 * @param  无
 * @retval 无
 * @note   调用该函数不需要调用TB6612_Init()函数
 */
void Trace_Init(void)
{
    TB6612_Init();
}

/**
 * @brief  获取需要循迹的圈数的函数
 * @param  无
 * @retval Circle_Num   要循迹的圈数
 * @note   读取PA26、PA27、PA21、PA22、PA2五个引脚电平决定圈数
 */
int Get_CircleNum(void)
{
    int Circle_Num = 0;
    
    if (!(DL_GPIO_readPins(Get_CircleNum_PORT, Get_CircleNum_Num_1_PIN)))
    {
        Circle_Num = 1;
        return Circle_Num;
    }
    if (!(DL_GPIO_readPins(Get_CircleNum_PORT, Get_CircleNum_Num_2_PIN)))
    {
        Circle_Num = 2;
        return Circle_Num;
    }

    if (!(DL_GPIO_readPins(Get_CircleNum_PORT, Get_CircleNum_Num_3_PIN)))
    {
        Circle_Num = 3;
        return Circle_Num;
    }
    if (!(DL_GPIO_readPins(Get_CircleNum_PORT, Get_CircleNum_Num_4_PIN)))
    {
        Circle_Num = 4;
        return Circle_Num;
    }
    if (!(DL_GPIO_readPins(Get_CircleNum_PORT, Get_CircleNum_Num_5_PIN)))
    {
        Circle_Num = 5;
        return Circle_Num;
    }
    return Circle_Num;
}

/**
 * @brief  获取小车的偏移情况
 * @param  无
 * @retval numerator / denominator
 * @note   < 0 表示黑线在左小车偏右 | > 0 表示黑线在右小车偏左
 */
int Get_Line_Error(void)
{
    int numerator = 0;          /* 所有传感器的偏移总和 */
    int denominator = 0;        /* 存储检测到黑线的传感器数量 */

    /* 遍历5个传感器 */
    for (int i = 0; i < 5; i++)
    {
        uint8_t value = TCRT5000_Read_Sensor(i);
        numerator += value * weights[i];
        denominator += value;
    }

    if (denominator == 0)
    {
        return 0;       /* 丢线 */
    }
    return numerator / denominator;     /* 偏移量 */
}

/**
 * @brief  PID循迹设置速度
 * @param  无
 * @retval 无
 * @note   PID实现循迹
 */
void PID_LineFollow(void)
{
    int error = Get_Line_Error();           /* 获取当前的误差，即判断当前状态是偏左还是偏右还是中间 */
    integral += error;                      /* 积分值 */
    int derivative = error - last_error;    /* 微分值 */

    float correction = Kp * error + Ki * integral + Kd * derivative;    /* PID误差 */
    last_error = error;

    /* 差速实现循迹 */
    float left_speed = BASE_SPEED + correction;
    float right_speed = BASE_SPEED - correction;
    
    /* 默认情况为都向前 */
    Direction left_dir = Foreward;
    Direction right_dir = Foreward;

    /* 若偏得太多就需要反向及时修正 */
    if (left_speed < 0)
    {
        left_dir = Backward;
        left_speed = -left_speed;
    }
    if (right_speed < 0)
    {
        right_dir = Backward;
        right_speed = -right_speed;
    }

    /* PID限幅 */
    if (left_speed > MAX_SPEED) left_speed = MAX_SPEED;
    if (left_speed < MIN_SPEED) left_speed = MIN_SPEED;
    if (right_speed > MAX_SPEED) right_speed = MAX_SPEED;
    if (right_speed < MIN_SPEED) right_speed = MIN_SPEED;

    /* 设置速度 */
    TB6612_Set_Motor(0, left_dir, left_speed);
    TB6612_Set_Motor(1, right_dir, right_speed);
}

/**
 * @brief  直行逻辑
 * @param  无
 * @retval 无
 */
void Drive_One_Side_PID(void)
{
    uint32_t lost_line_counter = 0;

    while (1)
    {
        /* 判断是否中间 3 个传感器都看不到黑线 */
        if (!TCRT5000_Read_Sensor(0) && !TCRT5000_Read_Sensor(1) && !TCRT5000_Read_Sensor(2) && !TCRT5000_Read_Sensor(3) && !TCRT5000_Read_Sensor(4))
        {
            lost_line_counter ++;

            /* 连续多次检测不到黑线才认定为拐角，比如连续 30 次，每次间隔 5ms，相当于 150ms */
            if (lost_line_counter >= 24)
            {
                break;      /* 退出循环，进入左转 */
            }
        }
        else
        {
            lost_line_counter = 0;      /* 检测到线，清除计数器 */
        }
        PID_LineFollow();       /* 没有检测到拐弯就一直走直线 */
        Delay_ms(3);        /* 小延时，防止跑太快导致判断不稳 */
    }

    /* 延时150ms停止稳定 */
    TB6612_Set_Motor(0, Stop, 0);
    TB6612_Set_Motor(1, Stop, 0);
    Delay_ms(150);
}

/**
 * @brief  左转逻辑
 * @param  无
 * @retval 无
 */
void Turn_Left_By_Sensor(void)
{
    /* 先确保五个传感器都没有黑线，然后就转弯 */
    uint32_t lost_counter = 0;
    while (1)
    {
        if (!TCRT5000_Read_Sensor(0) && !TCRT5000_Read_Sensor(1) && !TCRT5000_Read_Sensor(2) && !TCRT5000_Read_Sensor(3) && !TCRT5000_Read_Sensor(4))
        {
            lost_counter++;
            if (lost_counter >= 24)     /* 连续一段时间检测不到线才认为离开 */
                break;
        }
        else
        {
            lost_counter = 0;
        }
        /* 一直循迹 */
        PID_LineFollow();
        Delay_ms(3);
    }

    /* 左轮后退，右轮前进实现左转 */
    TB6612_Set_Motor(0, Stop, 20);
    TB6612_Set_Motor(1, Foreward, 20);

    while (!TCRT5000_Read_Sensor(2))
    {
        /* 中间传感器重新探测到黑线才停止 */
    }

    /* 转弯90°后停止，再重新循迹 */
    TB6612_Set_Motor(0, Stop, 0);
    TB6612_Set_Motor(1, Stop, 0);
    Delay_ms(100);
}

/**
 * @brief  1个完整正方形的循迹逻辑
 * @param  Num  要循迹的圈数
 * @retval 无
 */
void Track_Square(uint8_t Num)
{
    for (int i = 0; i < Num; i ++)
    {
        for (int j = 0; j < 4; j++)
        {
            /* 走4次直线和转弯实现1圈 */
            Drive_One_Side_PID();
            Turn_Left_By_Sensor();

            /* 完成一次转90°就响一次蜂鸣器 */ 
            DL_GPIO_setPins(BUZZER_PORT, BUZZER_BUZZER_1_PIN);
            Delay_ms(100);
            DL_GPIO_clearPins(BUZZER_PORT, BUZZER_BUZZER_1_PIN);
        }
    }
    TB6612_Set_Motor(0, Stop, 0);
    TB6612_Set_Motor(1, Stop, 0);
}
