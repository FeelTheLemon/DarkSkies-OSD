#include "Input.h"
#include <stdint.h>
#include <string.h>
#include <usart.h>

struct sOSD_Data OSD_Data;

uint8_t rx_buf[64];
const size_t buf_shift = 32;

void Input_Init(void)
{
	//buf_shift = sizeof(rx_buf) / 2;

	HAL_StatusTypeDef status = HAL_UART_Receive_DMA(&huart2, rx_buf, sizeof(rx_buf));

	if (status != HAL_OK) printf("Input_Init FAILED with status 0x%x, error: 0x%lx , state: 0x%x\r\n", status, HAL_UART_GetError(&huart2), HAL_UART_GetState(&huart2));
}

uint32_t serialize(struct sOSD_Data* in, uint8_t* out)
{

	float* q = (float*)out;
	*q = in->pitch;				q++;
	*q = in->yaw;				q++;
	*q = in->roll;				q++;

	*q = in->V;					q++;
	*q = in->I;					q++;
	*q = in->P;					q++;

	*q = in->altitude_bar;		q++;
	*q = in->speedKph_bar;		q++;

	*q = in->latitude;			q++;
	*q = in->longitude;			q++;
	*q = in->altitude;			q++;
	*q = in->speedKph;			q++;
	*q = in->PDOP;				q++;

	unsigned int * p = (unsigned int *)q;
	*p = in->time;				p++;

	char* c = (char*)p;
	*c = in->status;			c++;
	*c = in->NS;				c++;
	*c = in->EW;				c++;

	unsigned char* s = (unsigned char*)c;
	*s = in->satellitesNum;		s++;

	return (uint32_t)q - (uint32_t)out;
}

uint32_t deserialize(uint8_t* in, struct sOSD_Data* out)
{
	float* q = (float*)in;
	out->pitch = *q;				q++;
	out->yaw = *q;					q++;
	out->roll = *q;					q++;

	out->V = *q;					q++;
	out->I = *q;					q++;
	out->P = *q;					q++;

	out->altitude_bar = *q;			q++;
	out->speedKph_bar = *q;			q++;

	out->latitude = *q;				q++;
	out->longitude = *q;			q++;
	out->altitude = *q;				q++;
	out->speedKph = *q;				q++;
	out->PDOP = *q;					q++;


	unsigned int * p = (unsigned int *)q;
	out->time = *p;					p++;


	char* c = (char*)p;
	out->status = *c;				c++;
	out->NS = *c;					c++;
	out->EW = *c;					c++;

	unsigned char* s = (unsigned char*)c;
	out->satellitesNum = *s;		s++;

	return (uint32_t)q - (uint32_t)in;
}

const uint16_t SOD = 0x12BC;
const uint16_t EOD = 0x34DE;

uint8_t data[64];
uint16_t sod = 0x0000;
uint16_t eod = 0x0000;
bool started = false;
uint8_t c = 0;


inline void ReadSerial(uint8_t in)
{
	if (!started)
	{
		sod = (sod << 8) | in;

		if (sod == SOD)
		{
			started = true;
			c = 0;
			//printf("start\r\n");
		}
	}
	else
	{
		eod = (eod << 8) | in;

		if (eod == EOD)
		{
			started = false;
			//printf("end\r\n");
			deserialize(data, &OSD_Data);
			//printf("%f", OSD_Data.V);
		}
		else
		{
			data[c++] = in;
			if (c > 64)
			{
				started = false;
			}

		}

	}
}


void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef * huart)
{
	if (huart->Instance != huart2.Instance) return;

	//memcpy(rx_part_buf, rx_buf, buf_shift);

	for (uint8_t i = 0; i < buf_shift; ++i) {
		ReadSerial(rx_buf[i]);
	}



	//gpsUpdateData(_gps->pre_buf);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart)
{
	if (huart->Instance != huart2.Instance) return;

	//memcpy(rx_part_buf, rx_buf + buf_shift, buf_shift);

	for (uint8_t i = buf_shift; i < buf_shift * 2; ++i) {
		ReadSerial(rx_buf[i]);
		//printf("%u\r\n", rx_buf[i]);
	}


	//size_t bs = sizeof(_gps->rx_buf) / 2;
	//memcpy(_gps->pre_buf, _gps->rx_buf + bs, bs);

	//gpsUpdateData(_gps->pre_buf);
}

