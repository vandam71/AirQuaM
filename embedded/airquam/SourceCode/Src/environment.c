#include "environment.h"
#include "stm32f7xx_hal.h"
#include "hdc1080.h"
#include "ccs811.h"


/**
  * @brief  initialization process for the mics4514 and ccs811 sensors
  * @retval None
  */
void environment_init(void)
{

	hdc1080_init();
	
}

/**
  * @brief  read all gases
  * @retval gas_t object with all measurements
  */
environment_t read_environment(void)
{
	environment_t environment;
	
	hdc1080_measure_T_RH(&environment.T, &environment.RH);
	
	//update temperature and humidity compensation for ccs811 sensor
	ccs811_temp_rh_compensation(environment.T, environment.RH);
	
	return environment;
}

/**
  * @brief  read temperature
  * @retval temperature
  */
float readT(void)
{	
	return hdc1080_measureT();
}

/**
  * @brief  humidity
  * @retval humidity
  */
float readRH(void)
{
	return hdc1080_measureRH();
}
