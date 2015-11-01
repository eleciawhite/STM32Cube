/**
  ******************************************************************************
  * @file    stm32373c_eval_cec.c
  * @author  MCD Application Team
  * @version V2.0.2
  * @date    19-June-2015
  * @brief   This file provides all the STM32373C-EVAL HDMI-CEC firmware functions.
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
#include "stm32373c_eval_cec.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32_EVAL
  * @{
  */

/** @addtogroup STM32373C_EVAL
  * @{
  */

/** @defgroup STM32373C_EVAL_CEC
  * @brief This file includes the CEC Stack driver for HDMI-CEC Module
  *        of STM32373C-EVAL board
  * @{
  */

/** @defgroup STM32373C_EVAL_CEC_Private_Types
  * @{
  */

/**
  * @}
  */

/** @defgroup STM32373C_EVAL_CEC_Private_Defines
  * @{
  */

/**
  * @}
  */

/** @defgroup STM32373C_EVAL_CEC_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32373C_EVAL_CEC_Private_Variables
  * @{
  */
CEC_HandleTypeDef CecHandle;
__IO uint32_t ReceivedFrame = 0;
__IO uint32_t SendFrame = 0;
__IO uint8_t BufferPointer[15];
__IO uint32_t ReceiveStatus = 0;
__IO uint32_t SendStatus = 0;
__IO uint8_t TransErrorCode = 0;
uint8_t MyLogicalAddress = 0;
uint16_t MyPhysicalAddress = 0;
__IO uint8_t DeviceType = 0;
#ifdef HDMI_CEC_USE_DDC
uint8_t pBuffer[256];
__IO uint16_t NumByteToRead = 255;
#endif

__IO uint8_t CECDevicesNumber = 0;
HDMI_CEC_Message HDMI_CEC_TX_MessageStructPrivate;
HDMI_CEC_Message HDMI_CEC_RX_MessageStructPrivate;

__IO uint8_t FeatureOpcode = 0;
__IO uint8_t AbortReason = 0;
__IO uint8_t DeviceCount = 0;

HDMI_CEC_Map HDMI_CEC_MapStruct;
HDMI_CEC_Map HDMI_CEC_DeviceMap[14];

/* CEC follower addresses */
uint8_t* HDMI_CEC_Follower_String[13][2] =
  {
    {(uint8_t *)"         TV         ", (uint8_t *)"0"},
    {(uint8_t *)" Recording Device 1 ", (uint8_t *)"0"},
    {(uint8_t *)" Recording Device 2 ", (uint8_t *)"0"},
    {(uint8_t *)"      Tuner 1       ", (uint8_t *)"0"},
    {(uint8_t *)"  Playback Device 1 ", (uint8_t *)"0"},
    {(uint8_t *)"    Audio System    ", (uint8_t *)"0"},
    {(uint8_t *)"      Tuner 2       ", (uint8_t *)"0"},
    {(uint8_t *)"      Tuner 3       ", (uint8_t *)"0"},
    {(uint8_t *)"  Playback Device 2 ", (uint8_t *)"0"},
    {(uint8_t *)" Recording Device 3 ", (uint8_t *)"0"},
    {(uint8_t *)"      Tuner 4       ", (uint8_t *)"0"},
    {(uint8_t *)"  Playback Device 3 ", (uint8_t *)"0"},
    {(uint8_t *)"      Broadcast     ", (uint8_t *)"1"}
  };

/* EDID & CEA861 Extension */
const uint8_t EDID_Structure[256] =
{
  /*-------------------------- Block0: 128Bytes ------------------------------*/
  /* Header */
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
  /* Vendor & Product Identification */
  0x4C, 0x2D, 0x57, 0x03, 0x00, 0x00, 0x00, 0x00, 0x22, 0x11,
  /* EDID Structure Version & Revision */
  0x01, 0x03, 
  /* Basic Display Parameters & Features */
  0x80, 0x10, 0x09, 0x78, 0x0A, 
  /* Color Characteristics */
  0x1D, 0xED, 0xA7, 0x53, 0x34, 0xAC, 0x25, 0x12, 0x47, 0x4A,
  /* Established Timings */
  0x20, 0x00, 0x00,
  /* Standard Timings */
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01,
  /* Byte Data Blocks*/
  0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C, 0x45, 0x00, 0xA0, 
  0x5A, 0x00, 0x00, 0x00, 0x1E, 0x01, 0x1D, 0x00, 0x72, 0x51, 0xD0, 0x1E, 0x20, 
  0x6E, 0x28, 0x55, 0x00, 0xA0, 0x5A, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 
  0xFD, 0x00, 0x17, 0x3D, 0x1A, 0x44, 0x17, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 
  0x20, 0x20, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x53, 0x41, 0x4D, 0x53, 0x55, 0x4E, 
  0x47, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20,
  /* Extension Block Count */
  0x01,
  /* Checksum C */
  0xF3,
  /* ------------------------ Block1: 128Bytes------------------------------- */
  /* Refer to CEA 861 Standard for definitions */
  /* CEA 861 EXTENSION Block – Version 3 */
  0x02, 0x03,
  /* Byte number within this block where the 18-byte DTDs begin */
  0x21,
  /* Number of DTDs present */
  0x00,
  /* Start of Data Block Collection */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  /* IEEE Registration Identifier */
  0x03, 0x0C, 0x00,
  /* Vendor Specific Data: Physical Address */
  0x10, 0x00,
  /* Byte Data Blocks */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00
};

