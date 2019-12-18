#include "airquam.h"


/**
  * @brief  initialize measurement buffer on non volatile buffer
		@note		This function will verify if meaningfull data is present, if not the buffer is reset
  * @retval None
  */
void buffer_init(void)
{
	buffer_items				= 	(BKPSRAM_BASE);
	measurement_buffer	=		(BKPSRAM_BASE) + sizeof(uint32_t);
	
	//verify buffer integrity
	if( *buffer_items <= MAX_BUFFER_SIZE )							//size is good
		for(int i=*buffer_items; i<MAX_BUFFER_SIZE; i++)	//verify integrity
			if(measurement_buffer[i] != 0)									//if corrupted data or noise, reset buffer
			{
				*buffer_items = 0;
				memset(measurement_buffer, 0, MAX_BUFFER_SIZE*sizeof(airquam_t)); 
			}	
}

/**
  * @brief  init function for airquam molude
  * @retval None
  */
void airquam_init(void)
{
	gps_init();
	measurement_init();
	buffer_init();
}

/**
  * @brief  task function for the airquam task
  * @retval None
  */
void	airquam_taskFunction(void)
{
	
	
}