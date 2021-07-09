#pragma once

#include "stdint.h"

#define WRAM_TYPE_SETA            0
#define WRAM_TYPE_SETB            1
#define WRAM_TYPE_SETC            2
#define WRAM_TYPE_OLD             3
#define WRAM_TYPE_STATIC          4
#define WRAM_TYPE_NONE            5

typedef struct SWRAMPOSTRESULTS
{
  uint32_t       wramReadPrioity[6] ;
  uint32_t       wramWritePrioity[6] ;
  uint8_t        wramReadBankPriority[8] ;
  uint8_t        wramWriteBankPriority[8] ;
} SWRAMPOSTRESULTS ;

void WRAMSelfTest(SWRAMPOSTRESULTS *post) ;
