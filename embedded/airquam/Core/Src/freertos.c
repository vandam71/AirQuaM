/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "gps.h"
#include "gas.h"
#include "environment.h"
#include "airquam.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId iddleTaskHandle;
osThreadId taskWifiHandle;
osThreadId taskGpsHandle;
osThreadId taskGasHandle;
osThreadId taskEnvironmentHandle;
osThreadId taskAirquamHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void wifi_taskFunction(void const * argument);
extern void gps_taskFunction(void const * argument);
extern void gas_taskFunction(void const * argument);
extern void environment_taskFunction(void const * argument);
extern void airquam_taskFunction(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of iddleTask */
  osThreadDef(iddleTask, StartDefaultTask, osPriorityIdle, 0, 128);
  iddleTaskHandle = osThreadCreate(osThread(iddleTask), NULL);

  /* definition and creation of taskWifi */
  osThreadDef(taskWifi, wifi_taskFunction, osPriorityIdle, 0, 128);
  taskWifiHandle = osThreadCreate(osThread(taskWifi), NULL);

  /* definition and creation of taskGps */
  osThreadDef(taskGps, gps_taskFunction, osPriorityNormal, 0, 128);
  taskGpsHandle = osThreadCreate(osThread(taskGps), NULL);

  /* definition and creation of taskGas */
  osThreadDef(taskGas, gas_taskFunction, osPriorityNormal, 0, 128);
  taskGasHandle = osThreadCreate(osThread(taskGas), NULL);

  /* definition and creation of taskEnvironment */
  osThreadDef(taskEnvironment, environment_taskFunction, osPriorityNormal, 0, 128);
  taskEnvironmentHandle = osThreadCreate(osThread(taskEnvironment), NULL);

  /* definition and creation of taskAirquam */
  osThreadDef(taskAirquam, airquam_taskFunction, osPriorityNormal, 0, 128);
  taskAirquamHandle = osThreadCreate(osThread(taskAirquam), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
    
    
    
    
    
    
    

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_wifi_taskFunction */
/**
* @brief Function implementing the taskWifi thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_wifi_taskFunction */
void wifi_taskFunction(void const * argument)
{
  /* USER CODE BEGIN wifi_taskFunction */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END wifi_taskFunction */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
