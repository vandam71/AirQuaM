#ifndef _AIRQUAM_H_
#define _AIRQUAM_H_

#ifdef __cplusplus
 extern "C" {
#endif
	#include "stm32f7xx_hal.h"
	#include "measurement.h"
	
	
	#define BKP_MEM_SIZE 4094
	#define	MAX_BUFFER_SIZE 	(uint32_t)( ( sizeof(uint32_t)-BKP_MEM_SIZE ) / sizeof(airquam_t) )
	
	
	
	typedef struct 
	{
		measurement_t		*buffer;
		uint32_t 				*buffer_items;
		
		uint32_t 				sampling_period;
		uint8_t 				state;
		RTC_TimeTypeDef	current_time;
		RTC_DateTypeDef	current_date;
		gps_t 					current_gps_t;
		
	}airquam_t;

	
	void airquam_init(void);
	void airquam_taskFunction(void);
	

#ifdef __cplusplus
}
#endif
	
#endif /* _AIRQUAM_H_ */
