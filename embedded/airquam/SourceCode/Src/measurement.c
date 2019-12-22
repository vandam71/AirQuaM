#include "measurement.h"
#include "stm32f7xx_hal.h"
#include "environment.h"
#include "gas.h"
#include "gps.h"
#include "rtc.h"
#include "cmsis_os.h"


extern TaskHandle_t taskMeasurementHandle;	//handler for the measurement task

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
measurement_t measure(void)
{
	measurement_t meas;

	meas.gas = gas_read();
	meas.environment = environment_read();
	
	meas.gps.latitude=-1000;		
	meas.gps.longitude=-1000;
	if (gps_available())
		meas.gps = gps_read();
	
	HAL_RTC_GetTime(&hrtc, &meas.time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &meas.date, RTC_FORMAT_BIN);

	return meas;
}


void vMeasurement_taskFunction(void const * argument)
{
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
}

