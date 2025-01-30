#include "main.h"





static uint8_t RST[4] = { 0x00, 0x00, 0x00, 0x00 };
void MAX_WREG(uint8_t REG, uint8_t *data) //Запись в регистр 
{
	uint8_t cmdBuffer[4];
	cmdBuffer[0] = (REG << 1) | WREG;
	for (int i = 1; i < 4; i++)
	{
		cmdBuffer[i] = *(data + i);
	} 
	NSS_down;
	HAL_SPI_Transmit(&hspi1, cmdBuffer, 4, 10);

	NSS_up;
}

void MAX_RREG(uint8_t REG, uint8_t *rxBuff) // Чтение Регистра 
{
	
	uint8_t txBuff = (REG << 1) | RREG;
	NSS_down;
	HAL_SPI_Transmit(&hspi1, &txBuff, 1, 0);
	HAL_SPI_Receive(&hspi1, rxBuff, 3, 10);
	NSS_up;
}

void MAX_CNFG_GEN_init(struct CNFG_GEN_init reg)
{
	//формирование иструкции инициализации////////////////////
	uint8_t buf[4] = { 0, 0, 0, 0 };
	buf[3] =  reg.RBIASN << 0 //  первого байта регистра CNFG_GEN - [0:7]
			| reg.RBIASP << 1 
			| reg.RBIASV << 2
			| reg.EN_RBIAS << 4
			| reg.DCLOFF_VTH << 6;
	buf[2] = reg.DCLOFF_IMAG << 0 //  второго байта регистра CNFG_GEN - [8:15]
			| reg.DCLOFF_IPOL << 3
			| reg.EN_DCLOFF << 4;
			
	buf[1] = reg.EN_ECG << 3 // Третьего байта регистра CNFG_GEN - [16:23]
			| reg.FMSTR << 4
			| reg.EN_ULP_LON << 6;
	MAX_WREG(CNFG_GEN, buf);
	
}

struct CNFG_GEN_init MAX_CNFG_GEN_read()
{
	struct CNFG_GEN_init cfg;
	uint8_t rxBuf[4] = { 0, };
	MAX_RREG(CNFG_GEN, rxBuf); //Читаем регистр в rxBuf
	// Записываем первый байт/////////////////////////
	cfg.RBIASN = rxBuf[2];  
	cfg.RBIASP = rxBuf[2];
	cfg.RBIASV = rxBuf[2];
	cfg.EN_RBIAS = rxBuf[2];
	cfg.DCLOFF_VTH = rxBuf[2];
	
	cfg.RBIASN  = (cfg.RBIASN & 0x01) >> 0;  
	cfg.RBIASP = (cfg.RBIASP & 0x02) >> 1; 
	cfg.RBIASV = (cfg.RBIASV & 0x0C) >> 2;
	cfg.EN_RBIAS = (cfg.EN_RBIAS & 0x30) >> 4;
	cfg.DCLOFF_VTH = (cfg.DCLOFF_VTH & 0xC0) >> 6;
	
	//Записываем второй байт//////////////////////
	
	cfg.DCLOFF_IMAG = rxBuf[1];
	cfg.DCLOFF_IPOL = rxBuf[1];
	cfg.EN_DCLOFF =  rxBuf[1];
		
	cfg.DCLOFF_IMAG = (cfg.DCLOFF_IMAG & 0x07) >> 0;
	cfg.DCLOFF_IPOL = (cfg.DCLOFF_IPOL & 0x08)  >> 3;
	cfg.EN_DCLOFF = (cfg.EN_DCLOFF & 0x30) >> 4;
	
	//Записываем третий байт//////////////////////// 
	
	cfg.EN_ECG = rxBuf[0];
	cfg.FMSTR = rxBuf[0];
	cfg.EN_ULP_LON = rxBuf[0];
	
	cfg.EN_ECG = (cfg.EN_ECG & 0x08) >> 3;
	cfg.FMSTR = (cfg.FMSTR & 0x30) >> 4;
	cfg.EN_ULP_LON = (cfg.EN_ULP_LON & 0xC0) >> 6;
	
