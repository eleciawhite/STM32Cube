/**
  @page FreeRTOS message low power example
 
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    FreeRTOS/FreeRTOS_LowPower/readme.txt
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    19-June-2015
  * @brief   Description of the STM32F3xx CMSIS RTOS low power example.
  ******************************************************************************
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  @endverbatim

@par Description

This application shows how to enter and exit low power mode with CMSIS RTOS API.

This example creates two threads.

   + A Rx thread that blocks on a queue to wait for data, blipping an LED each 
     time data is received (turning it on and then off again) before returning 
     to block on the queue once more.

   + A Tx thread that repeatedly enters the Blocked state for 500ms.  
     On exiting the blocked state the Tx thread sends a value through the queue 
     to the Rx thread (causing the Rx thread to exit the blocked state and blip 
     the LED).

Blocking for a finite period allows the kernel to stop the tick interrupt
and place the STM32 into sleep mode.

In this example, not used GPIO's are configured to analog, this help to reduce 
the power consumption of the device

Observed behaviour:

Every 500ms the MCU will come out of the low power state to turn the LED on,
then return to the low power state for 20ms before leaving the low power
state again to turn the LED off.  This will be observed as a fast blipping
on the LED

The RTOS tick is suppressed while the MCU is in its low power state.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents
    - FreeRTOS/FreeRTOS_LowPower/Inc/main.h                main config file
    - FreeRTOS/FreeRTOS_LowPower/Inc/stm32f3xx_hal_conf.h  HAL Library Configuration file
    - FreeRTOS/FreeRTOS_LowPower/Inc/stm32f3xx_it.h        Header for stm32f3xx_it.c
    - FreeRTOS/FreeRTOS_LowPower/Inc/FreeRTOSConfig.h      FreeRTOS Configuration file
    - FreeRTOS/FreeRTOS_LowPower/Src/main.c                main program file
    - FreeRTOS/FreeRTOS_LowPower/Src/stm32f3xx_it.c        STM32 Interrupt handlers

@par Hardware and Software environment

  - This example runs on STM32F303xC devices.
    
  - This example has been tested with STM32303C-EVAL RevC board and can be
    easily tailored to any other supported device and development board. 

@par How to use it ?

In order to make the program work, you must do the following:
  1. Load the demonstration code in the Flash memory (see below)

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
