#include <gpio.h>
#include <spi.h>
#include <Adafruit_GFX_Library/gfxfont.h>
#include <CVBS/CVBS.h>
#include "Roboto_Mono.h"
#include "Display.h"
#include <Input/Input.h>

#include <math.h>

#define WIDTH 56*8
#define HEIGHT 283
#define RWidth 55*8
#define RHeight 273

#include "vector.h"

#include "bmp.h"
//#include "bitmaps/qr.h"
#include "bitmaps/gps.h"
#include "bitmaps/power.h"


CVBS tv(WIDTH, HEIGHT);

vec2 top(0, -1);
vec2 right(1, 0);

vec2 speed;
vec2 altitude;
vec2 gps;
vec2 power;

vec2 compass_from;
vec2 compass_to;
vec2 compass_1;

int16_t cx, cy;
uint16_t cw, ch;

void Display_Init() {
	tv.init();


	tv.setFont(&Roboto_Mono_Light_12);

	//tv.drawBitmap(200, 50, (const uint8_t *)pgm_read_pointer(&Bitmap_qr.buf), pgm_read_word(&Bitmap_qr.width), pgm_read_word(&Bitmap_qr.height), 1);
	//tv.drawBitmap(200, 50, (const uint8_t *)((void *)(*(const unsigned long *)(&Bitmap_qr.buf))), (*(const unsigned short *)(&Bitmap_qr.width)), (*(const unsigned short *)(&Bitmap_qr.height)), 1);

	tv.drawRect(0, 0, RWidth, RHeight, 1);

	cx = RWidth / 5;
	cy = RHeight / 4;
	cw = 5;
	ch = RHeight / 2;

	speed.x = RWidth / 5 - 24 - 8 * 3;
	speed.y = RHeight / 2;

	altitude.x = RWidth - RWidth / 5 + 24;
	altitude.y = RHeight / 2;

	gps.x = RWidth - 8*3;
	gps.y = 8;

	power.x = RWidth - 8*8;
	power.y = RHeight - 50;

	compass_from.x = RWidth / 2 - 8 * 14;
	compass_from.y = 8;

	compass_to.x = RWidth / 2 + 8 * 14;
	compass_to.y = compass_from.y + 11 * 2;

	compass_1.x = 1;
	compass_1.y = 1;


	tv.drawFastVLine(cx - 1, cy, ch, 1);

	for (int i = 0; i < 10; i++)
		tv.drawFastHLine(cx - cw - 1, cy + i * ch / 10 + ch / 20, cw, 1);


	tv.drawFastVLine(RWidth - cx, cy, ch, 1);

	for (int i = 0; i < 10; i++)
		tv.drawFastHLine(RWidth - cx, cy + i * ch / 10 + ch / 20, cw, 1);

	//compass
	//tv.drawRectV(compass_from - compass_1, compass_to + compass_1, 1);
	tv.drawFastHLine(compass_from.x, compass_from.y - 1, compass_to.x - compass_from.x, 1);


	//GPS
	tv.drawBitmap(gps.x - 8 - pgm_read_word(&Bitmap_gps.width), gps.y - 3, (const uint8_t *)pgm_read_pointer(&Bitmap_gps.buf), pgm_read_word(&Bitmap_gps.width), pgm_read_word(&Bitmap_gps.height), 1);

	tv.setCursor(gps.x - 8*5, gps.y + 25);
	tv.print("PDOP"); //PDOP


	tv.drawRect(speed.x - 2, speed.y - 2, 8*3 + 4, 11 + 4, 1);
	tv.setCursor(speed.x, speed.y + 25);
	tv.print("kph");

	tv.drawRect(altitude.x - 2, altitude.y - 2, 8*3 + 4, 11 + 4, 1);
	tv.setCursor(altitude.x + 8, altitude.y + 25);
	tv.print("m");

	// Power
	tv.drawBitmap(power.x + 4 - pgm_read_word(&Bitmap_power.width), power.y, (const uint8_t *)pgm_read_pointer(&Bitmap_power.buf), pgm_read_word(&Bitmap_power.width), pgm_read_word(&Bitmap_power.height), 1);

	tv.drawLine(power.x - 6, power.y + 18, power.x - 3, power.y + 18, 1); //little line on top of battery status so it look like battery
}

