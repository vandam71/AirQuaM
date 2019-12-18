#include "GPSConfig.h"
#include "GPS.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


gps_t gps;

uint8_t buff_arr[2][512]={0};					//declare two buffers
volatile uint8_t buff_index=0;				//
volatile uint8_t buff_receiving = 0;	//which buffer is receiveing
volatile uint8_t received_data=0;				//data available

//SemaphoreHandle_t xGpsSemaphore = NULL;

/**
  * @brief  get method for the gps module
  * @retval gps_t object
  */
gps_t gps_read(void)
{
	return gps;
}

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
  * @brief  init function for gps molude
  * @retval None
  */
void	gps_init(void)
{
	buff_index=0;
	buff_receiving=0;
	received_data=0;
	HAL_UART_Receive_IT(&GPS_UART_HANDLER, &buff_arr[buff_receiving][buff_index], 1);
	
	//xSemaphore = xSemaphoreCreateMutex();
	
}

/**
  * @brief  uart callback routine
  * @retval None
  */
void	gps_CallBack(void)
{
		if(buff_arr[buff_receiving][buff_index] == '\n')
		{
			buff_arr[buff_receiving][buff_index+1] = '\0';
			buff_receiving = !buff_receiving;
			received_data = 1;			//give semaphore here
			//xSemaphoreGiveFromISR(xGpsSemaphore);
			buff_index = 0;
		}
		else if(buff_arr[buff_receiving][buff_index] == '\r');
		else
		{
			buff_index++;
			buff_index &= ~(1<<7); //keep inside the limits
		}
		//set the interrups for UART3 Rx again
		HAL_UART_Receive_IT(&GPS_UART_HANDLER, &buff_arr[buff_receiving][buff_index], 1);
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
  * @brief  task function for the gps task
  * @retval None
  */
void	gps_taskFunction(void)
{
	//if( xSemaphoreTake( xSemaphore, ( TickType_t ) 10 ) == pdTRUE )
	//xSemaphoreGive( xSemaphore );
	
	gps_t my_gps;
	while (1)
  {
		if(received_data)	// semaphore take
		{
			received_data=0;

			if ( gps_process(&my_gps, &buff_arr[!buff_receiving][0]) == GPS_SUCESS)
			{
				gps = my_gps;
				printf("----------------------------------------------\r\n");
				printf("Latitude: %f \r\n",gps.latitude);
				printf("Longitude: %f \r\n", gps.longitude);
			}
		}			
  }
}


