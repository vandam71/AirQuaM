#ifndef _GAS_H_
#define _GAS_H_

#ifdef __cplusplus
 extern "C" {
#endif
	#include "stm32f7xx_hal.h"
	
	#define MICS4514_NOX_R0 1022	//Reference resistance for the NO2 sensing element
	#define MICS4514_RED_R0 1000000	//Reference resistance for the NO2 sensing element
	
	#define MICS4514_NOX_PREHEAT 1	//seconds for preheating the NO2 sensing element
	
	typedef struct
	{
		uint32_t NO2;			//in ppb
		uint32_t CO;			//in ppb
		uint32_t CO2;			//in ppb
		uint32_t TVOC;		//in ppb
	}gas_t;
	
	
	
	void gas_init(void);
	gas_t read(void);
	uint32_t readNO2(void);
	uint32_t readCO(void);
	uint32_t readCO2(void);
	uint32_t readTVOC(void);
	

#ifdef __cplusplus
}
#endif
	
#endif /* _GAS_H_ */
