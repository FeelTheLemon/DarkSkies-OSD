#ifndef INPUT_H_
#define INPUT_H_

#ifdef __cplusplus
 extern "C" {
#endif

struct sOSD_Data
{
	float pitch, yaw, roll;

	float V, I, P;

	float altitude_bar, speedKph_bar;

	float latitude, longitude, altitude, speedKph, PDOP;
	unsigned int time;
	char status, NS, EW;
	unsigned char satellitesNum;
};
extern struct sOSD_Data OSD_Data;

void Input_Init(void);

#ifdef __cplusplus
}
#endif

#endif
