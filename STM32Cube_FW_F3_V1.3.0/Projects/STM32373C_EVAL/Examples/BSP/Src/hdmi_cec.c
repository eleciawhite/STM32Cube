/**
  ******************************************************************************
  * @file    BSP/Src/hdmi_cec.c 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    19-June-2015
  * @brief   This example code shows how to use HDMI CEC features.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
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
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup BSP_Examples
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint8_t DeviceType;
extern uint8_t MyLogicalAddress;
extern uint16_t MyPhysicalAddress;
extern uint8_t* HDMI_CEC_Follower_String[13][2];
extern __IO uint32_t ReceivedFrame;
extern HDMI_CEC_Message HDMI_CEC_RX_MessageStructPrivate;
/* Private function prototypes -----------------------------------------------*/
static void HdmiCec_SetHint(void);
static void HdmiCec_SelectDevice(void);
static void HdmiCec_DisplayInitInfo(void);
static void HdmiCec_DisplayConnectedDevices(void);
static void HdmiCec_DisplayReceivedMsg(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief Test HDMI CEC Hardware.
  * @param  None
  * @retval None
  */
void HDMI_CEC_demo(void)
{  
  /* Display the demo description */
  HdmiCec_SetHint();

  /* Configuration of the joystick in GPIO mode */
  BSP_JOY_Init(JOY_MODE_GPIO);
  
  /* Select the device type of the board */
  HdmiCec_SelectDevice();

  /* Initialization of the HDMI CEC */
  if (BSP_HDMI_CEC_Init() != HDMI_CEC_OK)
  {
    Error_Handler();
  }
  
  /* Display informations after initialization */
  HdmiCec_DisplayInitInfo();
  
  /* Display connected devices */
  HdmiCec_DisplayConnectedDevices();

  /* Infinite loop */
  while(!CheckForUserInput())
  { 
    if(ReceivedFrame != 0)
    {

      /* Display received message */
      HdmiCec_DisplayReceivedMsg();
      
      /* Handle the received messages */
      BSP_HDMI_CEC_CommandCallBack();
      
      /* Display connected devices */
      HdmiCec_DisplayConnectedDevices();
    }
  }
}

/**
  * @brief  Display HDMI CEC demo hint
  * @param  None
  * @retval None
  */
static void HdmiCec_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set Joystick Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"HDMI CEC", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows initialization and devices", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"detection according the device type choosen.", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"It also displays the received messages", CENTER_MODE);
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);

  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
}

/**
  * @brief  Select the type of device
  * @param  None
  * @retval None
  */
static void HdmiCec_SelectDevice(void)
{
  JOYState_TypeDef JoyKey;

  BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"Select your CEC Device Type", LEFT_MODE);
  BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"RIGHT --> Recording", LEFT_MODE);
  BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"LEFT --> Tuner", LEFT_MODE);
  BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"UP   --> Playback", LEFT_MODE);
  BSP_LCD_DisplayStringAt(20, 175, (uint8_t *)"DOWN --> AudioSystem", LEFT_MODE);
  
  do
  {
    JoyKey = BSP_JOY_GetState();
  } while(JoyKey == JOY_NONE);
  
  switch(JoyKey)
  {
  case JOY_DOWN :
    DeviceType = HDMI_CEC_AUDIOSYSTEM;
    BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize()-30, (uint8_t *)"Device selected : AudioSystem", LEFT_MODE);
    break;
  case JOY_LEFT :
    DeviceType = HDMI_CEC_TUNER;
    BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize()-30, (uint8_t *)"Device selected : Tuner", LEFT_MODE);
    break;
  case JOY_RIGHT :
    DeviceType = HDMI_CEC_RECORDING;
    BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize()-30, (uint8_t *)"Device selected : Recording", LEFT_MODE);
    break;
  case JOY_UP :
    DeviceType = HDMI_CEC_PLAYBACK;
    BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize()-30, (uint8_t *)"Device selected : Playback", LEFT_MODE);
    break;
  default :
    DeviceType = HDMI_CEC_TV;
    BSP_LCD_DisplayStringAt(20, BSP_LCD_GetYSize()-30, (uint8_t *)"No device selected (TV by default)", LEFT_MODE);
  }

}

/**
  * @brief  Display initialization informations about device
  * @param  None
  * @retval None
  */
static void HdmiCec_DisplayInitInfo(void)
{
  uint8_t string[50] = {0};

  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillRect(20, 100, BSP_LCD_GetYSize() - 40, 90);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

  BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"Device initialized", LEFT_MODE);
  sprintf((char *) string, "- Logical address : 0x%x", MyLogicalAddress);
  BSP_LCD_DisplayStringAt(40, 115, string, LEFT_MODE);
  sprintf((char *) string, "- Physical address : 0x%x", MyPhysicalAddress);
  BSP_LCD_DisplayStringAt(40, 130, string, LEFT_MODE);
}

/**
  * @brief  Display connected devices
  * @param  None
  * @retval None
  */
static void HdmiCec_DisplayConnectedDevices(void)
{
  uint8_t string[50] = {0};
  uint8_t i, connected_devices = 0;

  for (i = 0; i < 12; i++)
  {
    if (strcmp((const char*)(HDMI_CEC_Follower_String[i][1]), "0"))
    {
      connected_devices++;
    }
  }
  
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillRect(20, 175, BSP_LCD_GetYSize() - 40, 30);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

  if (connected_devices == 0)
  {
    BSP_LCD_DisplayStringAt(20, 175, (uint8_t *)"No devices connected", LEFT_MODE);
  }
  else 
  {
    sprintf((char *) string, "%d device(s) connected", connected_devices);
    BSP_LCD_DisplayStringAt(20, 175, string, LEFT_MODE);
  }
}

/**
  * @brief  Display message received
  * @param  None
  * @retval None
  */
static void HdmiCec_DisplayReceivedMsg(void)
{
  uint8_t string[50] = {0};

  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillRect(20, 100, BSP_LCD_GetYSize() - 40, 75);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);

  BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"Message received", LEFT_MODE);
  sprintf((char *) string, "- Sender address : 0x%x", (HDMI_CEC_RX_MessageStructPrivate.Header >> 0x4));
  BSP_LCD_DisplayStringAt(40, 115, string, LEFT_MODE);
  sprintf((char *) string, "- Message length : %d", HDMI_CEC_RX_MessageStructPrivate.RxMessageLength);
  BSP_LCD_DisplayStringAt(40, 130, string, LEFT_MODE);
  sprintf((char *) string, "- Message opcode : 0x%x", HDMI_CEC_RX_MessageStructPrivate.Opcode);
  BSP_LCD_DisplayStringAt(40, 145, string, LEFT_MODE);
}

/**
  * @}
  */ 


/**
  * @}
  */ 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
