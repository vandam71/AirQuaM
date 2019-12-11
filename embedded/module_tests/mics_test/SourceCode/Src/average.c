#include "average.h"
#include "string.h"

/**
  * @brief  initialize the average_t struct
  * @retval None
  */
void average_init(average_t *avr, uint32_t initial)
{
	avr->average = initial;
	avr->index = 0;

	for(int i=0; i<AVR_LEN; i++)
		avr->buffer[i] = initial;
}
	
/**
  * @brief  fast algorithm to calculate the moving average
	* @param	avr 		pointer to the average_t struct to be updated
	* @param	new_val	new value to be added to the moving average
  * @retval new average
  */
uint32_t fast_average(average_t *avr, uint32_t new_val)
{
	uint32_t old_val;
	
	old_val = avr->buffer[(avr->index & (AVR_LEN - 1))];
	avr->buffer[(avr->index++ & (AVR_LEN - 1))] = new_val;
	
	avr->average = avr->average - (old_val/AVR_LEN) + (new_val/AVR_LEN);
	
	return avr->average;
}
