#ifndef _AVERAGE_H_
#define _AVERAGE_H_

#include "stm32f7xx_hal.h"
#ifdef __cplusplus
 extern "C" {
#endif
	
#define AVR_LEN (1<<3)		//must be base 2

typedef struct average
{
	uint32_t average;
	uint32_t index;
	uint32_t buffer[AVR_LEN];
}average_t;
	

void average_init(average_t *);
uint32_t fast_average(average_t *avr, uint32_t new_val);


#ifdef __cplusplus
}
#endif
	
#endif /* _AVERAGE_H_ */
