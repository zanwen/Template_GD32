#ifndef __INT_DHT11_H__
#define __INT_DHT11_H__

#include "gd32f4xx.h"
typedef uint8_t u8;

// DHT: Digital Humidity and Temperature
typedef struct {
    float temperature;
    float humidity;
} Struct_DHT_Data;

void Int_DHT11_Init();
char Int_DHT11_GetData(u8* dat);
void Int_DHT11_Convert(u8 *dat, Struct_DHT_Data* st_dat);

#endif /* __INT_DHT11_H__ */