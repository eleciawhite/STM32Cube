/**
  ******************************************************************************
  * @file    TouchSensing\TouchSensing_Linear\Inc\stmCriticalSection.h
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    19-June-2015
  * @brief   stmCriticalSection.c header file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
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
  */

#ifndef STM_CRITICAL_SECTION_H
#define STM_CRITICAL_SECTION_H

/* To call before modifying any critical data. In case the host is inside or
asked for enterring the critical section, this routine will wait for the host to
leave the critical section. */
void enterLock(void);

/* Leave the critical section. If the host is waiting, access will be granted
to him. Otherwise the first next one asking will own the turn. */
void exitLock(void);

#endif /* STM_CRITICAL_SECTION_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
