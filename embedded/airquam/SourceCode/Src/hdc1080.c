#include "hdc1080.h"
#include "stm32f7xx_hal.h"
#include "i2c.h"
	
	/**
  * @brief  initialization process for the hdc1080 sensor
  * @retval None
  */
void hdc1080_init(void)
{
	uint16_t	configuration = 0;
	uint8_t		config_arr[2];
	
	configuration = ( RST | MODE | TRES(TRES_14BIT) | HRES(HRES_14BIT) );
	
	config_arr[0] = (configuration & 0xFF00)>>8;	//configuration MSB
	config_arr[1] = (configuration & 0x00FF);			//configuration LSB
	
	HAL_I2C_Mem_Write(&HDC1080_I2C_HANDLER, HDC1080_I2C_ADDR<<1, CONFIGURATION, I2C_MEMADD_SIZE_8BIT, config_arr, 2, 100);
}

/**
  * @brief  read sensor i2c temperature register
  * @retval temperature in celcius
  */
float mics4514_measureT(void)
{
	float temperature, humidity;
	hdc1080_measure_T_RH(&temperature, &humidity);
	return temperature;
}

/**
  * @brief  read sensor i2c humidity register
  * @retval relative humidity in percentage
  */
float mics4514_measureRH(void)
{
	float temperature, humidity;
	hdc1080_measure_T_RH(&temperature, &humidity);
	return humidity;
}

/**
  * @brief  read sensor i2c temperature and humidity registers
  * @retval None
  */
inline void mics4514_measure_T_RH(float *temperature, float *humidity)
{
	uint8_t receive_data[4];
	uint32_t t, rh;
	uint8_t send_data = 1;//temperature and humidity
	
	//trigger reading
	HAL_I2C_Master_Transmit(&HDC1080_I2C_HANDLER, HDC1080_I2C_ADDR<<1, &send_data, 1, 1000);
	
	//conversion takes 6.50ms(temperature) + 6.35ms(humidity)
	HAL_Delay(15);

	//Read temperature and humidity
	while(HAL_I2C_Master_Receive(&HDC1080_I2C_HANDLER, HDC1080_I2C_ADDR<<1, receive_data, 4, 2000)== HAL_ERROR)
	{
		HAL_Delay(5);
	}
		
	t =  ( (receive_data[0]<<8) | receive_data[1] );
	rh = ( (receive_data[2]<<8) | receive_data[3] );
	
	*temperature = (float)( ((t/65536.0)*165.0)-40.0 );
	*humidity =    (float)( (rh/65536.0)*100.0 );
}
