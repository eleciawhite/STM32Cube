/**
  @page TIM_PWMOutput TIM PWM Output example
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    TIM/TIM_PWMOutput/readme.txt 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    19-June-2015
  * @brief   Description of the PWM signals generation using Timer2 Example
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

This example shows how to configure the TIM peripheral in PWM (Pulse Width Modulation) 
mode.

The TIM2CLK frequency is set to SystemCoreClock (Hz), to get TIM2 counter
clock at 16 MHz the Prescaler is computed as following:
   - Prescaler = (TIM2CLK / TIM2 counter clock) - 1

SystemCoreClock is set to 64 MHz for STM32F302R8 Nucleo board.

The TIM2 generates 4 PWM signals at 36 KHz:

    TIM2_Output Frequency = TIM2 counter clock/(ARR + 1)
                          = 16 MHz / 444
                          = 36 KHz
                        
The TIM2 CCR1 register value is equal to 222, so the TIM2 Channel 1 generates a 
PWM signal with a frequency equal to 36 KHz and a duty cycle equal to 50%:
TIM2 Channel1 duty cycle = (TIM2_CCR1/(TIM2_ARR + 1)) * 100 = 50%

The TIM2 CCR2 register value is equal to 166, so the TIM2 Channel 2 generates a 
PWM signal with a frequency equal to 36 KHz and a duty cycle equal to 37.5%:
TIM2 Channel2 duty cycle = (TIM2_CCR2/(TIM2_ARR + 1)) * 100 = 37.5%

The TIM2 CCR3 register value is equal to 111, so the TIM2 Channel 3 generates a 
PWM signal with a frequency equal to 36 KHz and a duty cycle equal to 25%:
TIM2 Channel3 duty cycle = (TIM2_CCR3/(TIM2_ARR + 1)) * 100 = 25%

The TIM2 CCR4 register value is equal to 55, so the TIM2 Channel 4 generates a 
PWM signal with a frequency equal to 36 KHz and a duty cycle equal to 12.5%:
TIM2 Channel4 duty cycle = (TIM2_CCR4/(TIM2_ARR + 1)) * 100 = 12.5%


The PWM waveforms can be displayed using an oscilloscope.

STM32 Nucleo board's LED2 can be used to monitor the error: when Error_Handler is called, the LED2 toggles 
with a frequency of 1Hz. 
 
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 

  - TIM/TIM_PWMOutput/Src/main.c                 Main program
  - TIM/TIM_PWMOutput/Src/system_stm32f3xx.c     STM32F3xx system clock configuration file
  - TIM/TIM_PWMOutput/Src/stm32f3xx_it.c         Interrupt handlers 
  - TIM/TIM_PWMOutput/Src/stm32f3xx_hal_msp.c    HAL MSP module
  - TIM/TIM_PWMOutput/Inc/main.h                 Main program header file  
  - TIM/TIM_PWMOutput/Inc/stm32f3xx_hal_conf.h   HAL Configuration file
  - TIM/TIM_PWMOutput/Inc/stm32f3xx_it.h         Interrupt handlers header file

        
@par Hardware and Software environment  

  - This example runs on STM32F302R8 device.
    
  - This example has been tested with STMicroelectronics STM32F302R8-Nucleo RevC
    boards and can be easily tailored to any other supported device 
    and development board.

  - STM32F302R8-Nucleo RevC Set-up
    - Connect the following pins to an oscilloscope to monitor the different 
      waveforms:
        - PA.15: (TIM2_CH1)
        - PB.03: (TIM2_CH2)
        - PB.10: (TIM2_CH3)
        - PA.10: (TIM2_CH4)  

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
