#ifndef __INT_NIXIETUBE_H__
#define __INT_NIXIETUBE_H__

#include "gd32f4xx.h"

void Int_NixieTube_Init();

void Int_NixieTube_Display(uint8_t chip, uint8_t num);

void Int_NixieTube_SetNum(long int num);
void Int_NixieTube_SetStr(char* str);
void Int_NixieTube_Refresh();

#endif