/**
  * @}
  */

/** @defgroup STM32373C_EVAL_CEC_Private_Function_Prototypes
  * @{
  */
static HDMI_CEC_Error HDMI_CEC_SinkPhysicalAddressDiscovery(void);
static HDMI_CEC_Error HDMI_CEC_SourcePhysicalAddressDiscovery(void);
static HDMI_CEC_Error HDMI_CEC_LogicalAddressAllocation(void);
static void SystemClock_ConfigAfterSTOP(void);

/**
  * @}
  */

/** @defgroup STM32373C_EVAL_CEC_Private_Functions
  * @{
  */

/**
  * @brief  Initializes the HDMI CEC.
  * @param  None
  * @retval HDMI_CEC_Error: CEC Error code
  */
HDMI_CEC_Error BSP_HDMI_CEC_Init(void)
{
  HDMI_CEC_Error errorstatus = HDMI_CEC_OK;
  uint8_t sendcount = 0;
  uint8_t idx = 0;
 
  for(idx=0; idx<12; idx++)
  {
    HDMI_CEC_Follower_String[idx][1] = (uint8_t *)"0";
  }

  HDMI_CEC_IO_Init();
  
  /* Configure CEC */
  CecHandle.Init.SignalFreeTime            = CEC_DEFAULT_SFT;
  CecHandle.Init.Tolerance                 = CEC_STANDARD_TOLERANCE;
  CecHandle.Init.BRERxStop                 = CEC_NO_RX_STOP_ON_BRE;
  CecHandle.Init.BREErrorBitGen            = CEC_BRE_ERRORBIT_NO_GENERATION;
  CecHandle.Init.LBPEErrorBitGen           = CEC_LBPE_ERRORBIT_NO_GENERATION;
  CecHandle.Init.BroadcastMsgNoErrorBitGen = CEC_BROADCASTERROR_ERRORBIT_GENERATION;
  CecHandle.Init.SignalFreeTimeOption      = CEC_SFT_START_ON_TXSOM;
  CecHandle.Init.OwnAddress                = 0xF;
  CecHandle.Init.ListenMode                = CEC_REDUCED_LISTENING_MODE;
  CecHandle.Init.InitiatorAddress          = 0xF;
  CecHandle.Instance                       = CEC;
  HAL_CEC_Init(&CecHandle);
  
  if (HDMI_CEC_ROOT != 0x1)
  {
    /* Source Physical Address discovery */
    errorstatus = HDMI_CEC_SourcePhysicalAddressDiscovery();
  }

  if (errorstatus == HDMI_CEC_OK)
  {
    /* Logical Address Allocation */
    sendcount = 0;
    do
    {
      errorstatus = HDMI_CEC_LogicalAddressAllocation();
      sendcount++;
    } while ((errorstatus != HDMI_CEC_OK) && sendcount < 0x5);

    if (errorstatus == HDMI_CEC_OK)
    {
      BSP_HDMI_CEC_CheckConnectedDevices();
    
      /* Set the CEC initiator address */
      /*CecHandle.Init.InitiatorAddress = MyLogicalAddress;
      HAL_CEC_Init(&CecHandle);*/

      if (HDMI_CEC_ROOT == 0x1)
      {
        /* Sink Physical Address discovery */
        errorstatus = HDMI_CEC_SinkPhysicalAddressDiscovery();
      }
    }
  }
 
  if (errorstatus == HDMI_CEC_OK)
  {
    HAL_CEC_Receive_IT(&CecHandle, (uint8_t *)(&HDMI_CEC_RX_MessageStructPrivate));
    
    /* Report physical address */
    sendcount = 0;
    do
    {
      errorstatus = BSP_HDMI_CEC_ReportPhysicalAddress();
      sendcount++;
    } while ((errorstatus != HDMI_CEC_OK) && sendcount < 0x5);
  }
  
  return errorstatus;
}

/**
  * @brief  Transmit message by taking data from typedef struct CEC_Meassage
  * @param  CEC_TX_MessageStructure: pointer to an CEC_Message structure that contains
  *         the message to be sent.
  * @retval HDMI_CEC_Error: CEC Error code
  */
HDMI_CEC_Error BSP_HDMI_CEC_TransmitMessage(HDMI_CEC_Message *HDMI_CEC_TX_MessageStructure)
{
  HAL_StatusTypeDef halstatus;
  __IO uint32_t j = 0;
  uint8_t length = HDMI_CEC_TX_MessageStructure->TxMessageLength;
  uint32_t BufferCount = 0;
  uint32_t tickstart = HAL_GetTick();

  /* Wait the end of the previous transmission */
  while (SendFrame != 0)
  {
    if((int32_t)(HAL_GetTick() - tickstart) >=  HDMI_CEC_TIMEOUT_VALUE)
    {
      return HDMI_CEC_TIMEOUT;
    }
  }
  SendFrame = 1;
  SendStatus = 0;
  /*BufferCount = 0;*/

  /* Initialize BufferPointer */
  for (j = 0; j < 15; j++)
  {
    BufferPointer[j] = 0;
  }

  BufferPointer[0] =  HDMI_CEC_TX_MessageStructure->Opcode;

  for (BufferCount = 1; BufferCount < length + 1; BufferCount++)
  {
    BufferPointer[BufferCount] = HDMI_CEC_TX_MessageStructure->Operande[BufferCount - 1];
  }

  halstatus = HAL_CEC_Transmit_IT(&CecHandle, HDMI_CEC_TX_MessageStructure->Header, (uint8_t *)BufferPointer, (length+1));
  /*halstatus = HAL_CEC_Transmit(&CecHandle, HDMI_CEC_TX_MessageStructure->Header, (uint8_t *)BufferPointer, (length+1), HDMI_CEC_TIMEOUT_VALUE);
  SendFrame = 0;*/

  if (halstatus == HAL_OK)
  {
    return HDMI_CEC_OK;
  }
  else
  {
    return HDMI_CEC_TX_ERROR ;
  }
}

