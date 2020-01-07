#include "wifi.h"
#include "usart.h"
#include "string.h"
#include "stdlib.h"
#include "cmsis_os.h"




const uint8_t stationID = 1;

static uint8_t wifi_availability = 0;


static char* ssid = "thisisnotanetwork";//20
static char* pass = "verysafepassword";//20

static char* server_ip = "192.168.137.1:3000";//32


static char post_buffer[512];
static char get_buffer[512];

static char wifi_TXbuffer[512];

static char wifi_RXbuffer[512]={0};
static volatile uint8_t wifi_RXbuffer_index=0;

static char wifi_user_buffer[512]={0};
static volatile uint8_t wifi_user_buffer_index=0;

static SemaphoreHandle_t xWifiOkSemaphore;
static SemaphoreHandle_t xWifiSettingsMutex;


static const char* post_str = 
"POST /data/measurement HTTP/1.1\r\n\
Host: %s\r\n\
Content-Type: application/json\r\n\
Content-Length: %d\r\n\
\r\n\
%s";		

static const char* measurement_post_str = 
"{\
	\"stationID\": %d,\r\n\
	\"time\": \"%02d:%02d:%02d\",\r\n\
	\"date\": \"%4d-%02d-%02d\",\r\n\
	\"GPSlatitude\": %.8f,\r\n\
	\"GPSlongitude\": %.8f,\r\n\
	\"NO2\": %d,\r\n\
	\"CO\": %d,\r\n\
	\"CO2\": %d,\r\n\
	\"TVOC\": %d,\r\n\
	\"Temperature\": %d,\r\n\
	\"Humidity\": %d\r\n\
}";		


static const char* get_str = 
"GET /data/station/%d HTTP/1.1\r\n\
Host: %s\r\n\
Content-Type: application/json\r\n\
\r\n";		


static inline void esp8266_enable(void);
static inline void esp8266_disable(void);
static inline void esp8266_reset(void);
static inline void esp8266_send_command(const char* cmd);
static uint8_t esp8266_send_command_ack(const char* cmd, uint32_t timeout);
static uint8_t esp8266_start_tcp(void);
static void esp8266_send_tcp(char* packet);
static void esp8266_close_tcp(void);


/**
  * @brief  turn on the ESP8266
  * @retval None
  */
inline void esp8266_enable(void)         
{
	HAL_GPIO_WritePin(WIFI_RST_GPIO_Port, WIFI_RST_Pin, GPIO_PIN_SET);
}

/**
  * @brief  turn off the ESP8266
  * @retval None
  */
inline void esp8266_disable(void)         
{
	HAL_GPIO_WritePin(WIFI_RST_GPIO_Port, WIFI_RST_Pin, GPIO_PIN_RESET);
}

/**
  * @brief  hardware reset the ESP8266
  * @retval None
  */
inline void esp8266_reset(void)         
{
	esp8266_disable();
	osDelay(200);
	esp8266_enable();
}

/**
  * @brief  send AT command to the ESP8266
	* @param	cmd command string
  * @retval None
  */
inline void esp8266_send_command(const char* cmd)         
{
		HAL_UART_Transmit(&ESP8266_UART_HANDLER, (uint8_t*)cmd, strlen(cmd), 100);
		HAL_UART_Transmit(&ESP8266_UART_HANDLER, (uint8_t*)"\r\n", 2, 100);
}

/**
  * @brief  send AT command to the ESP8266 and wait for "OK"
	* @param	cmd			command string
	* @param	timeout	timeout waiting for "OK"
  * @retval WIFI_SUCESS or WIFI_FAIL
  */
uint8_t esp8266_send_command_ack(const char* cmd, uint32_t timeout)         
{
	uint8_t cnt = 0;
	
  while(cnt++ < WIFI_MAX_ERRORS)
  {
		HAL_UART_Transmit(&ESP8266_UART_HANDLER, (uint8_t*)cmd, strlen(cmd), 100);
		xQueueReset(xWifiOkSemaphore);
		HAL_UART_Transmit(&ESP8266_UART_HANDLER, (uint8_t*)"\r\n", 2, 100);
		osDelay(200);//delay to prevent another command too fast
		
		if( xSemaphoreTake(xWifiOkSemaphore, pdMS_TO_TICKS(timeout)) == pdTRUE  )	//wait ok
			return WIFI_SUCESS;																											//ok found
  }
	
	return WIFI_FAIL;
}

