#ifndef _WIFI_H_
#define _WIFI_H_

#include "stdint.h"
#include "gpio.h"
#include "measurement.h"

#define ESP8266_RST  						WIFI_RST

#define ESP8266_UART_HANDLER  	huart4
#define WIFI_USER_UART_HANDLER  huart3

#define WIFI_MAX_ERRORS  		5

 
enum wifi_return 
{
	WIFI_FAIL=0, 
	WIFI_SUCESS
};


uint8_t wifi_establish_connection(void);
uint8_t wifi_post_measurement(measurement_t meas);
uint8_t wifi_get_station(void);
uint8_t wifi_available(void);
void wifi_user_CommandHandler(void);
void wifi_user_CallBack(void);
extern void wifi_CallBack(void);
void wifi_init(void);
void vWifi_taskFunction(void const * argument);




#endif /*_WIFI_H_*/