/**
  * @brief  Get the ISR register status.
  * @param  None
  * @retval HDMI_CEC_Error: CEC Error code
  */
HAL_CEC_ErrorTypeDef BSP_HDMI_CEC_GetErrorStatus (void)
{
  return ((HAL_CEC_ErrorTypeDef)HAL_CEC_GetError(&CecHandle));
}

/**
  * @brief Tx Transfer completed callback
  * @param hcec: CEC handle
  * @retval None
  */
void HAL_CEC_TxCpltCallback(CEC_HandleTypeDef *hcec)
{
  SendFrame = 0;
  SendStatus = SUCCESS;
  
  if (__HAL_CEC_GET_IT_SOURCE(hcec, CEC_IER_RXENDIE))
  {
    hcec->State = HAL_CEC_STATE_STANDBY_RX;
  }
}

/**
  * @brief Rx Transfer completed callback
  * @param hcec: CEC handle
  * @retval None
  */
void HAL_CEC_RxCpltCallback(CEC_HandleTypeDef *hcec)
{
  if (hcec->RxXferSize == 1)
  {
    HDMI_CEC_RX_MessageStructPrivate.RxMessageLength = hcec->RxXferSize;
  }
  else
  {
    HDMI_CEC_RX_MessageStructPrivate.RxMessageLength = hcec->RxXferSize-1;
    ReceivedFrame = 1;
  }

  ReceiveStatus = SUCCESS;

  HAL_CEC_Receive_IT(hcec, (uint8_t *)(&HDMI_CEC_RX_MessageStructPrivate));
}

/**
  * @brief CEC error callbacks
  * @param hcec: CEC handle
  * @retval None
  */
void HAL_CEC_ErrorCallback(CEC_HandleTypeDef *hcec)
{
  HAL_CEC_ErrorTypeDef errorstatus;
  errorstatus = BSP_HDMI_CEC_GetErrorStatus();
  
  if ((errorstatus & (HAL_CEC_ERROR_RXOVR | HAL_CEC_ERROR_BRE | HAL_CEC_ERROR_SBPE | 
                      HAL_CEC_ERROR_LBPE | HAL_CEC_ERROR_RXACKE)) != 0)
  {
    ReceiveStatus  = 0;
    /*ReceivedFrame  = 1;*/
  }
  
  if ((errorstatus & (HAL_CEC_ERROR_ARBLST | HAL_CEC_ERROR_TXUDR | 
                      HAL_CEC_ERROR_TXERR | HAL_CEC_ERROR_TXACKE)) != 0)
  {
    TransErrorCode = errorstatus;
    SendFrame = 0;
    SendStatus = 0;
  }
  
  /* Re-initialize CEC */
  __HAL_CEC_DISABLE_IT(hcec, (CEC_IER_TX_ALL_ERR | CEC_IER_RX_ALL_ERR | 
                              CEC_IER_TXBRIE | CEC_IER_TXENDIE | 
                              CEC_IER_RXBRIE | CEC_IER_RXENDIE)); 
  HAL_CEC_DeInit(hcec);
  HAL_CEC_Init(hcec);
  HAL_CEC_Receive_IT(hcec, (uint8_t *)(&HDMI_CEC_RX_MessageStructPrivate));
}

/**
  * @brief  Report physical address to all other devices thus allowing any
            device to create a map of the network.
  * @param  None
  * @retval HDMI_CEC_Error: CEC Error code.
  */
HDMI_CEC_Error BSP_HDMI_CEC_ReportPhysicalAddress(void)
{
  HDMI_CEC_TX_MessageStructPrivate.Header = (0xF);
  HDMI_CEC_TX_MessageStructPrivate.Opcode = HDMI_CEC_OPCODE_REPORT_PHYSICAL_ADDRESS;
  HDMI_CEC_TX_MessageStructPrivate.Operande[0] = MyPhysicalAddress >> 8;
  HDMI_CEC_TX_MessageStructPrivate.Operande[1] = MyPhysicalAddress & 0xFF;
  HDMI_CEC_TX_MessageStructPrivate.Operande[2] = DeviceType;
  HDMI_CEC_TX_MessageStructPrivate.TxMessageLength = 0x03;

  return (BSP_HDMI_CEC_TransmitMessage(&HDMI_CEC_TX_MessageStructPrivate));
}

/**
  * @brief  Handle CEC command receive callback.
  *         When receiving the STANDBY Opcode command, the system is entered in
  *         Stop mode and when wakeup, the clock configuration shall be restored.
  * @param  None
  * @retval None
  */
