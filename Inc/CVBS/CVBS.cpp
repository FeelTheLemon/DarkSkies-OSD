#include "stm32f1xx_hal.h"
#include <spi.h>
#include <tim.h>
#include <stdlib.h>
#include "CVBS.h"


unsigned int _WIDTH;

#define V_LINE_START 27
#define V_LINE_END 302

// dus - 1/10 us
#define dudelay(dus) for (int i = 0; i < dus; ++i) { __asm__("nop;nop;nop;nop;nop;nop;nop;"); }

volatile uint16_t _line = 0;
uint8_t* cbuf;

CVBS::CVBS(uint16_t w, uint16_t h) : Adafruit_GFX(w, h) {
    uint16_t bytes = ((w + 7) / 8) * h;
    if((buffer = (uint8_t *)malloc(bytes))) {
        memset(buffer, 0, bytes);
    }
}

CVBS::~CVBS(void) {
    if(buffer) free(buffer);
}

uint8_t* CVBS::getBuffer(void) {
    return buffer;
}

void CVBS::writeLine(void) {


	uint8_t *data = &cbuf[(_line - V_LINE_START) * ((_WIDTH + 7) / 8)];
	uint8_t size = (_WIDTH + 7) / 8;

	if (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_BUSY_TX) return;

	HAL_StatusTypeDef status = HAL_SPI_Transmit_DMA(&hspi1, data, (uint16_t)size);
	//HAL_StatusTypeDef status = HAL_SPI_Transmit(&hspi1, data, (uint16_t)size, 50000);

	if (status != HAL_OK) printf("Input_Init FAILED with status 0x%x, error: 0x%lx , state: 0x%x\r\n", status, HAL_SPI_GetError(&hspi1), HAL_SPI_GetState(&hspi1));
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == htim3.Instance)
    {
        //printf("tim3\r\n");
        //writeLine();
    }
}

void CVBS::handleCSYNC(void) {

	_line++;

	if (_line >= V_LINE_START && _line <= V_LINE_END) {

		__asm("nop;nop;nop;nop; nop;nop;nop;nop;nop;nop;nop; nop;nop;nop;nop;nop;nop;nop; nop;nop;nop;nop;nop;nop;nop; nop;nop;nop;nop;nop;nop;nop; nop;nop;nop;nop;nop;nop;nop; nop;nop;nop;nop;nop;nop;nop; nop;nop;nop;nop;nop;nop;nop; nop;nop;nop;nop;nop;nop;nop; nop;nop;nop;nop;nop;nop;nop; nop;nop;nop;nop;nop;nop;nop; nop;nop;nop;nop;nop;nop;nop; nop;nop;nop;nop;nop;nop;nop; nop;nop;nop;nop;nop;nop;nop; nop;nop;nop;nop;nop;nop;nop; nop;nop;nop;nop;nop;nop;nop; nop;nop;nop;nop;nop;nop;nop; nop;nop;nop;nop;nop;nop;nop; nop;nop;nop;nop;nop;nop;nop; nop;nop;nop;nop;nop;nop;nop; nop;nop;nop;nop;nop;nop;nop; nop;nop;nop;nop;nop;nop;nop; nop;nop;nop;nop;nop;nop;nop; nop;nop;nop;nop;nop;nop;nop; ");
		writeLine();

		//HAL_TIM_Base_Start_IT(&htim3); //not good as nop's. strange?
		//while(SPI1CMD & SPIBUSY) {}
	}
}


void CVBS::handleVSYNC(void) {
	_line = 0;
}

//void HandleEVEN(void) {}

void CVBS::init(void) {
	cbuf = buffer;
	_WIDTH = WIDTH;
	_initialized = true;
}

void CVBS::clearScreen(void) {
	memset(buffer, 0x00, (WIDTH + 7) * HEIGHT / 8);
}

