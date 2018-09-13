#ifndef DISPLAY_H_
#define DISPLAY_H_

#ifdef __cplusplus
 extern "C" {
#endif

void Display_Init();
void Display_Draw(unsigned long lftime);

void Display_CSYNC(void);
void Display_VSYNC(void);

#ifdef __cplusplus
}
#endif

#endif /* DISPLAY_H_ */
