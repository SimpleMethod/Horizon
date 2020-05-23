/*
 * sds011.c
 *
 *  Created on: 10.03.2019
 *      Author: SimpleMethod
 *
 *Copyright 2019 SimpleMethod
 *
 *Permission is hereby granted, free of charge, to any person obtaining a copy of
 *this software and associated documentation files (the "Software"), to deal in
 *the Software without restriction, including without limitation the rights to
 *use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 *of the Software, and to permit persons to whom the Software is furnished to do
 *so, subject to the following conditions:
 *
 *The above copyright notice and this permission notice shall be included in all
 *copies or substantial portions of the Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *THE SOFTWARE.
******************************************************************************
*/


#include "sds011.h"

/**
 * UART settings:
 *
 * Baud Rate: 9600 Bits/s
 *
 * Word Length: 8 Bits
 *
 * Parity None
 *
 * Stop Bits 1
 *
 * Enable NVIC global interrupt!
 *
 */


/**
 *   Library initialization.
 *
 *   @param sds pointer to SDS_t handle structure
 *   @param huart_sds pointer to UART handle structure
*/
void sdsInit(SDS* sds, const UART_HandleTypeDef* huart_sds)
{
sds->huart_sds=(UART_HandleTypeDef *)huart_sds;
HAL_UART_Transmit(sds->huart_sds,(uint8_t*)Sds011_WorkingMode, 19,30);
HAL_UART_Receive_IT(sds->huart_sds, sds->data_receive, 10);
}



/**
 *   Calculation of the checksum
 *
 *   @param buff array with a message to send to the device
 *   @return calculated checksum
*/
uint8_t getCRC(uint8_t buff[]) {
  uint8_t crc = 0;
  for (uint8_t i = 2; i < 17; ++i) {
    crc += buff[i];
  }
  return crc;
}


/**
 *   Enter the device into sleep mode
 *
 *   @param sds pointer to SDS_t handle structure
 *   @return status of the message transmission
*/
int8_t sdsSleepMode(SDS* sds)
{
	return HAL_UART_Transmit(sds->huart_sds, (uint8_t*)Sds011_SleepCommand,19,30)==HAL_OK ? 1:0;
}

/**
 *  Enter the device into working mode
 *
 *   @param sds pointer to SDS_t handle structure
 *   @return status of the message transmission
*/
int8_t sdsWorkingMode(SDS* sds)
{
	return HAL_UART_Transmit(sds->huart_sds, (uint8_t*)Sds011_WorkingMode,19,30)==HAL_OK ? 1:0;
}

/**
 *   Send any message to the device
 *
 *   @param sds pointer to SDS_t handle structure
 *   @param data_buffer array with a message to send to the device
 *   @param message length
 *   @return status of the message transmission
*/
int8_t sdsSend(SDS* sds, const uint8_t* data_buffer, const uint8_t length)
{
	return HAL_UART_Transmit(sds->huart_sds,(uint8_t *) data_buffer,length,30)==HAL_OK ? 1:0;
}

/**
 *   Getting the value of PM 2.5
 *
 *   @param sds pointer to SDS_t handle structure
 *   @return value of PM 2.5
*/
uint16_t sdsGetPm2_5(SDS* sds)
{
	return  sds->pm_2_5;
}

/**
 *   Getting the value of PM 10
 *
 *   @param sds pointer to SDS_t handle structure
 *   @return value of PM 10
*/
uint16_t sdsGetPm10(SDS* sds)
{
	return  sds->pm_10;
}

/**
 *   UART Rx Complete Callback
 *
 *   @param sds pointer to SDS_t handle structure
 *   @param huart pointer to UART handle structure
*/
void sds_uart_RxCpltCallback(SDS* sds, UART_HandleTypeDef *huart)
{
	if(huart == sds->huart_sds)
		{
				if((sds->data_receive[1] == 0xC0))
				{
					sds->pm_2_5 = ((sds->data_receive[3]<<8)| sds->data_receive[2])/10;
					sds->pm_10 = ((sds->data_receive[5]<<8)| sds->data_receive[4])/10;
			}
			HAL_UART_Receive_IT(sds->huart_sds, sds->data_receive, 10);
		}
}
