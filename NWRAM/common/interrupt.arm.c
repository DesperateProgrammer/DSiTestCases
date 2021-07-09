#include "Interrupt.h"

__attribute__((naked)) uint32_t CpuDisableIRQ()
{
  __asm(
    "MRS r0, cpsr         \n"
    "ORR r1, r0, #0x80    \n"
    "MSR cpsr, r1         \n"
    "AND r0, r0, #0x80    \n"
    "BX lr                \n"
  ); 
}

__attribute__((naked)) uint32_t CpuEnableIRQ()
{
  __asm(
    "MRS r0, cpsr         \n"
    "BIC r1, r0, #0x80    \n"
    "MSR cpsr, r1         \n"
    "AND r0, r0, #0x80    \n"
    "BX lr                \n"
  ); 
}

__attribute__((naked)) uint32_t CpuSetIRQBit(uint32_t bit)
{
  __asm(
    "MRS r1, cpsr         \n"
    "BIC r2, r0, #0x80    \n"
    "ORR r2, r1, r0       \n"
    "MSR cpsr, r2         \n"
    "AND r0, r1, #0x80    \n"
    "BX lr                \n"
  ); 
}

__attribute__((naked)) uint32_t CpuDisableIRQAndFIQ()
{
  __asm(
    "MRS r0, cpsr         \n"
    "ORR r1, r0, #0xC0    \n"
    "MSR cpsr, r1         \n"
    "AND r0, r0, #0xC0    \n"
    "BX lr                \n"
  ); 
}

__attribute__((naked)) uint32_t CpuSetIRQAndFIQBit(uint32_t bits)
{
  __asm(
    "MRS r1, cpsr         \n"
    "BIC r2, r0, #0xC0    \n"
    "ORR r2, r2, r0       \n"
    "MSR cpsr, r2         \n"
    "AND r0, r1, #0xC0    \n"
    "BX lr                \n"
  ); 
}

__attribute__((naked)) uint32_t CpuGetIRQBit()
{
  __asm(
    "MRS r0, cpsr         \n"
    "AND r0, r0, #0x80    \n"
    "BX lr                \n"
  ); 
}

__attribute__((naked)) uint32_t CpuCPSRMode()
{
  __asm(
    "MRS r0, cpsr         \n"
    "AND r0, r0, #0x1F    \n"
    "BX lr                \n"
  ); 
}

__attribute__((naked)) void CpuWaitForInterrupt() 
{
  __asm(
    "MOV r0, #0x00                \n"
    "MCR 15,0x0,r0,cr7,cr0,0x4    \n"
    "BX lr                        \n"
  ); 
}
