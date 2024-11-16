//
// Created by 86157 on 2024/11/14.
//

#ifndef HAL_WDGT_H
#define HAL_WDGT_H

#include "gd32f4xx.h"

void hal_fwdgt_init(uint32_t reloadValue);
void hal_fwdgt_reload(void);

#endif //HAL_WDGT_H