void BSP_HDMI_CEC_CommandCallBack(void)
{
  uint8_t i = 0, sendcount = 0;
  HDMI_CEC_Error errorstatus = HDMI_CEC_OK;
  
  ReceivedFrame = 0;

  switch (HDMI_CEC_RX_MessageStructPrivate.Opcode)
  {
    case HDMI_CEC_OPCODE_REPORT_PHYSICAL_ADDRESS:
      HDMI_CEC_MapStruct.PhysicalAddress_A = HDMI_CEC_RX_MessageStructPrivate.Operande[1] >> 4;
      HDMI_CEC_MapStruct.PhysicalAddress_B = HDMI_CEC_RX_MessageStructPrivate.Operande[1] & 0x0F;
      HDMI_CEC_MapStruct.PhysicalAddress_C = HDMI_CEC_RX_MessageStructPrivate.Operande[0] >> 4;
      HDMI_CEC_MapStruct.PhysicalAddress_D = HDMI_CEC_RX_MessageStructPrivate.Operande[0] & 0x0F;
      HDMI_CEC_MapStruct.LogicalAddress = (HDMI_CEC_RX_MessageStructPrivate.Header >> 0x4) & 0x0F;
      HDMI_CEC_MapStruct.DeviceType = HDMI_CEC_RX_MessageStructPrivate.Operande[2];
      HDMI_CEC_DeviceMap[DeviceCount] = HDMI_CEC_MapStruct;
      HDMI_CEC_Follower_String[(HDMI_CEC_DeviceMap[DeviceCount].LogicalAddress)][1] = (uint8_t *)"1";
      DeviceCount++;
      break;

    case HDMI_CEC_OPCODE_STANDBY:
      /* Request to enter Stop mode */
      HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);

      /* Re-configure clocks */
      SystemClock_ConfigAfterSTOP();
      break;

    case HDMI_CEC_OPCODE_GET_CEC_VERSION:
      /* Send the Used CEC version */
      HDMI_CEC_TX_MessageStructPrivate.Header = (HDMI_CEC_RX_MessageStructPrivate.Header >> 4);
      HDMI_CEC_TX_MessageStructPrivate.Opcode = HDMI_CEC_OPCODE_CEC_VERSION;
      HDMI_CEC_TX_MessageStructPrivate.Operande[0] = HDMI_CEC_VERSION; /* CEC Version */
      HDMI_CEC_TX_MessageStructPrivate.TxMessageLength = 0x01;
      errorstatus = BSP_HDMI_CEC_TransmitMessage(&HDMI_CEC_TX_MessageStructPrivate);

      /* Retransmit message until 5 time */
      while ((errorstatus != HDMI_CEC_OK) && sendcount < 0x5)
      {
        sendcount++;
        errorstatus = BSP_HDMI_CEC_TransmitMessage(&HDMI_CEC_TX_MessageStructPrivate);
      }
      break;

    case HDMI_CEC_OPCODE_GIVE_PHYSICAL_ADDRESS:
      /* Send the Physical address */
      errorstatus = BSP_HDMI_CEC_ReportPhysicalAddress();
      sendcount = 0;
      /* Retransmit message until 5 time */
      while ((errorstatus != HDMI_CEC_OK) && sendcount < 0x5)
      {
        sendcount++;
        errorstatus = BSP_HDMI_CEC_ReportPhysicalAddress();
      }
      break;

    case HDMI_CEC_OPCODE_FEATURE_ABORT:
      /* The device doesn't support the requested message type, or that it cannot
      execute it at the present time. */
      FeatureOpcode = HDMI_CEC_RX_MessageStructPrivate.Operande[0];
      AbortReason = HDMI_CEC_RX_MessageStructPrivate.Operande[1];
      break;

    case HDMI_CEC_OPCODE_GIVE_OSD_NAME:
      /* Send the OSD name = STM32373C CEC */
      HDMI_CEC_TX_MessageStructPrivate.Header = (HDMI_CEC_RX_MessageStructPrivate.Header >> 4);
      HDMI_CEC_TX_MessageStructPrivate.Opcode = HDMI_CEC_OPCODE_SET_OSD_NAME;
      /* STM32373C */
      HDMI_CEC_TX_MessageStructPrivate.Operande[0] = 0x53;
      HDMI_CEC_TX_MessageStructPrivate.Operande[1] = 0x54;
      HDMI_CEC_TX_MessageStructPrivate.Operande[2] = 0x4D;
      HDMI_CEC_TX_MessageStructPrivate.Operande[3] = 0x33;
      HDMI_CEC_TX_MessageStructPrivate.Operande[4] = 0x32;
      HDMI_CEC_TX_MessageStructPrivate.Operande[5] = 0x33;
      HDMI_CEC_TX_MessageStructPrivate.Operande[6] = 0x37;
      HDMI_CEC_TX_MessageStructPrivate.Operande[7] = 0x33;
      HDMI_CEC_TX_MessageStructPrivate.Operande[8] = 0x43;
      HDMI_CEC_TX_MessageStructPrivate.Operande[9] = 0x20;
      /* CEC */
      HDMI_CEC_TX_MessageStructPrivate.Operande[10] = 0x43;
      HDMI_CEC_TX_MessageStructPrivate.Operande[11] = 0x45;
      HDMI_CEC_TX_MessageStructPrivate.Operande[12] = 0x43;
      HDMI_CEC_TX_MessageStructPrivate.TxMessageLength = 13;
      errorstatus = BSP_HDMI_CEC_TransmitMessage(&HDMI_CEC_TX_MessageStructPrivate);
      sendcount = 0;
      /* Retransmit message until 5 time */
      while ((errorstatus != HDMI_CEC_OK) && sendcount < 0x5)
      {
        sendcount++;
        errorstatus = BSP_HDMI_CEC_TransmitMessage(&HDMI_CEC_TX_MessageStructPrivate);
      }
      break;

    case HDMI_CEC_OPCODE_ROUTING_CHANGE:
      for (i = 0;i < 14;i++)
      {
        if ((HDMI_CEC_DeviceMap[i].PhysicalAddress_A == (HDMI_CEC_RX_MessageStructPrivate.Operande[1] >> 4)) &&
            (HDMI_CEC_DeviceMap[i].PhysicalAddress_B == (HDMI_CEC_RX_MessageStructPrivate.Operande[1]&0x0F)) &&
            (HDMI_CEC_DeviceMap[i].PhysicalAddress_C == (HDMI_CEC_RX_MessageStructPrivate.Operande[0] >> 4)) &&
            (HDMI_CEC_DeviceMap[i].PhysicalAddress_D == (HDMI_CEC_RX_MessageStructPrivate.Operande[0]&0x0F)))
        {
          HDMI_CEC_MapStruct.LogicalAddress = (HDMI_CEC_RX_MessageStructPrivate.Header >> 0x4) & 0x0F;
          HDMI_CEC_MapStruct.DeviceType = HDMI_CEC_RX_MessageStructPrivate.Operande[2];
          HDMI_CEC_DeviceMap[i] = HDMI_CEC_MapStruct;
        }
      }
      break;

    default:
      /* Send Abort feature */
      HDMI_CEC_TX_MessageStructPrivate.Header = (HDMI_CEC_RX_MessageStructPrivate.Header >> 4);
      HDMI_CEC_TX_MessageStructPrivate.Opcode = HDMI_CEC_OPCODE_FEATURE_ABORT;
      HDMI_CEC_TX_MessageStructPrivate.Operande[0] = 0x02; /* Defines command to be performed */
      HDMI_CEC_TX_MessageStructPrivate.Operande[1] = HDMI_CEC_REFUSED; /* Reason for abort feature */
      HDMI_CEC_TX_MessageStructPrivate.TxMessageLength = 0x02;
      errorstatus = BSP_HDMI_CEC_TransmitMessage(&HDMI_CEC_TX_MessageStructPrivate);
      sendcount = 0;
      /* Retransmit message until 5 time */
      while ((errorstatus != HDMI_CEC_OK) && sendcount < 0x5)
      {
        sendcount++;
        errorstatus = BSP_HDMI_CEC_TransmitMessage(&HDMI_CEC_TX_MessageStructPrivate);
      }
      break;
  }
}

