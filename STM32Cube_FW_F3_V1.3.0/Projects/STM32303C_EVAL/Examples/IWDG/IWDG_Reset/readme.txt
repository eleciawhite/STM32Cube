/**
  @page IWDG_Example Independent Watchdog Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    IWDG/IWDG_Example/readme.txt 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    19-June-2015
  * @brief   Description of the Independent Watchdog Example.
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

This example describes how to ensure IWDG reload counter and to simulate a software 
fault generating an MCU IWDG reset on expiry of a programmed time period.

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 72 MHz.

When "LSI_TIM_MEASURE" define is enabled in main.h, TIM16 is configured to measure
the LSI frequency as the LSI is internally connected to TIM16 CH1, in order to adjust
the IWDG prescaler to have accurate timeout.

First, the TIM16 timer is configured to measure the LSI frequency as the 
LSI is internally connected to TIM16 CH1, in order to adjust the IWDG clock.

The LSI measurement using the TIM16 is described below:
  - Configure the TIM16 to remap internally the TIM16 CH1 Input Capture to the LSI
    clock output.
  - Enable the TIM16 Input Capture interrupt: after one cycle of LSI clock, the
    period value is stored in a variable and compared to the HCLK clock to get
    its real value. 

When "LSI_TIM_MEASURE" define is disabled, the LSI frequency is set to 42 KHz,
which will generate less accurate IWDG timeout.

Then, the IWDG reload counter is configured as below to obtain 250 ms according 
to the measured LSI frequency after setting the prescaler value:
  
    IWDG counter clock Frequency = LSI Frequency / Prescaler value

The IWDG reload counter is refreshed each 240 ms in the main program infinite 
loop to prevent a IWDG reset.
  
LED2 is also toggled each 240 ms indicating that the program is running.

An EXTI Line is connected to a GPIO pin, and configured to generate an interrupt
on the rising edge of the signal.

The EXTI Line is used to simulate a software failure: once the EXTI Line event 
occurs, by pressing the Key/Tamper push-button(PE.6), the corresponding interrupt  
is served. 

In the ISR, a write to invalid address generates a Hardfault exception 
containing an infinite loop and preventing to return to main program (the IWDG 
reload counter is not refreshed).
As a result, when the IWDG counter reaches 00h, the IWDG reset occurs.
  
If the IWDG reset is generated, after the system resumes from reset, LED1 turns ON.
If the EXTI Line event does not occur, the IWDG counter is indefinitely refreshed
in the main program infinite loop, and there is no IWDG reset.

LED3 will turn ON, if any error is occurred.

@par Directory contents 
 
  - IWDG/IWDG_Reset/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - IWDG/IWDG_Reset/Inc/main.h                  Header for main.c module
  - IWDG/IWDG_Reset/Src/stm32f3xx_it.c          Interrupt handlers
  - IWDG/IWDG_Reset/Src/main.c                  Main program
  - IWDG/IWDG_Reset/Src/stm32f3xx_hal_msp.c     HAL MSP module
  - IWDG/IWDG_Reset/Src/system_stm32f3xx.c      STM32F3xx system clock configuration file   
     
@par Hardware and Software environment

  - This example runs on STM32F303xC devices.
    
  - This example has been tested with STM32303C-EVAL evaluation board and can be
    easily tailored to any other supported device and development board.   


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
   
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 