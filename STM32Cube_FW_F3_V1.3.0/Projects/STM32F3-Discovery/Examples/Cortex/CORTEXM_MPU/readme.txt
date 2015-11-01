    /**
  @page CortexM_MPU CortexM MPU example

  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    CORTEXM/CORTEXM_MPU/readme.txt 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    19-June-2015
  * @brief   Description of the CortexM MPU example.
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

This example presents the MPU feature. The example purpose is to configure a 
memory region as privileged read only region and tries to perform read and write 
operation in different mode.

If the access is permitted LED3 is toggling. If the access is not permitted, 
a memory management fault is generated and LED4 is ON.
To generate an MPU memory fault exception due to an access right error, uncomment
the following line "PrivilegedReadOnlyArray[0] = 'e';" in the "stm32_mpu.c" file.
 

@par Directory contents 

  - CortexM/MPU/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - CortexM/MPU/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - CortexM/MPU/Inc/main.h                     Header for main.c module  
  - CortexM/MPU/Src/stm32f3xx_it.c          Interrupt handlers
  - CortexM/MPU/Src/main.c                     Main program
  - CortexM/MPU/Src/system_stm32f3xx.c      stm32f3xx system source file

@par Hardware and Software environment 

  - This example runs on STM32F303xC device.

  - This example has been tested with STM32F3_DISCOVERY board and can be
    easily tailored to any other supported device and development board.    


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Add the required example files
   - stm32_mpu.c
<ul>
- For RIDE and TrueSTUDIO toolchains you have to follow these instructions

    <li> RIDE 
        - In the Application options -> script menu, set "Use Default Script File" 
          to "No" and use "stm32F_flash_ROAarray.ld" as Script File. 
          This linker is configured for STM32F3xx. It should be updated to use it
          with other STM32 devices.      
        

    <li> TrueSTUDIO 
          - In the project properties window, select C/C++ Build->settings node then 
           the C Linker->General node and use "stm32F_flash_ROAarray.ld" as Script File.
          This linker is configured for STM32F3xx. It should be updated to use it
          with other STM32 devices.         
</ul>  
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
