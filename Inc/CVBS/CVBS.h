#ifndef CVBS_H_
#define CVBS_H_

#include <Adafruit_GFX_Library/Adafruit_GFX.h>
#include <Display/vector.h>

class CVBS : public Adafruit_GFX {
public:
	CVBS(uint16_t w, uint16_t h);
	~CVBS(void);

	void init();

	void writeLine(void);
	void handleCSYNC(void);
	void handleVSYNC(void);

	void drawPixel(int16_t x, int16_t y, uint16_t color);
	void clearScreen();
	void clearRectFast(int16_t x, int16_t y, uint16_t w, uint16_t h);

	void fillScreen(uint16_t color);
	void fillRectFast(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t color);

	void enableClip(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
	void disableClip();

	void drawLineV(vec2 from, vec2 to, uint16_t color);
	void drawRectV(vec2 from, vec2 to, uint16_t color);
	void fillRectV(vec2 from, vec2 to, uint16_t color);
	void setCursorV(vec2 to);
	void clearRectFastV(vec2 from, vec2 to);
	void fillRectFastV(vec2 from, vec2 to, uint16_t color);
	void enableClipV(vec2 from, vec2 to);

	uint8_t *getBuffer(void);
	void delayFrames(int frames);
	void waitLine(int line);
	void waitFrame(void);

	bool _initialized = false;

	//void drawText();
private:
	uint8_t *buffer;

	bool clipped = false;
	int16_t cx1, cy1, cx2, cy2;
};



#endif /* CVBS_H_ */