/**
  * @brief  Check the connected CEC devices.
  * @param  None
  * @retval HDMI_CEC_Error
  */
HDMI_CEC_Error BSP_HDMI_CEC_CheckConnectedDevices(void)
{
  HDMI_CEC_Error errorstatus = HDMI_CEC_OK;
  uint32_t i = 1;
  HAL_StatusTypeDef halstatus;
  uint8_t index;

  for (index = 0; index <= 0xB; index++)
  {
    halstatus = HAL_CEC_Transmit(&CecHandle, index, NULL, 0, HDMI_CEC_TIMEOUT_VALUE);
  
    if (halstatus == HAL_OK)
    {
      HDMI_CEC_Follower_String[index][1] = (uint8_t *)"1";
      i++;
      errorstatus = HDMI_CEC_OK;
    }
    else if (halstatus == HAL_TIMEOUT)
    {
      errorstatus = HDMI_CEC_TIMEOUT;
      return(errorstatus);
    }
    else
    {
      errorstatus = (HDMI_CEC_Error)BSP_HDMI_CEC_GetErrorStatus();
      /* Re-initialize CEC */
      HAL_CEC_DeInit(&CecHandle);
      HAL_CEC_Init(&CecHandle);
    }
  }

  CECDevicesNumber = i - 1;

  return errorstatus;
}

/**
  * @brief  Sink Physical address discovery.
  * @param  None
  * @retval HDMI_CEC_Error: CEC Error code.
  */
static HDMI_CEC_Error HDMI_CEC_SinkPhysicalAddressDiscovery(void)
{
  HDMI_CEC_Error errorstatus = HDMI_CEC_OK;

  /*------------------------------ Physical address discovery ----------------*/
  /* if HDMI_CEC_ROOT == 0x1*/
  MyPhysicalAddress = 0x0000;
  /* The HDMI-CEC here is configured as sink or as a repeater. The configuration
  of the +5V power signal and the HPD should be well configured.
  Implement here the EDID Structure to be sent to the HDMI source.
  For more details please refer to the HDMI specification. 
  The EDID structure should be sent to the device source using the DDC Channel
  and using the HPD signal. */

  if (HDMI_CEC_IO_WriteData((uint8_t *)EDID_Structure, 256) != HAL_OK)
  {
    errorstatus = HDMI_CEC_I2C_ERROR;
  }

  return errorstatus;
}

/**
  * @brief  Source Physical address discovery.
  * @param  None
  * @retval HDMI_CEC_Error: CEC Error code.
  */
