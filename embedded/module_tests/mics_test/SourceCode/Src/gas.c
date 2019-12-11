#include "gas.h"
#include "stm32f7xx_hal.h"
#include "average.h"
#include "mics4514.h"
#include "ccs811.h"


//sliding average objects
static average_t avrNO2;
static average_t avrCO;
static average_t avrCO2;
static average_t avrTVOC;



/**
  * @brief  initialization process for the mics4514 and ccs811 sensors
  * @retval None
  */
void gas_init(void)
{
	
	mics4514_init();
	ccs811_init();
	
	average_init(&avrNO2, measureNO2());
	average_init(&avrCO, measureCO());
	average_init(&avrCO2, measureCO2());
	average_init(&avrTVOC, measureTVOC());
	
}

/**
  * @brief  read all gases
  * @retval gas_t object with all measurements
  */
gas_t read_gas(void)
{
	gas_t gas;
	
	gas.NO2 = readNO2();
	gas.CO = readCO();
	measure_CO2_TVOC(&gas.CO2, &gas.TVOC);
	
	return gas;
}

/**
  * @brief  read NO2 gas
  * @retval filtered value of NO2 measurement
  */
uint32_t readNO2(void)
{	
	return fast_average(&avrNO2, measureNO2() );
}

/**
  * @brief  read CO gas
  * @retval filtered value of CO measurement
  */
uint32_t readCO(void)
{
	return fast_average(&avrCO, measureCO() );
}

/**
  * @brief  read CO2 gas
  * @retval filtered value of CO2 measurement
  */
uint32_t readCO2(void)
{
	return fast_average(&avrCO2, measureCO2() );
}

/**
  * @brief  read TVOC gas
  * @retval filtered value of TVOC measurement
  */
uint32_t readTVOC(void)
{
	return fast_average(&avrTVOC, measureTVOC() );
}
