/**
  @page  CEC_MultiAddress CEC Multiple Address communication example
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    CEC/CEC_MultiAddress/readme.txt 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    19-June-2015
  * @brief   Description of the CEC Multiple Address communication example.
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

This example shows how to configure and use the CEC peripheral to receive and 
transmit messages in the case where one device supports two distinct logical
addresses at the same time.


- Hardware Description

To use this example, two STM32373C_EVAL boards (called Device_1 and 
Device_2) are loaded with the same software then connected through CEC lines
(PB8 or HDMI connectors) and GND.
 /|\  In the firmware file main.h, uncomment the dedicated line to use
/_!_\ the CEC peripheral as STM32 device_1 or STM32 device_2.


@verbatim
*------------------------------------------------------------------------------*
|           STM32373C_EVAL                         STM32373C_EVAL              |
|         Device Address :0x01                    Device Address :0x03         |
|                                                 Device Address :0x05)         |
|         ____________________                   ____________________          |
|        |                    |                 |                    |         |
|        |                    |                 |                    |         | 
|        |     __________     |                 |     __________     |         |
|        |    |   CEC    |____|____CEC Line_____|____|   CEC    |    |         |
|        |    | Device_1 |    |                 |    | Device_2 |    |         |
|        |    |__________|    |                 |    |__________|    |         |
|        |                    |                 |                    |         |
|        |  O LD1             |                 |  O LD1             |         |
|        |  O LD2    Joystick |                 |  O LD2    Joystick |         |
|        |  O LD3        _    |                 |  O LD3        _    |         |
|        |  O LD4       |_|   |                 |  O LD4       |_|   |         |
|        |                    |                 |                    |         |
|        |             GND O--|-----------------|--O GND             |         |
|        |____________________|                 |____________________|         |
|                                                                              |
|                                                                              |
*------------------------------------------------------------------------------*
@endverbatim


- Software Description

The test unrolls as follows.

On Device_1 TX side, three possible messages can be transmitted and are 
indicated as below on the transmitting board:
 - when Tamper pushbutton is pressed, a ping message (no payload) is addressed
  to Device_2 first logical address; LED1 toggles
 - when Key pushbutton is pressed, a ping message (no payload) is addressed
  to Device_2 second logical address; LED4 toggles  
 - when Joystick Sel pushbutton is pressed, a broadcast message (with empty
 payload) is sent by Device_1; LED2 toggles


Accordingly, the following happens on Device_2 RX side in case of successful
transmission:
 - when Tamper pushbutton is pressed on TX side, 
      * LED1 is turned on
      * LED 3 and LED 4 are turned off 
 - when Key pushbutton is pressed on TX side, 
      * LED4 is turned on
      * LED 1 and LED 3 are turned off      
 - when Joystick Sel pushbutton is pressed on TX side, all LEDs are turned off 


Conversely, on Device_2 TX side, only ping messages addressed to the 
single Device_1 logical address or broadcast messages are sent:
 - when Tamper pushbutton is pressed, a ping message (no payload) is addressed
  to Device_1; LED1 toggles
 - when Key pushbutton is pressed, a ping message (no payload) is addressed
  to Device_1; LED4 toggles  
 - when Joystick Sel pushbutton is pressed, a broadcast message (with empty
 payload) is sent; LED2 toggles


Accordingly, the following happens on Device_1 RX side in case of successful
reception:
 - when Tamper or Key pushbutton are pressed on TX side, 
      * LED1 and LED 4 are turned on
      * LED 3 is turned off     
 - when Joystick Sel pushbutton is pressed on TX side, all LEDs are turned off 


For Device_1 and _2, in case of transmission or reception error, 
LED3 is turned on.


Practically, 4 EXTI lines (EXTI0, EXTI2 are EXTI9)are configured to 
generate an interrupt on each falling or rising edge. 
A specific message is then transmitted by the CEC IP
and a LED connected to a specific GPIO pin is toggled.
    - EXTI0  is mapped to PA0
    - EXTI2  is mapped to PA2 
    - EXTI9  is mapped to PE6  

Then, on TX side,
  when rising edge is detected on EXTI0, LED1 toggles
  when falling edge is detected on EXTI2, LED4 toggles
  when rising edge is detected on EXTI9, LED2 toggles

   

In this example, HCLK is configured at 72 MHz.

@par Directory contents 

  - CEC/CEC_MultiAddress/Inc/stm32f3xx_hal_conf.h    HAL configuration file
  - CEC/CEC_MultiAddress/Inc/stm32f3xx_it.h          Interrupt handlers header file
  - CEC/CEC_MultiAddress/Inc/main.h                  Header for main.c module  
  - CEC/CEC_MultiAddress/Src/stm32f3xx_it.c          Interrupt handlers
  - CEC/CEC_MultiAddress/Src/system_stm32f3xx.c      STM32F3xx system source file
  - CEC/CEC_MultiAddress/Src/main.c                  Main program
  - CEC/CEC_MultiAddress/Src/stm32f3xx_hal_msp.c     IP hardware ressourses initialization
  
@par Hardware and Software environment

  - This example runs on STM32F373xC devices.
    
  - This example has been tested with STM32373C-EVAL Rev B evaluation board and can be
    easily tailored to any other supported device and development board.      


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open the Projects\EWARM\Project.eww workspace
 - Rebuild all files: Project->Rebuild all
 - Load project image: Project->Download and Debug
 - Run program: Debug->Go(F5) 


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
