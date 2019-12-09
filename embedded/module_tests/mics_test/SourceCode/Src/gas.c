#include "gas.h"
#include "stm32f7xx_hal.h"
#include "gpio.h"
#include "adc.h"
#include "math.h"








/**
  * @brief  initialization process for the mics4514 and ccs811 sensors
  * @retval None
  */
void gas_init(void)
{
	MX_ADC1_Init();
  MX_ADC2_Init();
	
	
	//enable preheat for NO2 sensing element and wait
	HAL_GPIO_WritePin(MICS_PRE_GPIO_Port, MICS_PRE_Pin, GPIO_PIN_SET);
	HAL_Delay(MICS4514_NOX_PREHEAT*1000);
	HAL_GPIO_WritePin(MICS_PRE_GPIO_Port, MICS_PRE_Pin, GPIO_PIN_RESET);
	
}


gas_t read(void)
{
	gas_t gas;
	
	gas.NO2 = readNO2();
	gas.CO = readCO();
	gas.CO2 = readCO2();
	gas.TVOC = readTVOC();
	
	return gas;
}

uint32_t readNO2(void)
{	
	uint32_t adc_value;
	double voltage, Rs, ppb;
	
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	adc_value = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);
	
	if (!adc_value) //invalid value
		return 0;	
	
	voltage = (3.3*adc_value) / 4096;
	Rs = ((5/( voltage *2))-1)*19800;																	//calculate sensing resistance
	ppb = 1000*powf(10, 0.9682*(logf( Rs / MICS4514_NOX_R0 )/logf(10))-0.8108);	//calculate gas concentration
	
	return (uint32_t)ppb;
}

uint32_t readCO(void)
{
	uint32_t adc_value;
	double voltage, Rs, ppb;
	
	HAL_ADC_Start(&hadc2);
	HAL_ADC_PollForConversion(&hadc2, HAL_MAX_DELAY);
	adc_value = HAL_ADC_GetValue(&hadc2);
	HAL_ADC_Stop(&hadc2);
	
	if (!adc_value) //invalid value
		return 0;	
	
	voltage = (3.3*adc_value) / 4096;
	Rs = ((5/voltage)-1)*47000;																					//calculate sensing resistance
	ppb = 1000*powf(10, -1.1859*(logf( Rs / MICS4514_RED_R0 )/logf(10))+0.6201);	//calculate gas concentration
	
	return (uint32_t)ppb;
}

uint32_t readCO2(void)
{
	
}

uint32_t readTVOC(void)
{
	
}