/**
  * @brief  start TCP connection to the remote server
  * @retval WIFI_SUCESS or WIFI_FAIL
  */
uint8_t esp8266_start_tcp(void)         
{
	char ip[32];
	char buf[64];
	char* ptr = NULL;
	uint16_t port=80;
	
	xSemaphoreTake( xWifiSettingsMutex, pdMS_TO_TICKS(1000));						//wait for mutex
	strcpy(ip, server_ip);
	xSemaphoreGive( xWifiSettingsMutex );																//release mutex
	
	ptr=strstr(ip, ":");
	if(ptr)	//separate port from ip
	{
		port=atoi(ptr+1);
		*ptr='\0';
	}
	
	sprintf(buf, "AT+CIPSTART=\"TCP\",\"%s\",%d", ip, port);
	
	return esp8266_send_command_ack(buf,4000); 
}

/**
  * @brief  send TCP packet to the remote server
  * @retval None
  */
void esp8266_send_tcp(char* packet)         
{
	char buf[20];
	uint16_t length;
	
	length = strlen(packet);
	
	sprintf(buf, "AT+CIPSEND=%d\r\n", length);
	
	HAL_UART_Transmit(&ESP8266_UART_HANDLER, (uint8_t*)buf, strlen(buf), 500);
	osDelay(200);
	HAL_UART_Transmit(&ESP8266_UART_HANDLER, (uint8_t*)packet, length, 5000);
}

/**
  * @brief  close TCP connection
  * @retval None
  */
inline void esp8266_close_tcp(void)         
{
	esp8266_send_command("AT+CIPCLOSE"); 
}

/**
  * @brief  check if a wifi connection is available;
	* @retval availability state (1: yes, 0: no)
  */
uint8_t wifi_available(void)
{
	return wifi_availability;
}

/**
  * @brief  connect to a access point
  * @retval WIFI_SUCESS or WIFI_FAIL
  */
uint8_t wifi_establish_connection(void)
{
	char buf[64];
	
	xSemaphoreTake( xWifiSettingsMutex, pdMS_TO_TICKS(1000));	//wait for mutex
	sprintf(buf, "AT+CWJAP=\"%s\",\"%s\"", ssid, pass);
	xSemaphoreGive( xWifiSettingsMutex );

	return esp8266_send_command_ack(buf,20000); 
}

/**
  * @brief  POST HTTP packet with a measurement object
  * @retval WIFI_SUCESS or WIFI_FAIL
  */
uint8_t wifi_post_measurement(void)
{ //TODO: add measurement parameter
	sprintf(post_buffer, measurement_post_str, 
		stationID,2,3,4,2000+5,6,7,(float)8,(float)-9,10,11,12,13,14,15);
	
	xSemaphoreTake( xWifiSettingsMutex, pdMS_TO_TICKS(1000));						//wait for mutex
	sprintf(wifi_TXbuffer, post_str, 
		server_ip, strlen(post_buffer), post_buffer);											//assemble string
	xSemaphoreGive( xWifiSettingsMutex );																//release mutex

	
	if( esp8266_start_tcp() == WIFI_FAIL)
		return WIFI_FAIL;
	
	esp8266_send_tcp(wifi_TXbuffer);
	
	osDelay(3000);
	esp8266_close_tcp();
	
	return WIFI_SUCESS;
}

/**
  * @brief  GET HTTP packet with station object
  * @retval WIFI_SUCESS or WIFI_FAIL
  */
