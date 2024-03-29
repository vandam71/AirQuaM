#ifndef _MEASUREMENT_H_
#define _MEASUREMENT_H_

#ifdef __cplusplus
 extern "C" {
#endif
	#include "stm32f7xx_hal.h"
	#include "environment.h"
	#include "gas.h"
	#include "gps.h"
	#include "rtc.h"

	
	typedef struct
	{
		RTC_TimeTypeDef time;
		RTC_DateTypeDef date;
		gps_t gps
		gas_t gas;
		environment_t environment;
	}measurement_t;
	
	
	measurement_t measure (char);
	

#ifdef __cplusplus
}
#endif
	
#endif /* _MEASUREMENT_H_ */
