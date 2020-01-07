#ifndef _WIFI_H_
#define _WIFI_H_

#include "stdint.h"

#define ESP8266_UART_HANDLER  huart4
#define ESP8266_RST  					WIFI_RST




void wifi_establish_connection(void);
void wifi_post_measurement(void);
void wifi_get_station(void);
void wifi_user_CommandHandler(void);
void wifi_user_CallBack(void);
void wifi_CallBack(void);
void wifi_init(void);
void vWifi_taskFunction(void const * argument);




#endif /*_WIFI_H_*/
