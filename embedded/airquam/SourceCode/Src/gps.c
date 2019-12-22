#include "gps.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cmsis_os.h"

static volatile gps_t gps;
static volatile uint8_t gps_availability = 0;

static uint8_t buff_arr[2][512]={0};					//declare two buffers
static volatile uint8_t buff_index=0;					//
static volatile uint8_t buff_receiving = 0;		//which buffer is receiveing

extern TaskHandle_t taskGpsHandle;	//handler for the gps task

/**
  * @brief  convert the gps NMEA format to degrees
  * @retval decimal degrees
  */
float convertDegMinToDecDeg (float degMin)
{
  double min = 0.0;
  double decDeg = 0.0;
 
  //get the minutes, fmod() requires double
  min = fmod((double)degMin, 100.0);
 
  //rebuild coordinates in decimal degrees
  degMin = (int) ( degMin / 100 );
  decDeg = degMin + ( min / 60 );
 
  return decDeg;
}

/**
  * @brief  NMEA processing routine
  * @retval GPS_SUCESS if gps was found, GPS_FAIL if not
  */
char gps_process(gps_t *my_gps, uint8_t *rxBuffer)
{		
	float lat, lon;
	char	NS_Indicator = 0, EW_Indicator = 0;
	char	*str;
	
	str=strstr((char*)rxBuffer,"$GNGGA,");
	
	if(str == NULL)
		return GPS_FAIL;

	sscanf(str,"$GNGGA,%*2hhd%*2hhd%*2hhd.%*3hd,%f,%c,%f,%c,%*hhd,%*hhd,%*f,%*f,%*c,%*hd,%*s,*%*2s\r\n",
		&lat, &NS_Indicator, &lon, &EW_Indicator);
	
	
	if(NS_Indicator == 0 || EW_Indicator == 0)	//no position detected
		return GPS_FAIL;
	
	my_gps->latitude		= convertDegMinToDecDeg(lat);
	my_gps->longitude	= convertDegMinToDecDeg(lon);		

	if (NS_Indicator == 'S' || NS_Indicator == 's')	//apply direction
		my_gps->latitude *= -1;
	if (EW_Indicator == 'W' || EW_Indicator == 'w')
		my_gps->longitude *= -1;
		
	return GPS_SUCESS;
}

/**
  * @brief  init function for gps molude
  * @retval None
  */
void	gps_init(void)
{
	buff_index=0;
	buff_receiving=0;
	gps_availability=0;
	HAL_UART_Receive_IT(&GPS_UART_HANDLER, &buff_arr[buff_receiving][buff_index], 1);

}

/**
  * @brief  uart callback routine
  * @retval None
  */
void	gps_CallBack(void)
{
		if(buff_arr[buff_receiving][buff_index] == '\n')		//end of line
		{
			buff_arr[buff_receiving][buff_index+1] = '\0';
			buff_receiving = !buff_receiving; //switch buffer
			buff_index = 0;
			
			// notification give (binnary semaphore like)
			//vTaskNotifyGiveFromISR( taskGpsHandle, NULL );
		
		}
		else if(buff_arr[buff_receiving][buff_index] == '\r');
		else																								//new char
		{
			buff_index++;
			buff_index &= ~(1<<7); //keep inside the limits
		}
		//set the interrups for UART3 Rx again
		HAL_UART_Receive_IT(&GPS_UART_HANDLER, &buff_arr[buff_receiving][buff_index], 1);
}

/**
  * @brief  check if a recent valid gps position has been received
	* @retval availability state (1: yes, 0: no)
  */
uint8_t gps_available(void)
{
	return gps_availability;
}

/**
  * @brief  get method for the gps module
  * @retval gps_t object
  */
gps_t gps_read(void)
{
	return gps;
}

/**
  * @brief  task function for the gps task
  * @retval None
  */
void vGps_taskFunction(void const * argument)
{
	gps_t my_gps;
	static uint32_t last_gps_tick=0; //in ms
			printf("hellogps \n");
  /* Infinite loop */
	for(;;)
  {
		// notification take (binnary semaphore like)
		ulTaskNotifyTake( pdTRUE, pdMS_TO_TICKS(1000) );  

		if ( gps_process(&my_gps, &buff_arr[!buff_receiving][0]) == GPS_SUCESS) //process data
		{
			gps = my_gps;
			gps_availability = 1;
			last_gps_tick = HAL_GetTick();		//reset timeout if gps is available
		}
		else																//if the timeout exeeds, set availability accordingly
			if( gps_availability )				
				if( (HAL_GetTick()-last_gps_tick) > (GPS_AVAILABLE_TIMEOUT*HAL_GetTickFreq()*1000) )
					gps_availability = 0;

  }
	
}


