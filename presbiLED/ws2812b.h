#ifndef __WS2812B_H__
#define __WS2812B_H__

#ifdef USE_GAMMA
extern const unsigned char code gamma8[];
#define gamma(x) (gamma8[x])
#else
#define gamma(x) (x)
#endif

typedef struct { unsigned char r,g,b;} RGB;

void WS2812_SendZero(void);
void WS2812_SendOne_(void);
void WS2812_SendByte_(unsigned char);
void WS2812_Latch(void);

inline void WS2812_SendRGB(RGB*);
inline void WS2812_SendRGBData(unsigned char,unsigned char,unsigned char);
#endif
