#include "nrf24l01.h"
#include "main.h"

// 全局变量
uint8_t NRF24L01_TxAddress[5] = {0x11, 0x22, 0x33, 0x44, 0x55};
uint8_t NRF24L01_TxPacket[8];
uint8_t NRF24L01_RxAddress[5] = {0x11, 0x22, 0x33, 0x44, 0x55};
uint8_t NRF24L01_RxPacket[8];

uint8_t debug_last_status = 0;//调试用

// CE引脚控制
void NRF24L01_CE(uint8_t state) {
    HAL_GPIO_WritePin(NRF24L01_CE_PORT, NRF24L01_CE_PIN, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

// CSN引脚控制
void NRF24L01_CSN(uint8_t state) {
    HAL_GPIO_WritePin(NRF24L01_CSN_PORT, NRF24L01_CSN_PIN, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

// SPI发送接收一个字节
uint8_t NRF24L01_SPI_TransmitReceive(uint8_t data) {
    uint8_t rx_data;
    HAL_SPI_TransmitReceive(&hspi2, &data, &rx_data, 1, 100);
    return rx_data;
}

// 读取寄存器
uint8_t NRF24L01_ReadReg(uint8_t reg) {
    uint8_t value;
    NRF24L01_CSN(0);
    NRF24L01_SPI_TransmitReceive(NRF24L01_R_REGISTER | reg);
    value = NRF24L01_SPI_TransmitReceive(NRF24L01_NOP);
    NRF24L01_CSN(1);
    return value;
}

void NRF24L01_ReadRegs(uint8_t reg, uint8_t *pBuf, uint8_t len) {
    NRF24L01_CSN(0);
    NRF24L01_SPI_TransmitReceive(NRF24L01_R_REGISTER | reg);
    for(uint8_t i=0; i<len; i++) {
        pBuf[i] = NRF24L01_SPI_TransmitReceive(NRF24L01_NOP);
    }
    NRF24L01_CSN(1);
}

// 写入寄存器
void NRF24L01_WriteReg(uint8_t reg, uint8_t data) {
    NRF24L01_CSN(0);
    NRF24L01_SPI_TransmitReceive(NRF24L01_W_REGISTER | reg);
    NRF24L01_SPI_TransmitReceive(data);
    NRF24L01_CSN(1);
}

void NRF24L01_WriteRegs(uint8_t reg, uint8_t *buf, uint8_t len) {
    NRF24L01_CSN(0);
    NRF24L01_SPI_TransmitReceive(NRF24L01_W_REGISTER | reg);
    for (uint8_t i = 0; i < len; i++) {
        NRF24L01_SPI_TransmitReceive(buf[i]);
    }
    NRF24L01_CSN(1);
}

// 读取状态寄存器
uint8_t NRF24L01_ReadStatus(void) {
    uint8_t status;
    NRF24L01_CSN(0);
    status = NRF24L01_SPI_TransmitReceive(NRF24L01_NOP);
    NRF24L01_CSN(1);
    return status;
}

// 清空Tx FIFO
void NRF24L01_FlushTx(void) {
    NRF24L01_CSN(0);
    NRF24L01_SPI_TransmitReceive(NRF24L01_FLUSH_TX);
    NRF24L01_CSN(1);
}

// 清空Rx FIFO
void NRF24L01_FlushRx(void) {
    NRF24L01_CSN(0);
    NRF24L01_SPI_TransmitReceive(NRF24L01_FLUSH_RX);
    NRF24L01_CSN(1);
}

// 进入掉电模式
void NRF24L01_PowerDown(void) {
    NRF24L01_CE(0); // CE置0
    uint8_t config = NRF24L01_ReadReg(NRF24L01_CONFIG);
    config &= ~0x02; // PWR_UP = 0
    NRF24L01_WriteReg(NRF24L01_CONFIG, config);
}

// 进入待机模式1
void NRF24L01_StandbyI(void) {
    NRF24L01_CE(0); // CE置0
    uint8_t config = NRF24L01_ReadReg(NRF24L01_CONFIG);
    config |= 0x02; // PWR_UP = 1
    NRF24L01_WriteReg(NRF24L01_CONFIG, config);
}

// 进入接收模式
void NRF24L01_Rx(void) {
    NRF24L01_CE(0); // CE置0
    uint8_t config = NRF24L01_ReadReg(NRF24L01_CONFIG);
	if (config == 0xFF) {return;}					//配置寄存器全为1，出错，退出函数
    config |= 0x03; // PWR_UP = 1, PRIM_RX = 1
    NRF24L01_WriteReg(NRF24L01_CONFIG, config);

    NRF24L01_CE(1); // CE置1，进入接收模式
}

// 进入发送模式
void NRF24L01_Tx(void) {
    NRF24L01_CE(0); // CE置0
    uint8_t config = NRF24L01_ReadReg(NRF24L01_CONFIG);
	if (config == 0xFF) {return;}					//配置寄存器全为1，出错，退出函数
    config |= 0x02; // PWR_UP = 1
    config &= ~0x01; // PRIM_RX = 0
    NRF24L01_WriteReg(NRF24L01_CONFIG, config);

    NRF24L01_CE(1); // CE置1，进入发送模式

}

// 初始化函数
void NRF24L01_Init(void) {
    // 初始化引脚
    NRF24L01_CE(0);
    NRF24L01_CSN(1);
    HAL_Delay(5);

    // 配置寄存器
    NRF24L01_WriteReg(NRF24L01_CONFIG, 0x08);    // 使能CRC, 1字节CRC, 上电
//    NRF24L01_WriteReg(NRF24L01_EN_AA, 0x3F);     // 使能所有通道的自动应答
    NRF24L01_WriteReg(NRF24L01_EN_AA, 0x01);
    NRF24L01_WriteReg(NRF24L01_EN_RXADDR, 0x01); // 使能接收通道0
    NRF24L01_WriteReg(NRF24L01_SETUP_AW, 0x03);  // 5字节地址宽度
//    NRF24L01_WriteReg(NRF24L01_SETUP_RETR, 0x03);// 重试3次，间隔250us
    NRF24L01_WriteReg(NRF24L01_SETUP_RETR, 0x1F);
    NRF24L01_WriteReg(NRF24L01_RF_CH, 0x02);     // 2.402GHz
    NRF24L01_WriteReg(NRF24L01_RF_SETUP, 0x0E);  // 2Mbps, 0dBm

    // 设置接收地址
    NRF24L01_WriteRegs(NRF24L01_RX_ADDR_P0, NRF24L01_RxAddress, 5);
    NRF24L01_WriteReg(NRF24L01_RX_PW_P0, 8);    // 接收通道0数据宽度

    // 设置发送地址
    NRF24L01_WriteRegs(NRF24L01_TX_ADDR, NRF24L01_TxAddress, 5);

    // 清空FIFO
    NRF24L01_FlushTx();
    NRF24L01_FlushRx();

    // 清除状态标志
    NRF24L01_WriteReg(NRF24L01_STATUS, 0x70);

    // 进入接收模式
    NRF24L01_Rx();
}

// 发送函数
uint8_t NRF24L01_Send(void) {
    uint8_t Status;
    uint8_t SendFlag;
    uint32_t timeout = 10000; // 超时计数器

    // 设置发送地址
    NRF24L01_WriteRegs(NRF24L01_TX_ADDR, NRF24L01_TxAddress, 5);

    // 设置接收通道0地址为发送地址(用于接收ACK)
    NRF24L01_WriteRegs(NRF24L01_RX_ADDR_P0, NRF24L01_TxAddress, 5);

    // 写入发送数据
    NRF24L01_CSN(0);
    NRF24L01_SPI_TransmitReceive(NRF24L01_W_TX_PAYLOAD);
    for (uint8_t i = 0; i < 8; i++) {
        NRF24L01_SPI_TransmitReceive(NRF24L01_TxPacket[i]);
    }
    NRF24L01_CSN(1);

    // 启动发送
    NRF24L01_Tx();
//    delay_us(15);

    // 等待发送完成或超时
    while (1) {
        Status = NRF24L01_ReadStatus();
        debug_last_status = Status;


        timeout --;
		if (timeout == 0)			//如果计次减至0
		{
			SendFlag = 4;			//发送超时，置标志位为4
			NRF24L01_Init();		//发送出错，重新初始化一次设备，这样有助于设备从错误中恢复正常
			break;					//跳出循环
		}

		/*根据状态寄存器的值，判断发送状态*/
		if ((Status & 0x30) == 0x30)		//状态寄存器位4（MAX_RT）和位5（TX_DS）同时为1
		{
			SendFlag = 3;			//状态寄存器的值不合法，置标志位为3
			NRF24L01_Init();		//发送出错，重新初始化一次设备，这样有助于设备从错误中恢复正常
			break;					//跳出循环
		}
		else if ((Status & 0x10) == 0x10)	//状态寄存器位4（MAX_RT）为1
		{
			SendFlag = 2;			//达到了最大重发次数仍未收到应答，置标志位为2
			NRF24L01_Init();		//发送出错，重新初始化一次设备，这样有助于设备从错误中恢复正常
			break;					//跳出循环
		}
		else if ((Status & 0x20) == 0x20)	//状态寄存器位5（TX_DS）为1
		{
			SendFlag = 1;
			break;					//跳出循环
		}

    }
    /*给状态寄存器的位4（MAX_RT）和位5（TX_DS）写1，清标志位*/
    	NRF24L01_WriteReg(NRF24L01_STATUS, 0x30);

    	/*清空Tx FIFO的所有数据*/
    	NRF24L01_FlushTx();

    	/*发送完成后，恢复接收通道0原来的地址*/
    	/*如果发送地址和接收通道0地址设置相同，则可不执行这一句*/
    	NRF24L01_WriteRegs(NRF24L01_RX_ADDR_P0, NRF24L01_RxAddress, 5);

    	/*发送完成，芯片恢复为接收模式*/
    	NRF24L01_Rx();

    	/*返回发送标志位*/
    	return SendFlag;

}

// 接收函数
uint8_t NRF24L01_Receive(void) {
    uint8_t Status = NRF24L01_ReadStatus();
    uint8_t Config = NRF24L01_ReadReg(NRF24L01_CONFIG);
    uint8_t ReceiveFlag;
    if ((Config & 0x02) == 0x00)		//配置寄存器位1（PWR_UP）为0
    	{
    		ReceiveFlag = 3;				//设备仍处于掉电模式，置标志位为3
    		NRF24L01_Init();				//接收出错，重新初始化一次设备，这样有助于设备从错误中恢复正常
    	}
    	else if ((Status & 0x30) == 0x30)	//状态寄存器位4（MAX_RT）和位5（TX_DS）同时为1
    	{
    		ReceiveFlag = 2;				//状态寄存器的值不合法，置标志位为2
    		NRF24L01_Init();				//接收出错，重新初始化一次设备，这样有助于设备从错误中恢复正常
    	}
    	else if ((Status & 0x40) == 0x40)	//状态寄存器位6（RX_DR）为1
    	{
    		ReceiveFlag = 1;				//接收到数据，置标志位为1

    		/*读接收有效载荷，存放在全局数组NRF24L01_RxPacket中，数据宽度为NRF24L01_RX_PACKET_WIDTH*/
    	    NRF24L01_CSN(0);
    	    NRF24L01_SPI_TransmitReceive(NRF24L01_R_RX_PAYLOAD);
    	    for (uint8_t i = 0; i < 8; i++) {
    	    	NRF24L01_RxPacket[i]= NRF24L01_SPI_TransmitReceive(NRF24L01_NOP);
    	    }
    	    NRF24L01_CSN(1);

    		/*给状态寄存器的位6（RX_DR）写1，清标志位*/
    		NRF24L01_WriteReg(NRF24L01_STATUS, 0x40);

    		/*清空Rx FIFO的所有数据*/
    		NRF24L01_FlushRx();
    	}
    	else
    	{
    		ReceiveFlag = 0;				//未接收到数据，置标志位为0
    	}

    	/*返回接收标志位*/
    	return ReceiveFlag;
}

// 更新接收地址
void NRF24L01_UpdateRxAddress(void) {
    NRF24L01_WriteRegs(NRF24L01_RX_ADDR_P0, NRF24L01_RxAddress, 5);
}