uint8_t wifi_get_station(void)
{
	uint8_t cnt = 0;
	char* ptr = NULL;
	uint32_t sampleRate, state;
	uint32_t year, month, day;
	uint32_t hour, minute, second;
	
	do{
		if(cnt++ >= WIFI_MAX_ERRORS)
			return WIFI_FAIL;
		
		memset(get_buffer, 0, 512);//clear buffer
		
		xSemaphoreTake( xWifiSettingsMutex, pdMS_TO_TICKS(1000));			//wait for mutex
		sprintf(wifi_TXbuffer, get_str, 
			stationID, server_ip);																			//assemble string
		xSemaphoreGive( xWifiSettingsMutex );													//release mutex

		
		if( esp8266_start_tcp() == WIFI_FAIL )						 //start tcp connection
			continue;
		esp8266_send_tcp(wifi_TXbuffer); //send request

		HAL_UART_AbortReceive_IT(&ESP8266_UART_HANDLER);	//stop wifi uart interrupts
		__HAL_UART_FLUSH_DRREGISTER(&ESP8266_UART_HANDLER);
		HAL_UART_Receive(&ESP8266_UART_HANDLER, (uint8_t*)get_buffer, 512, 5000);  //receive data in poling mode
		
		HAL_UART_Receive_IT(&ESP8266_UART_HANDLER, (uint8_t*)&wifi_RXbuffer[wifi_RXbuffer_index], 1);//enable wifi uart interrupts again
		printf("%s",get_buffer);//show get data to serial
		
		esp8266_close_tcp();
		
		ptr=strstr( get_buffer, "{\"station");	//look for a expected string
	}while( !ptr ); //if not found, repeat
	
  //recover results
	ptr = strstr(get_buffer, "sampleRate");
  if (ptr)
    sscanf(ptr, "sampleRate\":%d", &sampleRate);
	
  ptr = strstr (get_buffer, "date");
	if (ptr)
    sscanf(ptr, "date\":\"%d-%d-%dT%d:%d:%d%*s", &year, &month, &day, &hour, &minute, &second);
	
  ptr = strstr (get_buffer, "state");
	if (ptr)
    sscanf(ptr, "state\":%d", &state);
	
	
	//{"stationID":1,"name":"Default Station","sampleRate":1,"date":"2020-01-07T02:01:30.728Z"}
	
	printf("\r\nGET STATION: samplerate: %d  ---- %4d-%02d-%02d, %d:%d:%d ",
			sampleRate, year, month, day, hour, minute, second);
	HAL_Delay(1000);
	
	return WIFI_SUCESS;
}

/**
  * @brief  Handle user commands
  * @retval None
  */
void wifi_user_CommandHandler(void)
{

	if( !strncmp(wifi_user_buffer, "help", strlen("help")) )		//help instructions
	{
		printf("\r\n---AIRQUAM Commands---");
		
		printf("\r\n\r\nSet ssid and pass:");
		printf("\r\nAT+CWJAP=\"ssid\",\"pass\"");
		
		printf("\r\n\r\nSet server IP or URL:");
		printf("\r\nAT+CIP=\"192.168.137.1:3000\"");
		printf("\r\nAT+CIP=\"airquam.herokuapp.com\"");
		
		printf("\r\n\r\n----------------------\r\n");
	}
	else if( !strncmp(wifi_user_buffer, "AT+CWJAP=\"", strlen("AT+CWJAP=\"")) )		//set ssid and pass
	{
		
		xSemaphoreTakeFromISR( xWifiSettingsMutex, NULL);									//wait for mutex
		sscanf(wifi_user_buffer, "AT+CWJAP=\"%[^\"]\",\"%[^\"]", ssid, pass);		//update settings
		xSemaphoreGiveFromISR( xWifiSettingsMutex, NULL);									//release mutex
		
	}
	else if( !strncmp(wifi_user_buffer, "AT+CIP=\"", strlen("AT+CIP=\"")) )		//set server address
	{
		
		xSemaphoreTakeFromISR( xWifiSettingsMutex, NULL);					//wait for mutex
		sscanf(wifi_user_buffer, "AT+CIP=\"%[^\"]", server_ip);			//update settings
		xSemaphoreGiveFromISR( xWifiSettingsMutex, NULL);					//release mutex

	}

}

/**
  * @brief  wifi user uart callback routine
  * @retval None
  */
void wifi_user_CallBack(void)
{
	if(wifi_user_buffer[wifi_user_buffer_index] == '\n')		//end of line
	{
		wifi_user_buffer[wifi_user_buffer_index+1] = '\0';
		wifi_user_buffer_index = 0;
		wifi_user_CommandHandler();
	}
	else if(wifi_user_buffer[wifi_user_buffer_index] == '\r');
	else																								//new char
	{
		wifi_user_buffer_index++;
		wifi_user_buffer_index &= ~(1<<8); //keep inside the limits
	}
	//set the interrups for UART3 Rx again
	HAL_UART_Receive_IT(&huart3, (uint8_t*)&wifi_user_buffer[wifi_user_buffer_index], 1);
}