void CVBS::clearRectFast(int16_t x, int16_t y, uint16_t w, uint16_t h) {
	for (int i = 0; i < h; ++i) {
		uint8_t *ptr  = &buffer[(x / 8) + (y + i) * ((WIDTH + 7) / 8)];
		memset(ptr, 0x00, ((w + 7) / 8));
	}

}

void CVBS::fillScreen(uint16_t color) {
    if(buffer) {
        uint16_t bytes = ((WIDTH + 7) / 8) * HEIGHT;
        memset(buffer, color ? 0xFF : 0x00, bytes);
    }
}

void CVBS::fillRectFast(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t color) {
	for (int i = 0; i < h; ++i)	{
		uint8_t *ptr  = &buffer[(x / 8) + (y + i) * ((WIDTH + 7) / 8)];
		memset(ptr, color ? 0xFF : 0x00, ((w + 7) / 8));
	}

}

void CVBS::delayFrames(int frames) {
	for (int i = 0; i < frames; ++i) {
		if (i + 1 < frames) {
			while (_line >= V_LINE_END) {}
		}
		while (_line <= V_LINE_END) {
			//__asm__("nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;");
		}
	}
}

void CVBS::waitLine(int line) {
	while (_line <= line + V_LINE_START);
}

void CVBS::waitFrame(void) {
	while (_line <= V_LINE_END);
}

void CVBS::enableClip(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
	cx1 = (x1 < x2) ? x1 : x2;
	cy1 = (y1 < y2) ? y1 : y2;
	cx2 = (x2 > x1) ? x2 : x1;
	cy2 = (y2 > y1) ? y2 : y1;
	clipped = true;
}

void CVBS::disableClip() {
	clipped = false;
}


void CVBS::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if(buffer) {
        if((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;

        if (clipped) {
        	if (x < cx1 || x > cx2) return;
        	if (y < cy1 || y > cy2) return;
        }


        int16_t t;
        switch(rotation) {
            case 1:
                t = x;
                x = WIDTH  - 1 - y;
                y = t;
                break;
            case 2:
                x = WIDTH  - 1 - x;
                y = HEIGHT - 1 - y;
                break;
            case 3:
                t = x;
                x = y;
                y = HEIGHT - 1 - t;
                break;
        }

        uint8_t   *ptr  = &buffer[(x / 8) + y * ((WIDTH + 7) / 8)];

        if(color) *ptr |=   0x80 >> (x & 7);
        else      *ptr &= ~(0x80 >> (x & 7));
    }
}

void CVBS::drawLineV(vec2 from, vec2 to, uint16_t color) {
	drawLine((int16_t)from.x, (int16_t)from.y, (int16_t)to.x, (int16_t)to.y, color);
}

void CVBS::drawRectV(vec2 from, vec2 to, uint16_t color) {
	drawRect((int16_t)from.x, (int16_t)from.y, (uint16_t)(to.x - from.x), (uint16_t)(to.y - from.y), color);
}

void CVBS::fillRectV(vec2 from, vec2 to, uint16_t color) {
	fillRect((int16_t)from.x, (int16_t)from.y, (uint16_t)(to.x - from.x), (uint16_t)(to.y - from.y), color);
}

void CVBS::setCursorV(vec2 to) {
	setCursor((int16_t)to.x, (int16_t)to.y);
}

void CVBS::clearRectFastV(vec2 from, vec2 to) {
	clearRectFast((int16_t)from.x, (int16_t)from.y, (int16_t)(to.x - from.x), (int16_t)(to.y - from.y));
}

void CVBS::fillRectFastV(vec2 from, vec2 to, uint16_t color) {
	fillRectFast((int16_t)from.x, (int16_t)from.y, (uint16_t)(to.x - from.x), (uint16_t)(to.y - from.y), color);
}

void CVBS::enableClipV(vec2 from, vec2 to) {
	enableClip((int16_t)from.x, (int16_t)from.y, (int16_t)to.x, (int16_t)to.y);
}