	////////////////////////////////////////////////
	
	return cfg;
};

void MAX_CNFG_ECG_init(struct CNFG_ECG_init reg) 
{ 
	uint8_t buf[4] = { 0, 0, 0, 0 };
	buf[2] = reg.DLPF << 4
			| reg.DHPF << 6;
	buf[1] = reg.GAIN << 0
			| reg.RATE << 6;
	MAX_WREG(CNFG_ECG, buf);		
}

struct CNFG_ECG_init MAX_CNFG_ECG_read()
{
	struct CNFG_ECG_init reg;
	uint8_t rxBuf[4] = { 0, }; 
	MAX_RREG(CNFG_ECG, rxBuf); //Читаем регистр в rxBuf
	//Записываем первый байт///////////////////////////
	reg.DHPF = rxBuf[1]; 
	reg.DLPF = rxBuf[1];
	
	reg.DLPF = (reg.DLPF & 0x30) >> 4;
	reg.DHPF = (reg.DHPF & 0x40) >> 6;
	
	//Забисываем второй байт////////////////////////// 
	reg.GAIN = rxBuf[0];
	reg.RATE = rxBuf[0];
	
	reg.GAIN = (reg.GAIN & 0x03) >> 0;
	reg.RATE = (reg.RATE & 0xC0) >> 6;
	
	//////////////////////////////////////////////////
	
	return reg;
};

void MAX_MNGR_DYN_init(struct MNGR_DYN_init cfg)
{
	uint8_t buf[4] = { 0, 0, 0, 0 };
	buf[1] = cfg.FAST_TH << 0
			| cfg.FAST << 6;
	MAX_WREG(MNGR_DYN, buf);
}

struct MNGR_DYN_init MAX_MNGR_DYN_read()
{
	struct MNGR_DYN_init reg = {};
	uint8_t rxBuf[4] = { 0, }; 
	MAX_RREG(MNGR_DYN, rxBuf); //Читаем регистр в rxBuf
	
	reg.FAST_TH = rxBuf[0];
	reg.FAST = rxBuf[0];
	
	reg.FAST_TH = (reg.FAST_TH & 0x3F) >> 0;
	reg.FAST = (reg.FAST & 0xC0) >> 6;
	return reg;
	
};

void MAX_CNFG_CAL_init(struct CNFG_CAL_init cfg)
{
	uint8_t buf[4] = { 0, 0, 0, 0 };
	
	buf[3] = cfg.THIGH_L;
	
	buf[2] = cfg.THIGH_H << 0
			| cfg.FIFTY << 3
			| cfg.FCAL << 4;
	
	buf[1] = cfg.VMAG << 4
			| cfg.VMODE << 5
			| cfg.EN_VCAL << 6;
	MAX_WREG(CNFG_CAL, buf);
}

struct CNFG_CAL_init MAX_CNFG_CAL_read()
{
	struct CNFG_CAL_init reg = { };
	uint8_t rxBuf[4] = { 0, }; 
	MAX_RREG(CNFG_CAL, rxBuf); //Читаем регистр в rxBuf
	//Записываем первый байт///////////////////////////
	reg.THIGH_L = rxBuf[2];
	
	//Забисываем второй байт////////////////////////// 
	reg.THIGH_H = rxBuf[1];
	reg.FIFTY = rxBuf[1];
	reg.FCAL = rxBuf[1];
	
	reg.THIGH_H = (reg.THIGH_H & 0x07) >> 0;
	reg.FIFTY = (reg.FIFTY & 0x08) >> 3;
	reg.FCAL = (reg.FCAL & 0x70) >> 4;
	
	//Забисываем третий байт////////////////////////// 
	reg.VMAG = rxBuf[0];
	reg.VMODE = rxBuf[0];
	reg.EN_VCAL = rxBuf[0]; 
	
