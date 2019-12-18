#include "measurement.h"
#include "stm32f7xx_hal.h"
#include "environment.h"
#include "gas.h"
#include "gps.h"
#include "rtc.h"




/**
  * @brief  init function for measurement molude
  * @retval None
  */
void measurement_init(void)
{
	gas_init();
	environment_init();
	
	
}

/**
  * @brief  read all sensors
  * @retval measurement_t object
  */
measurement_t measure (char active_sensors)
{
	measurement_t meas;
	environment_t env;
	gas_t gas;
	gps_t gps;
	
	gas = read_gas();
	meas.gas=gas;
	
	env = read_environment();
	meas.environment = env;
	
	gps = gps_read();
	
	
	
	
	return meas;
}


void	measurement_taskFunction(void)
{
	
	
	
	while (1)
  {
		
  }
}

