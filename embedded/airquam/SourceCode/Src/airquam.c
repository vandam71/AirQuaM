#include "airquam.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "string.h" 
#include "math.h" 
#include "wifi.h" 
#include "ssd1306.h"

static volatile airquam_t*			airquam;

static SemaphoreHandle_t xAirquamSettingsMutex;

extern TaskHandle_t taskAirquamHandle;	//handler for the airquam task



/**
  * @brief  manage the sampling to be sent to the remote server
  * @retval None
  */
static void airquam_manage_sampling(void)
{
	static uint32_t last_tick;
	uint32_t current_tick;
	
	current_tick =  HAL_GetTick();

	xSemaphoreTake( xAirquamSettingsMutex, pdMS_TO_TICKS(5000));	//wait for mutex
	if( airquam->sampling_state && 
			airquam->sampling_period*1000 <= current_tick-last_tick )
	{
		last_tick = current_tick;
		
		if( measurement_bkp_buffer_isNotFull() )
			measurement_bkp_buffer_push(airquam->current_meas);
	}
	xSemaphoreGive( xAirquamSettingsMutex );											//release mutex
}

/**
  * @brief  routine to draw a splash screen while the sensors, drivers and modules are being initialized
  * @retval None
  */
static void draw_splash(void)
{
	ssd1306_Fill(Black);
	ssd1306_SetCursor(8, 18);
	ssd1306_WriteString("AirQuaM", Font_16x26, White);
  ssd1306_UpdateScreen();
}

/**
  * @brief  routine to draw the display
	*	@param	aqm airqham_t object to be translated into display info
  * @retval None
  */
static void draw_display(airquam_t aqm)
{
	static uint8_t page=0;
	static uint32_t last_page_tick=0;
	char str[256];

	if( (HAL_GetTick()-last_page_tick) > (DISPLAY_PAGE_TIME*HAL_GetTickFreq()*1000) ){
		page=!page;		//switch page (only 2 pages here)
		last_page_tick = HAL_GetTick();
	}

	ssd1306_Fill(Black);		//clear display buffer
	
	if ( aqm.gps_state ){//valid gps
		ssd1306_SetCursor(94, 0);
		ssd1306_drawIcon(Icon_satellite, White);	//draw gps icon
	}
	if ( aqm.wifi_state ){//valid gps
		ssd1306_SetCursor(112, 0);
		ssd1306_drawIcon(Icon_wlan, White);				//draw wifi icon
	}
	
	if(page == 0)
	{
		sprintf(str,"    %2d:%02d:%02d", aqm.current_meas.time.Hours, aqm.current_meas.time.Minutes, aqm.current_meas.time.Seconds);
		ssd1306_SetCursor(4, 2);	ssd1306_WriteString(str, Font_7x10, White);		//time

		sprintf(str,"  NO2: %.2f ppm", (double)(aqm.current_meas.gas.NO2/1000.0));
		ssd1306_SetCursor(0, 24); ssd1306_WriteString(str, Font_7x10, White);		//gas concentration
		sprintf(str,"   CO: %d ppm", aqm.current_meas.gas.CO/1000);
		ssd1306_SetCursor(0, 34); ssd1306_WriteString(str, Font_7x10, White);
		sprintf(str,"  CO2: %d ppm", aqm.current_meas.gas.CO2/1000);
		ssd1306_SetCursor(0, 44); ssd1306_WriteString(str, Font_7x10, White);
		sprintf(str," TVOC: %d ppb", aqm.current_meas.gas.TVOC);
		ssd1306_SetCursor(0, 54); ssd1306_WriteString(str, Font_7x10, White);
	}
	else
	{
		sprintf(str,"    %2d-%02d-%4d", aqm.current_meas.date.Date, aqm.current_meas.date.Month, 2000+aqm.current_meas.date.Year);
		ssd1306_SetCursor(0, 2);	ssd1306_WriteString(str, Font_7x10, White);		//date

		sprintf(str,"T:%.1f'C   RH:%3.0f%%", aqm.current_meas.environment.T, aqm.current_meas.environment.RH);
		ssd1306_SetCursor(0, 24); ssd1306_WriteString(str, Font_7x10, White);										//Temperature and humidity
		sprintf(str,"    Ts: %d s", aqm.sampling_period);
		ssd1306_SetCursor(0, 34); ssd1306_WriteString(str, Font_7x10, White);										//Sampling period
		
		if ( aqm.gps_state )//valid gps
		{
			sprintf(str," %c  %3.8f", aqm.current_meas.gps.latitude<0?'S':'N' ,fabs(aqm.current_meas.gps.latitude));
			ssd1306_SetCursor(0, 44); ssd1306_WriteString(str, Font_7x10, White);										//latitude
			sprintf(str," %c  %3.8f", aqm.current_meas.gps.longitude<0?'W':'E' ,fabs(aqm.current_meas.gps.longitude));
			ssd1306_SetCursor(0, 54); ssd1306_WriteString(str, Font_7x10, White);										//longitude
		}
		else
		{
			ssd1306_SetCursor(0, 54); ssd1306_WriteString("  Waiting GPS...", Font_7x10, White);
		}
	}
	
	if(aqm.sampling_state == 0)		//draw on/off
	{	//off
		ssd1306_SetCursor(0, 2);
		ssd1306_WriteString("OFF", Font_7x10, White);	
	}
	else	
	{	//on
		for(int i=0; i<20; i++)
			for(int j=0; j<11; j++)
				ssd1306_DrawPixel(i, j, White);		//highlight 'on' state
		ssd1306_SetCursor(4, 2);
		ssd1306_WriteString("ON", Font_7x10, Black);
	}

	for(int i=0; i<=20; i++)	{						//on/off borders
		ssd1306_DrawPixel(i, 0, White);
		ssd1306_DrawPixel(i, 11, White);
	}
	for(int i=0; i<=11; i++)	{
		ssd1306_DrawPixel(20, i, White);
		ssd1306_DrawPixel(0, i, White);
	}
	
  ssd1306_UpdateScreen();
	return;
}


