#include "airquam.h"
#include "stm32f7xx_hal.h"
#include "measurement.h"
#include "environment.h"
#include "gas.h"
#include "gps.h"
#include "rtc.h"
#include "stdio.h"
#include "string.h" 
#include "cmsis_os.h"


static uint32_t*				measurement_bkp_buffer_items;		
static measurement_t*		measurement_bkp_buffer;					


static SemaphoreHandle_t xBkpBufferMutex;


/**
  * @brief  clear the non volatile buffer
  * @retval None
  */
static inline void measurement_bkp_buffer_erase(void)
{
	memset((uint32_t*)(BKPSRAM_BASE), 0, 4096); 	
}
	
/**
  * @brief  initialize measurement buffer on non volatile buffer
	*	@note		This function will verify if meaningfull data is present, if not the buffer is reset
  * @retval None
  */
static void measurement_bkp_buffer_init(void)
{

	measurement_bkp_buffer_items =	 (uint32_t*)(BKP_BUFF_BASE);
	measurement_bkp_buffer	=		(measurement_t*)((BKP_BUFF_BASE) + sizeof(uint32_t));
	
	measurement_t	null_meas;
	memset(&null_meas, 0, sizeof(measurement_t)); 
	
	//verify buffer integrity
	if( *measurement_bkp_buffer_items > BKP_BUFF_MAX_ITEMS )		//size variable is corrupted
	{
		measurement_bkp_buffer_erase();
		return;
	}
	for(int i=*measurement_bkp_buffer_items; i<BKP_BUFF_MAX_ITEMS; i++)	//verify integrity
		if( memcmp( &measurement_bkp_buffer[i], &null_meas, sizeof(measurement_t) ) != 0)		//if corrupted data or noise, reset buffer
		{
			measurement_bkp_buffer_erase();
			return;
		}	
	return;
}

/**
  * @brief  push a measurement_t object into the backup buffer
	* @param	meas measurement_t object to go into the buffer
  * @retval None
  */
void measurement_bkp_buffer_push(measurement_t meas)
{
	xSemaphoreTake( xBkpBufferMutex, pdMS_TO_TICKS(5000));			//wait for mutex
	if(*measurement_bkp_buffer_items < BKP_BUFF_MAX_ITEMS)									//if buffer not full
	{
		measurement_bkp_buffer[*measurement_bkp_buffer_items] = meas;
		*measurement_bkp_buffer_items = *measurement_bkp_buffer_items+1;
	}
	xSemaphoreGive( xBkpBufferMutex );													//release mutex
}

/**
  * @brief  pop a measurement_t object from the backup buffer
	* @retval	measurement_t object taken from the buffer
  */
measurement_t measurement_bkp_buffer_pop(void)
{
	measurement_t meas;
	xSemaphoreTake( xBkpBufferMutex, pdMS_TO_TICKS(5000));			//wait for mutex
	if(*measurement_bkp_buffer_items < BKP_BUFF_MAX_ITEMS)									//if buffer not empty
	{
		*measurement_bkp_buffer_items = *measurement_bkp_buffer_items-1;
		meas = measurement_bkp_buffer[*measurement_bkp_buffer_items];
		memset(&measurement_bkp_buffer[*measurement_bkp_buffer_items], 0, sizeof(measurement_t));
	}
	xSemaphoreGive( xBkpBufferMutex );													//release mutex
	return meas;
}

/**
  * @brief  check the remaining space in the buffer
	* @retval	remaining measurement_t spaces
  */
uint16_t measurement_bkp_buffer_isNotFull(void)
{
	uint16_t items;
	xSemaphoreTake( xBkpBufferMutex, pdMS_TO_TICKS(5000));			//wait for mutex
	items = *measurement_bkp_buffer_items;
	xSemaphoreGive( xBkpBufferMutex );													//release mutex
	
	return BKP_BUFF_MAX_ITEMS-items;
}

/**
  * @brief  check the items in the buffer
	* @retval	measurement_t items in buffer
  */
uint16_t measurement_bkp_buffer_isNotEmpty(void)
{
	uint16_t items;
	xSemaphoreTake( xBkpBufferMutex, pdMS_TO_TICKS(5000));			//wait for mutex
	items = *measurement_bkp_buffer_items;
	xSemaphoreGive( xBkpBufferMutex );													//release mutex
	
	return items;
}

/**
  * @brief  init function for measurement molude
  * @retval None
  */
void measurement_init(void)
{
	measurement_bkp_buffer_init();
	
	gas_init();
	environment_init();
	
	xBkpBufferMutex = xSemaphoreCreateMutex();
}

/**
  * @brief  read all sensors
  * @retval measurement_t object
  */
measurement_t measure(void)
{
	measurement_t meas;

	meas.gas = gas_read();
	meas.environment = environment_read();
	
	meas.gps.latitude=-1000;		
	meas.gps.longitude=-1000;
	if (gps_available())
		meas.gps = gps_read();
	
	HAL_RTC_GetTime(&hrtc, &meas.time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &meas.date, RTC_FORMAT_BIN);

	return meas;
}