/**
  * @brief  wifi uart callback routine
  * @retval None
  */
void wifi_CallBack(void) 
{
	static uint8_t aux;
	aux = wifi_RXbuffer[wifi_RXbuffer_index];
	HAL_UART_Transmit_IT(&huart3, &aux, 1);						//echo to uart3
	
	if(wifi_RXbuffer[wifi_RXbuffer_index] == '\n')		//end of line
	{
		wifi_RXbuffer[wifi_RXbuffer_index+1] = '\0';
		wifi_RXbuffer_index = 0;
		
	}
	else if(wifi_RXbuffer[wifi_RXbuffer_index] == '\r');
	else																								//new char
	{
		wifi_RXbuffer_index++;
		wifi_RXbuffer_index &= ~(1<<8); //keep inside the limits
		
		if( wifi_RXbuffer_index == 2 )
			if( !strncmp(wifi_RXbuffer, "OK", 2) )			//signal 'OK'
				xSemaphoreGiveFromISR( xWifiOkSemaphore, NULL);
		
	}
	//set the interrups for UART Rx again
	HAL_UART_Receive_IT(&ESP8266_UART_HANDLER, (uint8_t*)&wifi_RXbuffer[wifi_RXbuffer_index], 1);
}

/**
  * @brief  init function for the wifi module
  * @retval None
  */
void wifi_init(void)                                //This function contains AT commands that passes to connect_wifi()
{

	xWifiOkSemaphore = xSemaphoreCreateBinary();
	xWifiSettingsMutex = xSemaphoreCreateMutex();
	
	esp8266_disable();
	
	wifi_availability = 0;

}

/**
  * @brief  task function for the wifi task
  * @retval None
  */
void vWifi_taskFunction(void const * argument)
{
	HAL_UART_Receive_IT(&ESP8266_UART_HANDLER, (uint8_t*)&wifi_RXbuffer[wifi_RXbuffer_index], 1);	
	__HAL_UART_FLUSH_DRREGISTER(&ESP8266_UART_HANDLER);
	HAL_UART_Receive_IT(&ESP8266_UART_HANDLER, (uint8_t*)&wifi_RXbuffer[wifi_RXbuffer_index], 1);	
	
	HAL_UART_Receive_IT(&huart3, (uint8_t*)&wifi_user_buffer[wifi_user_buffer_index], 1);
	__HAL_UART_FLUSH_DRREGISTER(&huart3);
	HAL_UART_Receive_IT(&huart3, (uint8_t*)&wifi_user_buffer[wifi_user_buffer_index], 1);

	
	while(1)	//setup, only exit if concluded correctly
	{
		esp8266_reset();
		osDelay(5000);
		
		if( esp8266_send_command_ack("AT",200) == WIFI_FAIL )                   //Sends AT command with time(Command for Achknowledgement)
			continue;
		if( esp8266_send_command_ack("AT+CWMODE=1",200) == WIFI_FAIL )          //Sends AT command with time (For setting mode of Wifi)
			continue;
		if( esp8266_send_command_ack("AT+RST", 5000) == WIFI_FAIL )             //Sends AT command with time (For RESETTING WIFI)
			continue;
		break;
	}
	
	for(;;)		//infinite loop
	{	
		esp8266_enable();
		osDelay(5000);
		
		do{
			if( wifi_establish_connection() == WIFI_FAIL )  //connect to a access point
			{	
				wifi_availability = 0;
				break;
			}
			
			wifi_availability = 1;
			osDelay(500);
			
			if( wifi_get_station() == WIFI_FAIL )  					//get latest settings from server
			{	
				wifi_availability = 0;
				break;
			}
			
			osDelay(1000);
			
			//while()measurement available
			{
				if( wifi_post_measurement() == WIFI_FAIL )  	//post a measurement
				{	
					wifi_availability = 0;
					break;
				}
			}
			
		}while(0);
		
		esp8266_disable();
		osDelay(1000*60*2);		//sleep 2 minutes
	}
	
}


