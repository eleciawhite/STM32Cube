/**
  @page COMP_OutputBlanking COMP output blanking example
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    COMP/COMP_OutputBlanking/readme.txt 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    19-June-2015
  * @brief   Description of the COMP output blanking Example
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

This example shows how to use the output blanking feature of COMP peripheral.
The purpose of the output blanking feature in motor control is to prevent the
current regulation to trip upon short current spikes at the beginning of the PWM
period.
In this example COMP1 and TIM1 are used:
COMP1 is configured as following:
   - Non Inverting input is connected to PA1
   - Inverting input is connected to VREFINT (1.22V)
   - Output is available on PA0
   - Output is redirected to TIM1 BKIN to generate break event when
     non-inverting input is at higher voltage than inverting input (VREFINT)
   - TIM1 OC5 is used as blanking source

TIM1 is configured as following:
   - TIM1 period is APB2 clock / period = 72000000 / 50000 = 1440 Hz
   - TIM1 CH2 (PA9) configured in PWM mode with a frequency equal to 1440 Hz
     and duty cycle pulse/period = 100 * (37500 / 50000) =  75%
   - TIM1 OC5 (internal channel: not available on GPIO) configured in PWM mode
     with a frequency equal to 1440 Hz and high level equal to pulse / period = 
     2000 / 72000000 = 27.7 micro second
   - Break event is generated at high polarity (when non-inverting input is at
     higher voltage than inverting input)

At the beginning of TIM1 CH2 PWM, COMP1 output is blanked while TIM1 OC5 is at
high level (27.7 micro second): if non-inverting input is at higher voltage
than inverting input (VREFINT), the break event is not generated while TIM1 OC5
is at high level.
When OC5 is at low level, COMP1 output is no more blanked and if non-inverting
input is at higher voltage than inverting input (VREFINT) a break event is
generated and TIM1 CH2 (PA9) is held in safe state (low level).


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - COMP/COMP_OutputBlanking/Inc/stm32f3xx_hal_conf.h   HAL configuration file
  - COMP/COMP_OutputBlanking/Inc/stm32f3xx_it.h         COMP interrupt handlers header file
  - COMP/COMP_OutputBlanking/Inc/main.h                 Header for main.c module
  - COMP/COMP_OutputBlanking/Src/stm32f3xx_it.c         COMP interrupt handlers
  - COMP/COMP_OutputBlanking/Src/main.c                 Main program
  - COMP/COMP_OutputBlanking/Src/system_stm32f3xx.c     STM32F3xx system source file
  - COMP/COMP_OutputBlanking/Src/stm32f3xx_hal_msp.c    HAL MSP file


@par Hardware and Software environment

  - This example runs on STM32F3xxx devices.
    
  - This example has been tested with STM32303C-EVAL RevB evaluation board and can be
    easily tailored to any other supported device and development board.    

  - STM32303C-EVAL Set-up
    - Connect TIM1 CH2 (PA9) to a scope and to the input trigger of a wave generator
    - Connect the wave generator ouput to COMP1 Non Inverting input (PA1)
    - Configure the wave generator to output a pulse at rising edge event with 
      high level greater than VREFINT (1.22V) and pulse duration lower 27.7 micro second
      (as an example you can use 10 micro second). 
      You can change the pulse delay and note that the break event is generated
      only when the pulse is triggered after 27.7 micro second


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
