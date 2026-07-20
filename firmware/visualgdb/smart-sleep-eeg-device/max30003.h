#ifndef __MAX30003_H
#define __MAX30003_H
#ifdef __cplusplus
extern "C" {
#endif
#define WREG 0x00
#define RREG 0x01
#define   NO_OP           0x00
#define   STATUS          0x01
#define   EN_INT          0x02
#define   EN_INT2         0x03
#define   MNGR_INT        0x04
#define   MNGR_DYN        0x05
#define   SW_RST          0x08
#define   SYNCH           0x09
#define   FIFO_RST        0x0A
#define   INFO            0x0F
#define   CNFG_GEN        0x10
#define   CNFG_CAL        0x12
#define   CNFG_EMUX       0x14
#define   CNFG_ECG        0x15
#define   CNFG_RTOR1      0x1D
#define   CNFG_RTOR2      0x1E
#define   ECG_FIFO_BURST  0x20
#define   ECG_FIFO        0x21
#define   RTOR            0x25	
#define LedOn HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
#define LedOff HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
#define FCLK_on HAL_TIM_PWM_Start (&htim2, TIM_CHANNEL_2)
#define FCLK_off HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2) 
#define NSS_up   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)
#define NSS_down HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
	
#define EtagValid 0x00
#define EtagFastModeValid 0x01
#define EtagLastValid 0x02
#define EtagLastFastMode 0x03
#define EtagEmpty 0x06
#define EtagOverFlow 0x07
	struct CNFG_GEN_init // CNFG_GEN структура инициализации 
	{
		uint8_t RBIASN; // << 0
		uint8_t RBIASP; // << 1
		uint8_t RBIASV; // << 2 (2 bit)
		uint8_t EN_RBIAS; // << 4 (2 bit)
		uint8_t DCLOFF_VTH; // << 6 (2 bit)
		uint8_t DCLOFF_IMAG; // << 8(3 bit)
		uint8_t DCLOFF_IPOL; //<< 11 
		uint8_t EN_DCLOFF; // << 12 (2 bit) 
		uint8_t EN_ECG; // << 19
		uint8_t FMSTR; //<< 20(2 bit)
		uint8_t EN_ULP_LON; // << 22 (2 bit)
	}; 
	struct CNFG_ECG_init  // CNFG_ECG структура инициализации
	{
		uint8_t DLPF; //<< 12 (2 bit)
		uint8_t DHPF; //<< 14 
		uint8_t GAIN; // << 16 2 bit
		uint8_t RATE; // << 22(bit)		
	}; 
	struct MNGR_DYN_init // MNGR_DYN структура инициализации 
	{
		uint8_t FAST_TH; // <<16 (6 bit) [16:21]
		uint8_t FAST; // << << 22(2 bit [22:23]
		
	}; 
	struct  CNFG_CAL_init // CNFG_CAL структура инициализации 
	{
		uint8_t THIGH_L;  //THIGH[7:0]
		uint8_t THIGH_H; // THIGH[10:8]
		uint8_t FIFTY; // FIFTY[11]
		uint8_t FCAL; // FCAL[14:12]
		uint8_t VMAG; // VMAG
		uint8_t VMODE;
		uint8_t EN_VCAL; // EN_VCAL
	};
	struct CNFG_EMUX_init // CNFG_EMUX структура инициализации
	{
		uint8_t CALN_SEL;
		uint8_t CALP_SEL;
		uint8_t OPENN;
		uint8_t OPENP;
		uint8_t POL;
	}; 
	struct RO_STATUS// Структура регистра STATUS 
	{
		uint8_t LDOFF_NL; // [0]
		uint8_t LDOFF_NH; // [1]
		uint8_t LDOFF_PL; // [2]
		uint8_t LDOFF_PH; // [3]
		uint8_t PLLINT; // [8]
		uint8_t SAMP; // [9]
		uint8_t RRINT; // [10]
		uint8_t LONINT; // [11]
		uint8_t DCLOFFINT; // [20]
		uint8_t FSTINT; // [21]
		uint8_t EOVF; // [22]
		uint8_t EINT; // [23]
	};
	struct EN_INT_init // EN_INT структура инициализации
	{
		uint8_t INTB_TYPE; // [1:0]
		uint8_t EN_PLLINT; // [8]
		uint8_t EN_SAMP;//[9]
		uint8_t EN_RRINT;//[10]
		uint8_t EN_LONINT;//[11]
		uint8_t EN_EINT;//[23]
		uint8_t EN_EOVF;//[22]
		uint8_t EN_FSTINT;//[21]
		uint8_t EN_DCLOFFINT;//[20]
		
		
		
	};
	void MAX_WREG(uint8_t REG, uint8_t *data);
	void MAX_RREG(uint8_t REG, uint8_t *rxBuff);
	struct CNFG_GEN_init MAX_CNFG_GEN_read();
	void MAX_CNFG_GEN_init(struct CNFG_GEN_init reg);
	void MAX_CNFG_ECG_init(struct CNFG_ECG_init reg); 
	struct CNFG_ECG_init MAX_CNFG_ECG_read();
	void MAX_MNGR_DYN_init(struct MNGR_DYN_init cfg);
	struct MNGR_DYN_init MAX_MNGR_DYN_read();
	void MAX_CNFG_CAL_init(struct CNFG_CAL_init cfg);
	struct CNFG_CAL_init MAX_CNFG_CAL_read();
	void  MAX_CNFG_EMUX_init(struct CNFG_EMUX_init cfg);
	struct CNFG_EMUX_init MAX_CNFG_EMUX_init_read();
	void  MAX_EN_INT_init(struct EN_INT_init cfg, uint8_t intNuber);
	struct RO_STATUS MAX_Status_read();
	uint32_t MAX_GetData(uint8_t *fifo);
	uint32_t  MAX_readFIFO();
	void MAX_FIFO_RESET();
	void MAX_SYNCH();
	void MAX_SW_RESET();
	signed int PositiveToNegative(uint32_t data);
	void MAX_WriteToBuff(uint32_t *buff, uint16_t counter);
	void MAX_Transmit_sample(uint32_t *buff, uint8_t size);
	static void in32ToByteArray(uint32_t val, uint8_t *buff);
    void clearBuff(uint8_t *buff, uint16_t size);
	void MAX_WriteBuff_8bit(uint8_t *buff, uint16_t count);
	uint8_t CheckEtag(uint8_t sample);
	uint8_t FifoReadyCheck();
#ifdef __cplusplus
}
#endif

#endif 

