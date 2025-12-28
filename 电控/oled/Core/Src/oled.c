#include "oled.h"
#include <string.h>
#include "main.h"
#include <stdlib.h>  // 用于随机数生成
#include <time.h>    // 用于随机数种子初始化

// 屏幕/字符参数（8x16点阵）
#define SCREEN_WIDTH  128   // OLED宽度
#define SCREEN_HEIGHT 64    // OLED高度
#define CHAR_WIDTH    8     // 字符宽
#define CHAR_HEIGHT   16    // 字符高
#define HYPER_LEN     5     // "hyper"字符数
#define REFRESH_DELAY 500   // 位置刷新间隔（ms）

// 全局变量
static I2C_HandleTypeDef *oled_hi2c;
static uint32_t last_refresh_time = 0;  // 上一次刷新时间

// ====================== hyper字符点阵（8x16，新宋体12号） ======================
const uint8_t HYPER_8x16[HYPER_LEN][16] = {
    // h
    {0x00,0x80,0x00,0x04,0x84,0xFC,0x04,0x04,0x00,0x01,0x01,0x01,0x00,0x01,0x01,0x01},
    // y
    {0x00,0xE8,0x24,0x24,0x24,0x24,0xF8,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x00},
    // p
    {0x00,0x70,0x88,0x04,0x04,0x89,0xFF,0x01,0x00,0x00,0x00,0x01,0x01,0x00,0x01,0x01},
    // e
    {0x00,0x80,0x60,0x18,0x1E,0x61,0x81,0x00,0x01,0x01,0x01,0x00,0x00,0x01,0x01,0x01},
    // r
    {0x04,0xFC,0x04,0x00,0x00,0x84,0xFC,0x04,0x00,0x00,0x01,0x01,0x01,0x00,0x0F,0x08}
};

// ====================== 内部函数 ======================
static void OLED_WriteCommand(uint8_t cmd) {
  HAL_I2C_Mem_Write(oled_hi2c, OLED_ADDR, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 1000);
}

static void OLED_WriteData(uint8_t data) {
  HAL_I2C_Mem_Write(oled_hi2c, OLED_ADDR, 0x40, I2C_MEMADD_SIZE_8BIT, &data, 1, 1000);
}

// 生成随机位置（确保hyper完整显示在屏幕内）
static void GetRandomPos(uint8_t *x, uint8_t *y) {
    // 水平范围：0 ~ (屏幕宽度 - 字符串总宽度)
    *x = rand() % (SCREEN_WIDTH - HYPER_LEN * CHAR_WIDTH);
    // 垂直范围：0 ~ (屏幕高度 - 字符高度)/8（OLED按8行分页）
    *y = rand() % ((SCREEN_HEIGHT - CHAR_HEIGHT) / 8);
}

// ====================== 显示函数 ======================
void OLED_Init(I2C_HandleTypeDef *hi2c) {
  oled_hi2c = hi2c;
  HAL_Delay(300);

  // OLED初始化指令
  OLED_WriteCommand(0xAE); OLED_WriteCommand(0x20); OLED_WriteCommand(0x02);
  OLED_WriteCommand(0x00); OLED_WriteCommand(0x10); OLED_WriteCommand(0x40);
  OLED_WriteCommand(0xA0); OLED_WriteCommand(0xC0); OLED_WriteCommand(0xA6);
  OLED_WriteCommand(0xA8); OLED_WriteCommand(0x3F); OLED_WriteCommand(0xD3);
  OLED_WriteCommand(0x00); OLED_WriteCommand(0xD5); OLED_WriteCommand(0x80);
  OLED_WriteCommand(0xD9); OLED_WriteCommand(0xF1); OLED_WriteCommand(0xDA);
  OLED_WriteCommand(0x12); OLED_WriteCommand(0xDB); OLED_WriteCommand(0x40);
  OLED_WriteCommand(0x8D); OLED_WriteCommand(0x14); OLED_WriteCommand(0xAF);

  OLED_Clear();
  // 初始化随机数种子（用系统时间做种子，确保每次启动随机位置不同）
  srand(HAL_GetTick());
  last_refresh_time = HAL_GetTick();
}

void OLED_Clear(void) {
  uint8_t page, col;
  for (page = 0; page < 8; page++) {
    OLED_WriteCommand(0xB0 + page);
    OLED_WriteCommand(0x00);
    OLED_WriteCommand(0x10);
    for (col = 0; col < 128; col++) {
      OLED_WriteData(0x00);
    }
  }
}

// 显示单个8x16字符
void OLED_ShowChar(uint8_t x, uint8_t y, const uint8_t *char_data) {
  uint8_t i;
  if (x >= 128 || y >= 7) return;  // 边界检查（y+1 < 8页）

  // 上半部分（0-7行）
  OLED_WriteCommand(0xB0 + y);
  OLED_WriteCommand(0x00 + (x & 0x0F));
  OLED_WriteCommand(0x10 + (x >> 4));
  for (i = 0; i < 8; i++) OLED_WriteData(char_data[i]);

  // 下半部分（8-15行）
  OLED_WriteCommand(0xB0 + y + 1);
  OLED_WriteCommand(0x00 + (x & 0x0F));
  OLED_WriteCommand(0x10 + (x >> 4));
  for (i = 8; i < 16; i++) OLED_WriteData(char_data[i]);
}

// 在指定位置显示hyper字符串
static void OLED_ShowHyperAt(uint8_t x, uint8_t y) {
    for (uint8_t i = 0; i < HYPER_LEN; i++) {
        OLED_ShowChar(x + i * CHAR_WIDTH, y, HYPER_8x16[i]);
    }
}

// 主显示逻辑：hyper随机位置刷新
void OLED_ShowMainLogic(void) {
    // 每隔REFRESH_DELAY时间刷新一次位置
    if (HAL_GetTick() - last_refresh_time >= REFRESH_DELAY) {
        OLED_Clear();  // 清屏
        uint8_t rand_x, rand_y;
        GetRandomPos(&rand_x, &rand_y);  // 生成随机位置
        OLED_ShowHyperAt(rand_x, rand_y);  // 在随机位置显示
        last_refresh_time = HAL_GetTick();  // 更新刷新时间
    }
}
