#ifndef __INT_MATRIXKEY_H__
#define __INT_MATRIXKEY_H__

#include "gd32f4xx.h"

typedef void (*MatrixKey_Callback)(uint8_t row, uint8_t col);

void Int_MatrixKey_Init();
// return the number of pressed key
char Int_MatrixKey_Detect();
void Int_MatrixKey_GetPressedKeys(uint8_t* keybuf, uint8_t* sizebuf);

void Int_MatrixKey_RegisterKeyDownCallabck(MatrixKey_Callback callback);
void Int_MatrixKey_RegisterKeyUpCallabck(MatrixKey_Callback callback);

#endif /* __INT_MATRIXKEY_H__ */