#ifndef _HDC1080_H_
#define _HDC1080_H_

#ifdef __cplusplus
 extern "C" {
#endif
	#include "stm32f7xx_hal.h"
	#include "i2c.h"
	
	#define HDC1080_I2C_ADDR			0x40
	#define HDC1080_I2C_HANDLER		hi2c2
	
	
	//I2C REGISTERS
	#define TEMPERATURE				0x00
	#define	HUMIDITY					0x01
	#define	CONFIGURATION			0x02
	#define	SERIAL_ID					0xFB
	#define	MANUFACTURER_ID		0xFE
	#define	DEVICE_ID					0xFF
	
	//CONFIGURATION Register
	#define	RST					0x0F			//self clearing reset
	#define	HEAT				0x0D			//heater to 
	#define	MODE				0x0C
	#define	BTST				0x0B
	#define	TRES(X)			(( X&0x3 )<<10)
	#define	TRES_14BIT	0x0
	#define	TRES_11BIT	0x1
	#define	HRES(X)			(( X&0x3 )<<8)
	#define	HRES_14BIT	0x0
	#define	HRES_11BIT	0x1
	#define	HRES_8BIT		0x2
	
	
	void	hdc1080_init(void);
	float hdc1080_measureT(void);
	float hdc1080_measureRH(void);
	void 	hdc1080_measure_T_RH(float*, float*);

#ifdef __cplusplus
}
#endif
	
#endif /* _HDC1080_H_ */