	reg.VMAG = (reg.VMAG & 0x10) >> 4;
	reg.VMODE = (reg.VMODE & 0x20) >> 5;
	reg.EN_VCAL = (reg.EN_VCAL & 0x40) >> 6;
	
	
	return reg;
}; 

void  MAX_CNFG_EMUX_init(struct CNFG_EMUX_init cfg) 
{ 
	uint8_t buf[4] = { 0, 0, 0, 0 };
	
	buf[1] = cfg.CALN_SEL << 0
			| cfg.CALP_SEL << 2
			| cfg.OPENN << 4
			| cfg.OPENP << 5
			| cfg.POL << 7;
	MAX_WREG(CNFG_EMUX, buf);
}

struct CNFG_EMUX_init MAX_CNFG_EMUX_init_read()
{
	struct  CNFG_EMUX_init reg = { };
	uint8_t rxBuf[4] = { 0, }; 
	MAX_RREG(CNFG_EMUX, rxBuf); //Читаем регистр в rxBuf
	reg.CALN_SEL = rxBuf[0];
	reg.CALP_SEL = rxBuf[0];
	reg.OPENN = rxBuf[0];
	reg.OPENP = rxBuf[0];
	reg.POL = rxBuf[0];
	
	reg.CALN_SEL = (reg.CALN_SEL & 0x03) >> 0;
	reg.CALP_SEL = (reg.CALP_SEL & 0x0C) >> 2;
	reg.OPENN = (reg.OPENN & 0x01) >> 4;
	reg.OPENP = (reg.OPENP & 0x20) >> 5;
	reg.POL = (reg.POL & 0x80) >> 7;
	
	return reg;
};

struct RO_STATUS MAX_Status_read()
{
	struct  RO_STATUS status = { };
	uint8_t rxBuf[4] = { 0, }; 
	MAX_RREG(CNFG_CAL, rxBuf); //Читаем регистр в rxBuf
	
	status.LDOFF_NL = rxBuf[2];
	status.LDOFF_NH = rxBuf[2];
	status.LDOFF_PL = rxBuf[2];
	status.LDOFF_PH = rxBuf[2];
	
	status.LDOFF_NL = (status.LDOFF_NL & 0x01) >> 0;
	status.LDOFF_NH = (status.LDOFF_NH & 0x02) >> 1;
	status.LDOFF_PL = (status.LDOFF_PL & 0x04) >> 2;
	status.LDOFF_PH = (status.LDOFF_PH & 0x08) >> 3;
	
	status.PLLINT = rxBuf[1];
	status.SAMP = rxBuf[1];
	status.RRINT = rxBuf[1];
	status.LONINT = rxBuf[1];
	
	status.PLLINT = (status.PLLINT & 0x01) >> 0;
	status.SAMP = (status.SAMP & 0x02) >> 1;
	status.RRINT = (status.RRINT & 0x04) >> 2;
	status.LONINT = (status.LONINT & 0x08) >> 3;
	
	status.DCLOFFINT = rxBuf[0];
	status.FSTINT = rxBuf[0];
	status.EOVF = rxBuf[0];
	status.EINT = rxBuf[0];
	
	status.DCLOFFINT = (status.DCLOFFINT & 0x10) >> 4;
	status.FSTINT = (status.FSTINT & 0x20) >> 5;
	status.EOVF = (status.EOVF & 0x40) >> 6; 
	status.EINT = (status.EINT & 0x80) >> 7;
	
	return status;
	
};

