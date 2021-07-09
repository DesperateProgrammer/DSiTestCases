// bios

#include "bios.h"

__attribute__((naked)) void biosWaitByLoop(uint32_t cycles)
{
  __asm(
    "svc 0x03       \n"
    "bx lr          \n"
  ) ;
}

__attribute__((naked)) bool biosSHA1Init(void *shaContext) 
{
  __asm(
    "mov r3, #0x60  \n"
    "add r3, r0     \n"
    "mov r2, #0     \n"
    "str r2, [r3]   \n"
    "svc 0x24       \n"
    "bx lr          \n"
  ) ;
}

__attribute__((naked)) bool biosSHA1Update(void *shaContext, uint8_t *src, uint32_t srclen) 
{
  __asm(
    "svc 0x25       \n"
    "bx lr          \n"
  ) ;
}

__attribute__((naked)) bool biosSHA1Finish(uint8_t *dst, void *shaContext) 
{
  __asm(
    "mov r12, r1    \n"
    "add r1, r0, #0 \n"
    "mov r0, r12    \n"
    "svc 0x26       \n"
    "bx lr          \n"
  );
}

__attribute__((naked)) bool biosSHA1InitUpdateFin(uint8_t *dst, uint8_t *src, uint32_t srclen) 
{
  __asm(
    "svc 0x27       \n"
    "bx lr          \n"
  ) ;
}

__attribute__((naked)) bool biosUnkown_28() 
{
  __asm(
    "svc 0x28       \n"
    "bx lr          \n"
  ) ;
}

__attribute__((naked)) bool biosRSADecryptUnpad(void *heapInfo, uint8_t *dest, uint8_t *src, uint8_t *key) 
{
  __asm(
    "svc 0x22       \n"
    "bx lr          \n"
  ) ;
}

__attribute__((naked)) bool biosRSADecryptUnpadOpenPGP(void *heapInfo, uint8_t *dest, uint8_t *src, uint8_t *key) 
{
  __asm(
    "svc 0x23       \n"
    "bx lr          \n"
  ) ;
}



