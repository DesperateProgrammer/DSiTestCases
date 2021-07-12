// Interrupt management 
//

#pragma once

#include <stdint.h>
#include <stdbool.h>

#define INTERRUPT_SOURCE_ID_VBLANK          0
#define INTERRUPT_SOURCE_ID_HBLANK          1
#define INTERRUPT_SOURCE_ID_VCOUNTER        2
#define INTERRUPT_SOURCE_ID_TIMER0          3
#define INTERRUPT_SOURCE_ID_TIMER1          4
#define INTERRUPT_SOURCE_ID_TIMER2          5
#define INTERRUPT_SOURCE_ID_TIMER3          6
#define INTERRUPT_SOURCE_ID_SIO             7
#define INTERRUPT_SOURCE_ID_DMA0            8
#define INTERRUPT_SOURCE_ID_DMA1            9
#define INTERRUPT_SOURCE_ID_DMA2            10
#define INTERRUPT_SOURCE_ID_DMA3            11
#define INTERRUPT_SOURCE_ID_KEYPAD          12
#define INTERRUPT_SOURCE_ID_GBASLOT         13
#define INTERRUPT_SOURCE_ID_GAMESLOT0       14
#define INTERRUPT_SOURCE_ID_GAMESLOT1       15
#define INTERRUPT_SOURCE_ID_IPCSYNC         16
#define INTERRUPT_SOURCE_ID_IPCTXEMPTY      17
#define INTERRUPT_SOURCE_ID_IPCRXNOTEMPTY   18
#define INTERRUPT_SOURCE_ID_GAMEPACK_READY  19
#define INTERRUPT_SOURCE_ID_GAMEPACK_EXT    20
#define INTERRUPT_SOURCE_ID_GEOMETRYFIFO    21
#define INTERRUPT_SOURCE_ID_UNFOLD          22
#define INTERRUPT_SOURCE_ID_SPI             23
#define INTERRUPT_SOURCE_ID_WIFI            24
#define INTERRUPT_SOURCE_ID_DSP             24
#define INTERRUPT_SOURCE_ID_CAM             25
#define INTERRUPT_SOURCE_ID_26              26
#define INTERRUPT_SOURCE_ID_27              27
#define INTERRUPT_SOURCE_ID_NDMA0           28
#define INTERRUPT_SOURCE_ID_NDMA1           29
#define INTERRUPT_SOURCE_ID_NDMA2           30
#define INTERRUPT_SOURCE_ID_NDMA3           31

typedef void (* InterruptRoutineFunc)(void) ;
typedef void (* InterruptProxyRoutineFunc)(uint32_t contextLink) ;

/** Disable IRQs by setting the IRQ Mask in cpsr. Return the previous IRQ Mask Bit */
uint32_t CpuDisableIRQ();

/** Enable IRQs by clearing the IRQ Mask in cpsr. Return the previous IRQ Mask Bit */
uint32_t CpuEnableIRQ();

/** Sets the IRQ Mask in cpsr. Return the previous IRQ Mask Bit */
uint32_t CpuSetIRQBit(uint32_t bit);

/** Disable IRQs by setting the IRQ + FIQ Mask in cpsr. Return the previous IRQ + FIQ Mask Bit */
uint32_t CpuDisableIRQAndFIQ();

/** Sets the IRQ + FIQ Mask in cpsr. Return the previous IRQ + FIQ Mask Bit */
uint32_t CpuSetIRQAndFIQBit(uint32_t bits);

/** Get the status of the IRQ Bit without altering it */
uint32_t CpuGetIRQBit();

/** Get CPSR Mode */
uint32_t CpuCPSRMode();

/** Use the cp15 to wait for interrupt */
void CpuWaitForInterrupt() ;

/** halt system, does not return! */
void CpuHalt() ;

uint32_t InterruptMasterEnable() ;
uint32_t InterruptMasterDisable() ;
uint32_t InterruptMasterSet(uint32_t value) ;
uint32_t InterruptGetMatserEnable() ;

uint32_t InterruptGetFlagged() ;
void InterruptAcknowledge(uint32_t mask) ;
uint32_t InterruptGetEnabled() ;

/** Disable interrupt sources */
uint32_t InterruptSourceDisable(uint32_t mask) ;

/** Enable interrupt sources */
uint32_t InterruptSourceEnable(uint32_t mask) ;

void InterruptSetRoutineForMask(uint32_t mask, InterruptProxyRoutineFunc routine) ;

/** Acknowledge and clear pending interrupt sources */
uint32_t InterruptSourceAck(uint32_t mask) ;

/** The interrupt handler that gets called from the bios */
void InterruptHandler() ;

void InterruptInitialize() ;

