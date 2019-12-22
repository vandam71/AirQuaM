#ifndef _AIRQUAM_H_
#define _AIRQUAM_H_

#ifdef __cplusplus
 extern "C" {
#endif
	#include "stm32f7xx_hal.h"
	#include "measurement.h"
	#include "tim.h"
	
	#define PERIODIC_TIMER_HANDLER  htim2
	
	#define BKP_MEM_SIZE 4094
	#define	MAX_BUFFER_SIZE 	(uint32_t)( ( BKP_MEM_SIZE-sizeof(uint32_t) ) / sizeof(measurement_t) )
	
	#define DISPLAY_PAGE_TIME 10 //seconds
	
	typedef struct 
	{
		
		uint32_t 				sampling_period;		//in seconds
		uint8_t 				sampling_state;
		uint8_t 				gps_state;
		uint8_t 				wifi_state;
		measurement_t		current_meas;
		
	}airquam_t;

	
	void airquam_init(void);
	void airquam_CallBack(void);
	void vAirquam_taskFunction(void const * argument);
	

#ifdef __cplusplus
}
#endif
	
#endif /* _AIRQUAM_H_ */
