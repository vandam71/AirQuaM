#ifndef _MICS4514_H_
#define _MICS4514_H_

#ifdef __cplusplus
 extern "C" {
#endif
	#include "stdint.h"
	
	#define MICS4514_NOX_R0 1022	//Reference resistance for the NO2 sensing element
	#define MICS4514_RED_R0 1000000	//Reference resistance for the NO2 sensing element
	
	#define MICS4514_NOX_PREHEAT 1	//seconds for preheating the NO2 sensing element
		
	
	void 			mics4514_init(void);
	uint32_t	mics4514_measureNO2(void);
	uint32_t	mics4514_measureCO(void);
	

#ifdef __cplusplus
}
#endif
	
#endif /* _MICS4514_H_ */
