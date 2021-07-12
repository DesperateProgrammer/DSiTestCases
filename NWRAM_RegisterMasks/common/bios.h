// bios functions

#pragma once
#include <stdint.h>
#include <stdbool.h>

/** timing */
void biosWaitByLoop(uint32_t cycles) ;

/** SHA Bios Functions */
bool biosSHA1Init(void *shaContext) ;
bool biosSHA1Update(void *shaContext, uint8_t *src, uint32_t srclen) ;
bool biosSHA1Finish(uint8_t *dst, void *shaContext) ; // These arguments seem off
bool biosSHA1InitUpdateFin(uint8_t *dst, uint8_t *src, uint32_t srclen) ;

typedef struct SRSAHEAPINFO
{
  uint32_t start ;
  uint32_t end ;
  uint32_t size ;
} *SRSAHEAPINFO ;

/** RSA Bios Functions */
bool biosRSAInitHeap(SRSAHEAPINFO *heapInfo, uint8_t *heap, uint32_t heapSize) ;
bool biosRSADecryptUnpad(void *heapInfo, uint8_t *dest, uint8_t *src, uint8_t *key) ;
bool biosRSADecryptUnpadOpenPGP(void *heapInfo, uint8_t *dest, uint8_t *src, uint8_t *key) ;

/** Unknown */
bool biosUnkown_28() ;  // Suspect for an SHA1 function