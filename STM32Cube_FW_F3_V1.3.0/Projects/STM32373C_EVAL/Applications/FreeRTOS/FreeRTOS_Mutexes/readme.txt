/**
  @page FreeRTOS mutexes application
 
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    FreeRTOS/FreeRTOS_Mutexes/readme.txt
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    19-June-2015
  * @brief   Description of the STM32F3xx CMSIS RTOS mutexes example.
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

This application shows how to use mutexes with CMSIS RTOS API.

This example creates three threads, with different priorities, that access the 
same mutex, as described below:

MutexHighPriorityThread() has the highest priority so executes 
first and grabs the mutex and sleeps for a short period to let the lower 
priority threads execute.  When it has completed its demo functionality
it gives the mutex back before suspending itself. 
 
MutexMeduimPriorityThread() attempts to access the mutex by performing
a blocking 'wait'.  This thread blocks when the mutex is already taken 
by the high priority thread. It does not unblock until the highest 
priority thread  has released the mutex, and it does not actually run until 
the highest priority thread has suspended itself.
When it eventually does obtain the mutex all it does is give the mutex back
prior to also suspending itself.  At this point both the high and medium
priority threads are suspended.

MutexLowPriorityThread() runs at the idle priority.  It spins round
a tight loop attempting to obtain the mutex with a non-blocking call.  As
the lowest priority thread it will not successfully obtain the mutex until
both high and medium priority threads are suspended.  Once it eventually 
does obtain the mutex it first resume both suspended threads prior to giving
the mutex back - resulting in the low priority thread temporarily inheriting
the highest thread priority.       


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents

    - FreeRTOS/FreeRTOS_Mutexes/Inc/main.h                main config file
    - FreeRTOS/FreeRTOS_Mutexes/Inc/stm32f3xx_hal_conf.h  HAL Library Configuration file
    - FreeRTOS/FreeRTOS_Mutexes/Inc/stm32f3xx_it.h        Header for stm32f3xx_it.c
    - FreeRTOS/FreeRTOS_Mutexes/Inc/FreeRTOSConfig.h      FreeRTOS Configuration file
    - FreeRTOS/FreeRTOS_Mutexes/Src/main.c                main program file
    - FreeRTOS/FreeRTOS_Mutexes/Src/stm32f3xx_it.c        STM32 Interrupt handlers

@par Hardware and Software environment

  - This example runs on STM32F373xx devices.
    
  - This example has been tested with STM32373C-EVAL board and can be
    easily tailored to any other supported device and development board. 

@par How to use it ?

In order to make the program work, you must do the following:
  1. Load the demonstration code in the Flash memory (see below)
  2. Add the following variables to LiveWatch (HighPriorityThreadCycles, 
     MediumPriorityThreadCycles, LowPriorityThreadCycles) 
     All three variables must remain equals all the time
     LED1, LED2 and LED4 should toggle
     LED3 will turn on in case of error
     
  In order to load the demonstration code, you have do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