void  MAX_EN_INT_init(struct EN_INT_init cfg, uint8_t intNuber) 
{ 
	uint8_t buf[4] = { 0, 0, 0, 0 };
	
	buf[3] = cfg.INTB_TYPE << 0;
	buf[2] = cfg.EN_PLLINT 
			| cfg.EN_SAMP << 1
			| cfg.EN_RRINT << 2
			| cfg.EN_LONINT << 3;
	
	buf[1] = cfg.EN_DCLOFFINT << 4
			| cfg.EN_FSTINT << 5
			| cfg.EN_EOVF << 6
			| cfg.EN_EINT << 7;	
	switch (intNuber)
	{
	case 1:
		MAX_WREG(EN_INT, buf);
		break;
	case 2:
		MAX_WREG(EN_INT2, buf);
	default:
		break;
	}
	
}
static uint8_t MAX_GetEtag(uint8_t *fifo)
{
	uint8_t eTag = (*(fifo + 2) &= 0x38) >> 3;
	return eTag;
}

static uint8_t MAX_GetPtag(uint8_t *fifo)
{
	uint8_t pTag = (*(fifo + 2) &= 0x07) >> 0;
	return pTag;
}

uint32_t MAX_GetData(uint8_t *fifo)
{
	uint32_t data1 = (*(fifo) << 16);
	uint32_t data2 = (*(fifo + 1) << 8);
	uint32_t data3 = (*(fifo + 2));
	uint32_t data = data1 | data2 | data3;
	data = data >> 6;
	return data;
}
uint32_t  MAX_readFIFO()
{
	uint8_t rawData[4] = { 0, };
	MAX_RREG(ECG_FIFO_BURST, rawData);
	if ((MAX_GetEtag(rawData) == 0x02) || (MAX_GetEtag(rawData) == 0x00) || (MAX_GetPtag(rawData) == 0x00) || (MAX_GetPtag(rawData) == 0x02))
	{
		HAL_UART_Transmit(&huart1, rawData, 3 , 10);
		uint32_t data = MAX_GetData(rawData);
		return data;
	}
	else
	{
		MAX_FIFO_RESET();
		return 0;
	}
}
void MAX_FIFO_RESET()
{
	MAX_WREG(FIFO_RST, RST);
}

void MAX_SYNCH()
{
	MAX_WREG(SYNCH, RST);
}

void MAX_SW_RESET()
{
	MAX_WREG(SW_RST, RST);
}

signed int PositiveToNegative(uint32_t data)
{
	signed int signedData = data;
	if (data > 131072)
	{
		signedData = ((data ^= 0x3FFFF) + 1) * -1;
		
	}
	
	
	
	return signedData;
}

void MAX_WriteToBuff(uint32_t *buff , uint16_t counter)
{
		uint8_t temp[3] = { 0, };
		MAX_RREG(ECG_FIFO, temp);
		buff[counter] = MAX_GetData(temp);
}

static void in32ToByteArray(uint32_t val, uint8_t *buff)
{
	buff[0] = val >> 16;
	buff[1] = (val >> 8);
	buff[2] = (val >> 0);
}

void clearBuff(uint8_t *buff, uint16_t size)
{
	for (uint16_t i = 0; i < size; i++)
	{
		buff[i] = 0;
	}
}
void MAX_Transmit_sample(uint32_t *buff, uint8_t size)
{
	uint8_t temp[3] = { 0, };
	for (uint8_t i = 0; i < size; i++)
	{
		in32ToByteArray(buff[i], temp);
		HAL_UART_Transmit_DMA(&huart1, temp, 3);
		clearBuff(temp, 3);
	}
}

uint8_t FifoReadyCheck()
{
	uint8_t temp[3] = { 0, };
	MAX_RREG(STATUS, temp);
	if ((temp[0] &= 0x80) == 0x80)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t CheckEtag(uint8_t sample)				
{												
	return (sample >> 3) & 0x07;						
			 
}

void MAX_WriteBuff_8bit(uint8_t *buff, uint16_t count)
{
	uint8_t temp[3] = { 0, };

		MAX_RREG(ECG_FIFO, temp);
		uint8_t  etag = CheckEtag(temp[2]);
	if (etag == EtagValid || etag == EtagLastValid)
	{		
			for (uint16_t j = 0; j < 3; j++)
			{
				buff[(3*count + j)] = temp[j]; 
			}				
		
	}
	

	
		
		//clearBuff(temp, 3);
	
}

