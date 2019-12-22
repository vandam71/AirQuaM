#ifndef _GAS_H_
#define _GAS_H_

#ifdef __cplusplus
 extern "C" {
#endif
	#include "stdint.h"
	
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
	
	
	
	void 			gas_init(void);
	gas_t 		gas_read(void);
	uint32_t	gas_readNO2(void);
	uint32_t	gas_readCO(void);
	uint32_t	gas_readCO2(void);
	uint32_t	gas_readTVOC(void);
	

#ifdef __cplusplus
}
#endif
	
#endif /* _GAS_H_ */
