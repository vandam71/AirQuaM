#include "measurement.h"
#include "stm32f7xx_hal.h"
#include "environment.h"
#include "gas.h"
#include "gps.h"
#include "rtc.h"



measurement_t measure (char active sensors)
{
	measurement_t meas;
	environment_t env;
	gas_t gas;
	gps_t gps;
	
	gas = read_gas(void);
	meas.gas=gas;
	
	env = read_environment(void);
	meas.evnironment = env;
	
	gps = gps_read();
	
	
}


void	measurement_taskFunction(void)
{
	
	
	
	while (1)
  {
		
  }
}

