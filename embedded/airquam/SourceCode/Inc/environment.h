#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_

#ifdef __cplusplus
 extern "C" {
#endif
	#include "stm32f7xx_hal.h"
	
	typedef struct 
	{
		float T;			//temperature in celcius
		float RH;			//relative humidity in percentage
	}environment_t;
	
	
	
	void environment_init(void);
	environment_t read_environment(void);
	float readT(void);
	float readRH(void);
	

#ifdef __cplusplus
}
#endif
	
#endif /* _ENVIRONMENT_H_ */