/**
  * @brief  set method for sampling_period member
	* @param	period	new sampling period in seconds
  * @retval None
  */
void airquam_set_samplig_period(uint32_t period)
{
	airquam->sampling_period = period;
}

/**
  * @brief  set method for sampling_state member
	* @param	period	new sampling state
  * @retval None
  */
void airquam_set_samplig_state(uint8_t state)
{
	airquam->sampling_state = state;
}

/**
  * @brief  init function for airquam molude
  * @retval None
  */
void airquam_init(void)
{
	//Enable the PWR clock
	//__HAL_RCC_PWR_CLK_ENABLE();nao
	//Enable access to the backup domain
	//HAL_PWR_EnableBkUpAccess();
	//Enable backup SRAM Clock
	__HAL_RCC_BKPSRAM_CLK_ENABLE();//sim
	//Enable the Backup SRAM low power Reg
	//HAL_PWREx_EnableBkUpReg();nao
	
	
	airquam = (airquam_t*)(BKP_AIRQUAM_BASE);
	
	airquam->sampling_period = 60;
	airquam->sampling_state = 0;
	airquam->gps_state = 0;
	airquam->wifi_state = 0;
	
	xAirquamSettingsMutex = xSemaphoreCreateMutex();
	
	gps_init();
	ssd1306_Init();
	draw_splash();
	wifi_init();
	measurement_init();
	
}

/**
  * @brief  timer callback routine
  * @retval None
  */
void airquam_CallBack(void)
{
	//BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	// notification give (binnary semaphore like)
	vTaskNotifyGiveFromISR( taskAirquamHandle, NULL );
}

/**
  * @brief  task function for the airquam task
	* @param  argument: Not used 
  * @retval None
  */
void vAirquam_taskFunction(void const * argument)
{
	//uint32_t start, end;
	
	HAL_TIM_Base_Start_IT(&PERIODIC_TIMER_HANDLER);
  /* Infinite loop */
  for(;;)
  {
		// notification take (binnary semaphore like)
		ulTaskNotifyTake( pdTRUE, portMAX_DELAY  );  
		
		//printf("start:%d   ", start=HAL_GetTick());
		airquam->current_meas = measure(); 
		
		airquam->gps_state=0;
		if( gps_valid(airquam->current_meas.gps) )
		{
			airquam->gps_state=1;
			airquam_manage_sampling();
		}
		
		airquam->wifi_state = wifi_available();
		
		draw_display(*airquam);
		//end=HAL_GetTick();
		//printf("end:%d     time:%d\r\n", end, end-start);
    osDelay(1);
  }
}
