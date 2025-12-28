#ifndef __NRF24L01_H
#define __NRF24L01_H

#include "stm32f1xx_hal.h"
#include "NRF24L01_Define.h"
#include "delay.h"
// 定义硬件SPI句柄
extern SPI_HandleTypeDef hspi2;

// 定义GPIO引脚
#define NRF24L01_CE_PORT    GPIOB
#define NRF24L01_CE_PIN     GPIO_PIN_0

#define NRF24L01_CSN_PORT   GPIOB
#define NRF24L01_CSN_PIN    GPIO_PIN_1

// 外部可调用全局数组
extern uint8_t NRF24L01_TxAddress[];
extern uint8_t NRF24L01_TxPacket[];
extern uint8_t NRF24L01_RxAddress[];
extern uint8_t NRF24L01_RxPacket[];

// 调试用变量
extern uint8_t debug_last_status;

// 函数声明
void NRF24L01_CE(uint8_t state);
void NRF24L01_CSN(uint8_t state);
uint8_t NRF24L01_SPI_TransmitReceive(uint8_t data);
uint8_t NRF24L01_ReadReg(uint8_t reg);
void NRF24L01_ReadRegs(uint8_t reg, uint8_t *buf, uint8_t len);
void NRF24L01_WriteReg(uint8_t reg, uint8_t data);
void NRF24L01_WriteRegs(uint8_t reg, uint8_t *buf, uint8_t len);
uint8_t NRF24L01_ReadStatus(void);
void NRF24L01_FlushTx(void);
void NRF24L01_FlushRx(void);
void NRF24L01_PowerDown(void);
void NRF24L01_StandbyI(void);
void NRF24L01_Rx(void);
void NRF24L01_Tx(void);
void NRF24L01_Init(void);
uint8_t NRF24L01_Send(void);
uint8_t NRF24L01_Receive(void);
void NRF24L01_UpdateRxAddress(void);

#endif
