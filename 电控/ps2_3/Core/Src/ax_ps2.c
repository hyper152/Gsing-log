/**
  * @brief  PS2手柄控制 - HAL库版本
  * @author XTARK创新团队
  * @date   2023-10-05
  * @modified 修正PS2通信时序和协议兼容性
  */

#include "ax_ps2.h"
#include "main.h"

// PS2手柄的输入输出端口定义
#define DI_PORT     GPIOA
#define DI_PIN      GPIO_PIN_7

#define CMD_PORT    GPIOA
#define CMD_PIN     GPIO_PIN_6

#define CS_PORT     GPIOA
#define CS_PIN      GPIO_PIN_5

#define CLK_PORT    GPIOA
#define CLK_PIN     GPIO_PIN_4

// 宏定义简化IO操作
#define DI()        HAL_GPIO_ReadPin(DI_PORT, DI_PIN)
#define CMD_H()     HAL_GPIO_WritePin(CMD_PORT, CMD_PIN, GPIO_PIN_SET)
#define CMD_L()     HAL_GPIO_WritePin(CMD_PORT, CMD_PIN, GPIO_PIN_RESET)
#define CS_H()      HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET)
#define CS_L()      HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET)
#define CLK_H()     HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_SET)
#define CLK_L()     HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_RESET)

// PS2标准通信命令帧（3字节命令+6字节读取）
const uint8_t PS2_cmnd[9] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static uint8_t PS2_data[9] = {0};  // 接收的数据缓冲区

/**
  * @brief  PS2初始化
  * @param  无
  * @retval 无
  */
void AX_PS2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 使能GPIO时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // DATA引脚配置 - 输入上拉（PS2数据引脚默认高电平）
    GPIO_InitStruct.Pin = DI_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(DI_PORT, &GPIO_InitStruct);

    // COMMAND引脚配置 - 推挽输出
    GPIO_InitStruct.Pin = CMD_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(CMD_PORT, &GPIO_InitStruct);

    // CS引脚配置 - 推挽输出
    GPIO_InitStruct.Pin = CS_PIN;
    HAL_GPIO_Init(CS_PORT, &GPIO_InitStruct);

    // CLK引脚配置 - 推挽输出
    GPIO_InitStruct.Pin = CLK_PIN;
    HAL_GPIO_Init(CLK_PORT, &GPIO_InitStruct);

    // 初始化状态：时钟高电平，片选关闭
    CLK_H();
    CS_H();
    // 确保命令线初始为高电平
    CMD_H();
    delay_us(100);  // 等待硬件稳定
}

/**
  * @brief  PS2数据读写函数（核心时序修正）
  * @param  cmd: 要写入的命令
  * @retval 读出的数据
  */
static uint8_t PS2_ReadWriteData(uint8_t cmd)
{
    uint8_t res = 0;
    uint8_t ref;

    // PS2协议是LSB先传（最低位优先）
    for(ref = 0x01; ref > 0x00; ref <<= 1)
    {
        // 1. 在时钟高电平时准备数据
        if(ref & cmd)
            CMD_H();
        else
            CMD_L();
        delay_us(2);  // 数据建立时间

        // 2. 时钟下降沿：数据应在此时被手柄采样
        CLK_L();
        delay_us(10);  // 低电平保持时间（至少5us）

        // 3. 读取数据（在时钟低电平时读取，确保稳定）
        if(DI())
            res |= ref;

        // 4. 时钟上升沿：准备下一位
        CLK_H();
        delay_us(10);  // 高电平保持时间（至少5us）
    }

    // 每字节传输后额外延时，增强兼容性
//    delay_us(2);
    return res;
}

/**
  * @brief  PS2获取按键及摇杆数值
  * @param  *JoystickStruct 手柄键值结构体
  * @retval 无
  */
void AX_PS2_ScanKey(JOYSTICK_TypeDef *JoystickStruct)
{
    uint8_t i;

    // 使能手柄：CS拉低后需等待至少100us让手柄进入通信状态
    CS_L();
    delay_us(250);

    // 读取PS2数据（9字节通信周期）
    for(i = 0; i < 9; i++)
    {
        PS2_data[i] = PS2_ReadWriteData(PS2_cmnd[i]);
    }

    // 关闭使能：CS拉高前确保时钟线为高电平
    CLK_H();
    delay_us(16);
    CS_H();
    delay_us(16);  // 确保片选信号稳定

    // 数值传递（根据PS2标准协议映射）
    // 注意：部分手柄需要先发送0x01 0x43 0x00 0x01 0x00激活模拟模式
    JoystickStruct->mode = PS2_data[1];         // 模式字节（0x41=数字模式，0x73=模拟模式）
    JoystickStruct->btn1 = ~PS2_data[3];        // 按钮组1（低电平有效，取反后高电平表示按下）
    JoystickStruct->btn2 = ~PS2_data[4];        // 按钮组2
    JoystickStruct->RJoy_LR = PS2_data[5];      // 右摇杆X轴
    JoystickStruct->RJoy_UD = PS2_data[6];      // 右摇杆Y轴
    JoystickStruct->LJoy_LR = PS2_data[7];      // 左摇杆X轴
    JoystickStruct->LJoy_UD = PS2_data[8];      // 左摇杆Y轴
}

/**
  * @brief  增强型HAL库微秒级延迟函数
  * @param  udelay: 要延时的微秒数
  * @retval 无
  */
void delay_us(uint32_t udelay)
{
    uint32_t startval, tickn, delays, wait;
    const uint32_t sys_freq = SystemCoreClock / 1000000;  // 系统时钟MHz数

    startval = SysTick->VAL;
    tickn = HAL_GetTick();
    delays = udelay * sys_freq;  // 转换为SysTick计数（1MHz=1us）

    if (delays > startval)
    {
        // 等待SysTick溢出
        while (HAL_GetTick() == tickn);
        // 计算剩余等待计数
        wait = (SysTick->LOAD + 1) - (delays - startval);
        while (SysTick->VAL < wait);
    }
    else
    {
        wait = startval - delays;
        while (SysTick->VAL > wait && HAL_GetTick() == tickn);
    }
}
