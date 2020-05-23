/*
 * sds011.h
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

#ifndef INC_SDS011_H_
#define INC_SDS011_H_

#include "stm32f0xx_hal.h"


typedef struct SDS_t {
  UART_HandleTypeDef* huart_sds;
  uint16_t pm_2_5;
  uint16_t  pm_10;
  uint8_t data_receive[19];
} SDS;


static const uint8_t Sds011_SleepCommand[] = {
		0xAA,
		0xB4,
		0x06,
		0x01,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0xFF,
		0xFF,
		0x05,
		0xAB
};

static const uint8_t Sds011_WorkingMode[] = {
		0xAA,
		0xB4,
		0x06,
		0x01,
		0x01,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0xFF,
		0xFF,
		0x06,
		0xAB
};


void sdsInit(SDS* sds, const UART_HandleTypeDef* huart_sds);
void sds_uart_RxCpltCallback(SDS* sds, UART_HandleTypeDef *huart);

int8_t sdsSend(SDS* sds, const uint8_t *data_buffer, const uint8_t length);

uint16_t sdsGetPm2_5(SDS* sds);
uint16_t sdsGetPm10(SDS* sds);

int8_t sdsWorkingMode(SDS* sds);
int8_t sdsSleepMode(SDS* sds);

uint8_t getCRC(uint8_t buff[]);


#endif /* SDS011_H_ */
