/**
  @page FLASH_EraseProgram FLASH Erase and Program
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    FLASH/FLASH_EraseProgram/readme.txt
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    19-June-2015
  * @brief   Description of the FLASH Erase and Program example.
  ******************************************************************************
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  @endverbatim

@par Example Description 

This example describes how to configure and use the FLASH HAL API to erase and program 
the internal FLASH memory.
  
After Reset, the Flash memory Program/Erase Controller is locked. To unlock it,
the FLASH_Unlock function is used.
Before programming the desired addresses, an erase operation is performed using 
the flash erase page feature.  A programmation of the erase procedure is done 
in filling the erase init structure (erase will be done by pages from the 1st 
page to the calculation of pages number) 
Then all these pages will be erased one by one by calling HAL_FLASHEx_Erase function. 

@note: if problem occurs on a sector, erase will be stopped and faulty page will 
be returned to user (through variable 'PageError').

Once this operation is finished, the word programming operation will be performed by 
using the HAL_FLASH_Program function. The written data is then checked and the
result of the programming operation is stored into the MemoryProgramStatus variable.
  
STM32 Eval board's LEDs can be used to monitor the transfer status:
 - LED3 is ON when there are no errors detected after programmation
 - LED4 is ON when there are errors dectected after programmation
 - LED5 is ON when there is an issue during erase or program procedure

@par Directory contents 

  - FLASH/FLASH_EraseProgram/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - FLASH/FLASH_EraseProgram/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - FLASH/FLASH_EraseProgram/Inc/main.h                  Header for main.c module  
  - FLASH/FLASH_EraseProgram/Src/stm32f3xx_it.c          Interrupt handlers
  - FLASH/FLASH_EraseProgram/Src/main.c                  Main program
  - FLASH/FLASH_EraseProgram/Src/system_stm32f3xx.c      STM32F3xx system clock configuration file  
  
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Hardware and Software environment

  - This example runs on STM32F303xC devices.
    
  - This example has been tested with STM32F3-DK Rev x Discovery board and can be
    easily tailored to any other supported device and development board.      


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open the Projects\EWARM\Project.eww workspace
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Download and Debug
 - Run program: Debug->Go(F5) 


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