void Display_Draw(unsigned long lftime) {

	tv.setCursor(8, RHeight - 48 + 10);
	tv.clearRectFast(8, RHeight - 48, 8*5, 11);
	tv.printf("p %3.0f", OSD_Data.pitch);

	tv.setCursor(8, RHeight - 48 + 25);
	tv.clearRectFast(8, RHeight - 48 + 15, 8*5, 11);
	tv.printf("y %3.0f", OSD_Data.yaw);

	tv.setCursor(8, RHeight - 48 + 40);
	tv.clearRectFast(8, RHeight - 48 + 30, 8*5, 11);
	tv.printf("r %3.0f", OSD_Data.roll);


	tv.setCursor(80, RHeight - 48 + 10);
	tv.clearRectFast(80, RHeight - 48, 8*5, 11);
	tv.printf("%lu", lftime);


	tv.setCursor(speed.x, speed.y + 10 - 16);
	tv.clearRectFast(speed.x, speed.y- 16, 8*3, 11);
	tv.printf("%3.0f", OSD_Data.speedKph);

	tv.setCursor(speed.x, speed.y + 10);
	tv.clearRectFast(speed.x, speed.y, 8*3, 11);
	tv.printf("%3.0f", OSD_Data.speedKph_bar);


	tv.setCursor(altitude.x, altitude.y + 10 - 16);
	tv.clearRectFast(altitude.x, altitude.y - 16, 8*3, 11);
	tv.printf("%3.0f", OSD_Data.altitude);

	tv.setCursor(altitude.x, altitude.y + 10);
	tv.clearRectFast(altitude.x, altitude.y, 8*3, 11);
	tv.printf("%3.0f", OSD_Data.altitude_bar);


	/*GPS */
	tv.setCursor(gps.x, gps.y + 10);
	tv.clearRectFast(gps.x, gps.y, 8*2, 11);
	//tv.print(OSDData.satellitesNum);
	tv.printf("%u", OSD_Data.satellitesNum);

	tv.setCursor(gps.x, gps.y + 25);
	tv.clearRectFast(gps.x, gps.y + 15, 8*2, 11);
	tv.printf("%2.0f", OSD_Data.PDOP); //PDOP

	tv.setCursor(RWidth / 2 - 8 * 10, RHeight - 18 + 10);
	tv.clearRectFast(RWidth / 2 - 8 * 10, RHeight - 18, 8*21, 11);
	tv.printf("%c%2.6f %c%2.6f", OSD_Data.NS, OSD_Data.latitude, OSD_Data.EW, OSD_Data.longitude);
	/*GPS end*/


	tv.setCursor(power.x + 8, power.y + 10);
	tv.clearRectFast(power.x + 8, power.y, 8*5, 11);
	tv.printf("%2.2fV", OSD_Data.V);

	tv.setCursor(power.x + 8, power.y + 25);
	tv.clearRectFast(power.x + 8, power.y + 15, 8*5, 11);
	tv.printf("%2.2fA", OSD_Data.I);

	tv.setCursor(power.x + 8, power.y + 40);
	tv.clearRectFast(power.x + 8, power.y + 30, 8*6, 11);
	tv.printf("%3.1fW", OSD_Data.P);


	float vy = (constrain(OSD_Data.V, 9.9, 12.6) - 9.9) / 2.7 * 21;


	tv.clearRectFast(power.x - 8, power.y + 19, 8, 11 * 2);
	tv.drawRect(power.x - 8, power.y + 19, 8, 11 * 2, 1);
	tv.fillRectFast(power.x - 8, power.y + 19 + 22 - vy, 8, vy, 1);

/*
		tv.setCursor(gps_sat.x, gps_sat.y + 40);
		tv.clearRectFast(gps.x, gps.y + 30, 8*2, 11);
		tv.printf("%.0f", OSD_Data.V); //VDOP*/

	tv.waitLine((int)(cy + ch));

	tv.enableClip(cx, cy, cx + RWidth / 5 * 3, cy + ch);
		tv.clearRectFast(cx, cy, RWidth / 5 * 3, ch+1);

		vec2 center(RWidth / 2, RHeight / 2);

		tv.drawRect(RWidth / 2 - 5, RHeight / 2 - 5, 10, 10, 1);

		vec2 pitch_px(0, RHeight / 90);

		vec2 horizon(70, 0);
		vec2 horizon_s(0, -5);

		horizon.rotate(-OSD_Data.roll);
		horizon_s.rotate(-OSD_Data.roll);
		pitch_px.rotate(-OSD_Data.roll);


		//tv.drawLine(RWidth / 2 - horizon.x, RHeight / 2 - horizon.y, RWidth / 2 + horizon.x, RHeight / 2 + horizon.y, 1);
		tv.drawLineV(center + horizon / 2, center + horizon, 1);
		tv.drawLineV(center + horizon, center + horizon + horizon_s, 1);

		tv.drawLineV(center - horizon / 2, center - horizon, 1);
		tv.drawLineV(center - horizon, center - horizon + horizon_s, 1);

		tv.setCursorV(center + horizon * 1.1);
		//tv.print("0");
		tv.printf("%3.0f", OSD_Data.pitch);

		//vec2 horizon10 = pitch_px * 10;

		for (int i = -90; i <= 90; i += 15) {
			if (abs(OSD_Data.pitch - i) <= 40) {
				vec2 pitch(pitch_px.x * (OSD_Data.pitch - i), pitch_px.y * (OSD_Data.pitch - i));

				tv.drawLineV(center + horizon / 3 + pitch, center - horizon / 3 + pitch, 1);

				if (abs(OSD_Data.pitch - i) >= 3) {
					tv.setCursorV(center + horizon / 3 * 1.1 + pitch);
					tv.print(i);
				}
			}
		}
	tv.disableClip();



	tv.waitLine((int)compass_to.y);
	//tv.enableClipV(compass_from, compass_to);
	tv.enableClip(compass_from.x, compass_from.y, compass_to.x - 1, compass_to.y);
		tv.clearRectFastV(compass_from, compass_to + right * 8);
		//tv.clearRectFast(compass_from.x, compass_from.y,compass_to.x, compass_to.y);

		vec2 center_top(RWidth / 2, compass_from.y);
		vec2 compass_line(0, 5);
		vec2 yaw_px(RWidth / 90, 0);

		tv.drawLineV(center_top, center_top + compass_line * 1.5, 1);

		float yaw_div = 10;
		//104
		int yaw_f = floor(OSD_Data.yaw / yaw_div) * yaw_div; //100

		float yaw = OSD_Data.yaw - yaw_f; // 4

		for (int i = -30; i <= 30; i += 10) {
			tv.drawLineV(center_top - yaw_px * (i + yaw), center_top + compass_line - yaw_px * (i + yaw), 1);

			vec2 n_add(3,0);

			int p_yaw = yaw_f - i;

			if (p_yaw >= 10) {
				n_add.x = 7;
			} else if (p_yaw >= 100) {
				n_add.x = 11;
			}

			tv.setCursorV(center_top + compass_line * 4 - yaw_px * (i + yaw) - n_add);
			tv.print(p_yaw);
		}

	tv.disableClip();


}



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (!tv._initialized) return; //prevents uart1 stuck

	if (GPIO_Pin == GPIO_PIN_4)
	{
		tv.handleCSYNC();

	}

	if (GPIO_Pin == GPIO_PIN_6)
	{
		tv.handleVSYNC();
	}
}
