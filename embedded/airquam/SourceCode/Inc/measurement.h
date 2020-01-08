#ifndef _MEASUREMENT_H_
#define _MEASUREMENT_H_

#ifdef __cplusplus
 extern "C" {
#endif
	#include "stm32f7xx_hal.h"
	#include "airquam.h"
	#include "environment.h"
	#include "wifi.h"
	#include "gas.h"
	#include "gps.h"
	#include "rtc.h"
	
	
	#define BKP_BUF_SIZE 				(uint32_t)( 4096 - WIFI_BKP_MEM_SIZE - BKP_AIRQUAM_SIZE )
	#define	BKP_BUFF_BASE 			(uint32_t)( BKP_AIRQUAM_BASE + BKP_AIRQUAM_SIZE )
	#define	BKP_BUFF_MAX_ITEMS	(uint32_t)( ( BKP_BUF_SIZE-sizeof(uint32_t) ) / sizeof(measurement_t) )
	
	
	typedef struct
	{
		RTC_TimeTypeDef time;
		RTC_DateTypeDef date;
		gps_t gps;
		gas_t gas;
		environment_t environment;
	}measurement_t;
	
	
	void measurement_bkp_buffer_push(measurement_t meas);
	measurement_t measurement_bkp_buffer_pop(void);
	uint16_t measurement_bkp_buffer_isNotFull(void);
	uint16_t measurement_bkp_buffer_isNotEmpty(void);
	
	void measurement_init(void);
	uint8_t measurement_validGps(measurement_t);
	measurement_t measure(void);
	void vMeasurement_taskFunction(void const * argument);
	
	

#ifdef __cplusplus
}
#endif
	
#endif /* _MEASUREMENT_H_ */