static HDMI_CEC_Error HDMI_CEC_SourcePhysicalAddressDiscovery(void)
{
  HDMI_CEC_Error errorstatus = HDMI_CEC_OK;

#ifdef HDMI_CEC_USE_DDC
/* This configuration is only when the HDMI CEC is configured as source.
   The HDMI source has to provide the +5V Power signal to the sink. 
   Then, the source will wait for HPD signal to be asserted from the sink.
   Once the HPD signal is detected the source shall read the EDID structure 
   through the DDC channel. */

  /* The HDMI-CEC here is configured as source or as a repeater. The configuration
  of the +5V power signal and the HPD should be well configured.
  The source should wait for HPD and then read the EDID structure. */
  while(HAL_GPIO_ReadPin(HDMI_CEC_HPD_SOURCE_GPIO_PORT, HDMI_CEC_HPD_SOURCE_PIN) == GPIO_PIN_RESET)
  {
  }

  /* Wait for 100 ms after HPD was received */
  HAL_Delay(100);
  
  if (HDMI_CEC_IO_ReadData(HDMI_CEC_I2C_ADDRESS, pBuffer, 256) != HAL_OK)
  {
    errorstatus = HDMI_CEC_I2C_ERROR;
  }
  
  MyPhysicalAddress = ((pBuffer[158] << 8)|pBuffer[159]);
#else
  MyPhysicalAddress = 0x1000;
#endif

  return errorstatus;
}

/**
  * @brief  Allocate the logical address.
  * @param  None
  * @retval HDMI_CEC_Error: CEC Error code.
  */
