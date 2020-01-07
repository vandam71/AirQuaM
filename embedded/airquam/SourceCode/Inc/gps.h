#ifndef _GPS_H_
#define _GPS_H_

#include "stdint.h"

#define GPS_UART_HANDLER  huart2

#define GPS_AVAILABLE_TIMEOUT  60 //in seconds

enum gps_return 
{
	GPS_FAIL = 0,
	GPS_SUCESS
};

typedef struct
{
	//uint8_t			UTC_Hour;
	//uint8_t			UTC_Min;
	//uint8_t			UTC_Sec;
	//uint16_t		UTC_MicroSec;
	
	float				latitude;
	//double			latitudeDecimal;
	//char				NS_Indicator;
	float				longitude;
	//double			longitudeDecimal;
	//char				EW_Indicator;
	
	//uint8_t			PositionFixIndicator;
	//uint8_t			SatellitesUsed;
	//float				HDOP;
	//float				MSL_Altitude;
	//char				MSL_Units;
	//float				Geoid_Separation;
	//char				Geoid_Units;
	//
	//uint16_t		AgeofDiffCorr;
	//char				DiffRefStationID[4];
	//char				CheckSum[2];	
	
}gps_t;


void		gps_init(void);
void		gps_CallBack(void);
uint8_t gps_available(void);
gps_t 	gps_read(void);
void		vGps_taskFunction(void const * argument);


#endif