static HDMI_CEC_Error HDMI_CEC_LogicalAddressAllocation(void)
{
  HDMI_CEC_Error errorstatus = HDMI_CEC_OK;
  HAL_StatusTypeDef halstatus;
  HAL_CEC_ErrorTypeDef halerrorstatus;

  /*------------------ Logical address allocation ----------------------------*/
  /* Get the device type */
  /* Device type = CEC_TV */
  if (DeviceType == HDMI_CEC_TV)
  {
    if (HDMI_CEC_ROOT)
    {
      MyLogicalAddress = 0x00;
      CecHandle.Init.OwnAddress = (1 << 0x0); /* Own address = 0x0 */
      CecHandle.Init.InitiatorAddress = 0x0;
      HAL_CEC_Init(&CecHandle);
    }
    else
    {
      CecHandle.Init.OwnAddress = (1 << 0xE); /* Own address = 0xE */
      CecHandle.Init.InitiatorAddress = 0xE;
      HAL_CEC_Init(&CecHandle);

      halstatus = HAL_CEC_Transmit(&CecHandle, 0xE, NULL, 0, HDMI_CEC_TIMEOUT_VALUE);
      if (halstatus == HAL_TIMEOUT)
      {
        errorstatus = HDMI_CEC_TIMEOUT;
        return(errorstatus);
      }

      halerrorstatus = BSP_HDMI_CEC_GetErrorStatus();

      if ((halerrorstatus & HAL_CEC_ERROR_TXACKE) != 0)
      {
        MyLogicalAddress = 0x0E;
        /* Re-initialize CEC */
        HAL_CEC_DeInit(&CecHandle);
        HAL_CEC_Init(&CecHandle);
        errorstatus = HDMI_CEC_OK;
      }
      else if (halerrorstatus == HAL_CEC_ERROR_NONE)
      {
        MyLogicalAddress = 0x0F;
        CecHandle.Init.OwnAddress = 0xF;
        CecHandle.Init.InitiatorAddress = 0xF;
        HAL_CEC_Init(&CecHandle);
        errorstatus = HDMI_CEC_DEVICE_UNREGISTRED;
      }
    }
  }

  /* Device type = CEC_RECORDING */
  if (DeviceType == HDMI_CEC_RECORDING)
  {
    CecHandle.Init.OwnAddress = (1 << 0x1); /* Own address = 0x1 */
    CecHandle.Init.InitiatorAddress = 0x1;
    HAL_CEC_Init(&CecHandle);

    halstatus = HAL_CEC_Transmit(&CecHandle, 0x1, NULL, 0, HDMI_CEC_TIMEOUT_VALUE);
    if (halstatus == HAL_TIMEOUT)
    {
      errorstatus = HDMI_CEC_TIMEOUT;
      return(errorstatus);
    }

    halerrorstatus = BSP_HDMI_CEC_GetErrorStatus();

    if ((halerrorstatus & HAL_CEC_ERROR_TXACKE) != 0)
    {
      MyLogicalAddress = 0x01;
      /* Re-initialize CEC */
      HAL_CEC_DeInit(&CecHandle);
      HAL_CEC_Init(&CecHandle);
      errorstatus = HDMI_CEC_OK;
    }
    else if (halerrorstatus == HAL_CEC_ERROR_NONE)
    {
      CecHandle.Init.OwnAddress = (1 << 0x2); /* Own address = 0x2 */
      CecHandle.Init.InitiatorAddress = 0x2;
      HAL_CEC_Init(&CecHandle);

      halstatus = HAL_CEC_Transmit(&CecHandle, 0x2, NULL, 0, HDMI_CEC_TIMEOUT_VALUE);
      if (halstatus == HAL_TIMEOUT)
      {
        errorstatus = HDMI_CEC_TIMEOUT;
        return(errorstatus);
      }

      halerrorstatus = BSP_HDMI_CEC_GetErrorStatus();

      if ((halerrorstatus & HAL_CEC_ERROR_TXACKE) != 0)
      {
        MyLogicalAddress = 0x02;
        /* Re-initialize CEC */
        HAL_CEC_DeInit(&CecHandle);
        HAL_CEC_Init(&CecHandle);
        errorstatus = HDMI_CEC_OK;
      }
      else if (halerrorstatus == HAL_CEC_ERROR_NONE)
      {
        CecHandle.Init.OwnAddress = (1 << 0x9); /* Own address = 0x9 */
        CecHandle.Init.InitiatorAddress = 0x9;
        HAL_CEC_Init(&CecHandle);

        halstatus = HAL_CEC_Transmit(&CecHandle, 0x9, NULL, 0, HDMI_CEC_TIMEOUT_VALUE);
        if (halstatus == HAL_TIMEOUT)
        {
          errorstatus = HDMI_CEC_TIMEOUT;
          return(errorstatus);
        }

        halerrorstatus = BSP_HDMI_CEC_GetErrorStatus();

        if ((halerrorstatus & HAL_CEC_ERROR_TXACKE) != 0)
        {
          MyLogicalAddress = 0x09;
          /* Re-initialize CEC */
          HAL_CEC_DeInit(&CecHandle);
          HAL_CEC_Init(&CecHandle);
          errorstatus = HDMI_CEC_OK;
        }
        else if (halerrorstatus == HAL_CEC_ERROR_NONE)
        {
          MyLogicalAddress = 0x0F;
          CecHandle.Init.OwnAddress = 0xF;
          CecHandle.Init.InitiatorAddress = 0xF;
          HAL_CEC_Init(&CecHandle);
          errorstatus = HDMI_CEC_DEVICE_UNREGISTRED;
        }
      }
    }
  }

  /* Device type = CEC_TUNER */
  if (DeviceType == HDMI_CEC_TUNER)
  {
    CecHandle.Init.OwnAddress = (1 << 0x3); /* Own address = 0x3 */
    CecHandle.Init.InitiatorAddress = 0x3;
    HAL_CEC_Init(&CecHandle);

    halstatus = HAL_CEC_Transmit(&CecHandle, 0x3, NULL, 0, HDMI_CEC_TIMEOUT_VALUE);
    if (halstatus == HAL_TIMEOUT)
    {
      errorstatus = HDMI_CEC_TIMEOUT;
      return(errorstatus);
    }

    halerrorstatus = BSP_HDMI_CEC_GetErrorStatus();

    if ((halerrorstatus & HAL_CEC_ERROR_TXACKE) != 0)
    {
      MyLogicalAddress = 0x03;
      /* Re-initialize CEC */
      HAL_CEC_DeInit(&CecHandle);
      HAL_CEC_Init(&CecHandle);
      errorstatus = HDMI_CEC_OK;
    }
    else if (halerrorstatus == HAL_CEC_ERROR_NONE)
    {
      CecHandle.Init.OwnAddress = (1 << 0x6); /* Own address = 0x6 */
      CecHandle.Init.InitiatorAddress = 0x6;
      HAL_CEC_Init(&CecHandle);

      halstatus = HAL_CEC_Transmit(&CecHandle, 0x6, NULL, 0, HDMI_CEC_TIMEOUT_VALUE);
      if (halstatus == HAL_TIMEOUT)
      {
        errorstatus = HDMI_CEC_TIMEOUT;
        return(errorstatus);
      }

      halerrorstatus = BSP_HDMI_CEC_GetErrorStatus();

      if ((halerrorstatus & HAL_CEC_ERROR_TXACKE) != 0)
      {
        MyLogicalAddress = 0x06;
        /* Re-initialize CEC */
        HAL_CEC_DeInit(&CecHandle);
        HAL_CEC_Init(&CecHandle);
        errorstatus = HDMI_CEC_OK;
      }
      else if (halerrorstatus == HAL_CEC_ERROR_NONE)
      {
        CecHandle.Init.OwnAddress = (1 << 0x7); /* Own address = 0x7 */
        CecHandle.Init.InitiatorAddress = 0x7;
        HAL_CEC_Init(&CecHandle);

        halstatus = HAL_CEC_Transmit(&CecHandle, 0x7, NULL, 0, HDMI_CEC_TIMEOUT_VALUE);
        if (halstatus == HAL_TIMEOUT)
        {
          errorstatus = HDMI_CEC_TIMEOUT;
          return(errorstatus);
        }

        halerrorstatus = BSP_HDMI_CEC_GetErrorStatus();

        if ((halerrorstatus & HAL_CEC_ERROR_TXACKE) != 0)
        {
          MyLogicalAddress = 0x07;
          /* Re-initialize CEC */
          HAL_CEC_DeInit(&CecHandle);
          HAL_CEC_Init(&CecHandle);
          errorstatus = HDMI_CEC_OK;
        }
        else if (halerrorstatus == HAL_CEC_ERROR_NONE)
        {
          CecHandle.Init.OwnAddress = (1 << 0xA); /* Own address = 0xA */
          CecHandle.Init.InitiatorAddress = 0xA;
          HAL_CEC_Init(&CecHandle);

          halstatus = HAL_CEC_Transmit(&CecHandle, 0xA, NULL, 0, HDMI_CEC_TIMEOUT_VALUE);
          if (halstatus == HAL_TIMEOUT)
          {
            errorstatus = HDMI_CEC_TIMEOUT;
            return(errorstatus);
          }

          halerrorstatus = BSP_HDMI_CEC_GetErrorStatus();

          if ((halerrorstatus & HAL_CEC_ERROR_TXACKE) != 0)
          {
            MyLogicalAddress = 0x0A;
            /* Re-initialize CEC */
            HAL_CEC_DeInit(&CecHandle);
            HAL_CEC_Init(&CecHandle);
            errorstatus = HDMI_CEC_OK;
          }
          else if (halerrorstatus == HAL_CEC_ERROR_NONE)
          {
            MyLogicalAddress = 0x0F;
            CecHandle.Init.OwnAddress = 0xF;
            CecHandle.Init.InitiatorAddress = 0xF;
            HAL_CEC_Init(&CecHandle);
            errorstatus = HDMI_CEC_DEVICE_UNREGISTRED;
          }
        }
      }
    }
  }

  /* Device type = CEC_PLAYBACK */
  if (DeviceType == HDMI_CEC_PLAYBACK)
  {
    CecHandle.Init.OwnAddress = (1 << 0x4); /* Own address = 0x4 */
    CecHandle.Init.InitiatorAddress = 0x4;
    HAL_CEC_Init(&CecHandle);

    halstatus = HAL_CEC_Transmit(&CecHandle, 0x4, NULL, 0, HDMI_CEC_TIMEOUT_VALUE);
    if (halstatus == HAL_TIMEOUT)
    {
      errorstatus = HDMI_CEC_TIMEOUT;
      return(errorstatus);
    }

    halerrorstatus = BSP_HDMI_CEC_GetErrorStatus();

    if ((halerrorstatus & HAL_CEC_ERROR_TXACKE) != 0)
    {
      MyLogicalAddress = 0x04;
      /* Re-initialize CEC */
      HAL_CEC_DeInit(&CecHandle);
      HAL_CEC_Init(&CecHandle);
      errorstatus = HDMI_CEC_OK;
    }
    else if (halerrorstatus == HAL_CEC_ERROR_NONE)
    {
      CecHandle.Init.OwnAddress = (1 << 0x8); /* Own address = 0x8 */
      CecHandle.Init.InitiatorAddress = 0x8;
      HAL_CEC_Init(&CecHandle);

      halstatus = HAL_CEC_Transmit(&CecHandle, 0x8, NULL, 0, HDMI_CEC_TIMEOUT_VALUE);
      if (halstatus == HAL_TIMEOUT)
      {
        errorstatus = HDMI_CEC_TIMEOUT;
        return(errorstatus);
      }

      halerrorstatus = BSP_HDMI_CEC_GetErrorStatus();

      if ((halerrorstatus & HAL_CEC_ERROR_TXACKE) != 0)
      {
        MyLogicalAddress = 0x08;
        /* Re-initialize CEC */
        HAL_CEC_DeInit(&CecHandle);
        HAL_CEC_Init(&CecHandle);
        errorstatus = HDMI_CEC_OK;
      }
      else if (halerrorstatus == HAL_CEC_ERROR_NONE)
      {
        CecHandle.Init.OwnAddress = (1 << 0xB); /* Own address = 0xB */
        CecHandle.Init.InitiatorAddress = 0xB;
        HAL_CEC_Init(&CecHandle);

        halstatus = HAL_CEC_Transmit(&CecHandle, 0xB, NULL, 0, HDMI_CEC_TIMEOUT_VALUE);
        if (halstatus == HAL_TIMEOUT)
        {
          errorstatus = HDMI_CEC_TIMEOUT;
          return(errorstatus);
        }

        halerrorstatus = BSP_HDMI_CEC_GetErrorStatus();

        if ((halerrorstatus & HAL_CEC_ERROR_TXACKE) != 0)
        {
          MyLogicalAddress = 0x0B;
          /* Re-initialize CEC */
          HAL_CEC_DeInit(&CecHandle);
          HAL_CEC_Init(&CecHandle);
          errorstatus = HDMI_CEC_OK;
        }
        else if (halerrorstatus == HAL_CEC_ERROR_NONE)
        {
          MyLogicalAddress = 0x0F;
          CecHandle.Init.OwnAddress = 0xF;
          CecHandle.Init.InitiatorAddress = 0xF;
          HAL_CEC_Init(&CecHandle);
          errorstatus = HDMI_CEC_DEVICE_UNREGISTRED;
        }
      }
    }
  }

  /* Device type = CEC Audio System */
  if (DeviceType == HDMI_CEC_AUDIOSYSTEM)
  {
    CecHandle.Init.OwnAddress = (1 << 0x5); /* Own address = 0x5 */
    CecHandle.Init.InitiatorAddress = 0x5;
    HAL_CEC_Init(&CecHandle);

    halstatus = HAL_CEC_Transmit(&CecHandle, 0x5, NULL, 0, HDMI_CEC_TIMEOUT_VALUE);
    if (halstatus == HAL_TIMEOUT)
    {
      errorstatus = HDMI_CEC_TIMEOUT;
      return(errorstatus);
    }

    halerrorstatus = BSP_HDMI_CEC_GetErrorStatus();

    if ((halerrorstatus & HAL_CEC_ERROR_TXACKE) != 0)
    {
      MyLogicalAddress = 0x05;
      /* Re-initialize CEC */
      HAL_CEC_DeInit(&CecHandle);
      HAL_CEC_Init(&CecHandle);
      errorstatus = HDMI_CEC_OK;
    }
    else if (halerrorstatus == HAL_CEC_ERROR_NONE)
    {
      MyLogicalAddress = 0x0F;
      CecHandle.Init.OwnAddress = 0xF;
      CecHandle.Init.InitiatorAddress = 0xF;
      HAL_CEC_Init(&CecHandle);
      errorstatus = HDMI_CEC_DEVICE_UNREGISTRED;
    }
  }

  return errorstatus;
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            HSE PREDIV                     = 1
  *            PLLMUL                         = RCC_PLL_MUL9 (9)
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
static void SystemClock_ConfigAfterSTOP(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  HAL_RCC_OscConfig(&RCC_OscInitStruct); 

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2); 
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
