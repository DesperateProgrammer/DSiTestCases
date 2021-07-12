#include "interrupt.h"

#define ADDR_IME      0x04000208
#define ADDR_IE       0x04000210
#define ADDR_IF       0x04000214

typedef struct SPROXYINTERRUPT      // used to call a subscriber of an interrupt
                                    // used by the system itself
{
  InterruptProxyRoutineFunc mRoutine ;
  uint32_t                  mInstalled ;
  uint32_t                  mContextLink ;
} SPROXYINTERRUPT ;

SPROXYINTERRUPT InterruptProxyTable[12] ;
const uint16_t InterruptProxyTranslationTable[12] 
                = { 8, 9, 10, 11, 28, 29, 30, 31, 3, 4, 5, 6 } ;

void IsrGeneric(uint8_t number)
{
  InterruptProxyRoutineFunc proxyRoutine = InterruptProxyTable[number].mRoutine ;
  uint32_t link = InterruptProxyTable[number].mContextLink ;
  uint32_t installed = InterruptProxyTable[number].mInstalled ;
  uint16_t interruptId = InterruptProxyTranslationTable[number] ;
  InterruptProxyTable[number].mRoutine = 0 ;
  if (proxyRoutine != 0)
  {
    proxyRoutine(link) ;
  }
  // TODO: There is a flag set in a global var
  if (installed)
    return ;
  InterruptSourceDisable(1 << interruptId) ;
}

void isrDummy()
{
  // Nothing to do here
}

#define ISRGENERIC(name, n)  void name() { IsrGeneric(n) ; }

ISRGENERIC(isrDMA0, 0) 
ISRGENERIC(isrDMA1, 1) 
ISRGENERIC(isrDMA2, 2) 
ISRGENERIC(isrDMA3, 3) 
ISRGENERIC(isrNDMA0, 4) 
ISRGENERIC(isrNDMA1, 5) 
ISRGENERIC(isrNDMA2, 6) 
ISRGENERIC(isrNDMA3, 7) 
ISRGENERIC(isrTimer0, 8) 
ISRGENERIC(isrTimer1, 9) 
ISRGENERIC(isrTimer2, 10) 
ISRGENERIC(isrTimer3, 11) 

InterruptRoutineFunc  InterruptHandlerTable[32] =
{
  isrDummy,
  isrDummy,
  isrTimer0,
  isrTimer1,
  isrTimer2,
  isrTimer3,
  isrDummy,
  isrDMA0,
  isrDMA0,
  isrDMA1,
  isrDMA3,
  isrDummy,
  isrDummy,
  isrDummy,
  isrDummy,
  isrDummy,
  isrDummy,
  isrDummy,
  isrDummy,
  isrDummy,
  isrDummy,
  isrDummy,
  isrDummy,
  isrDummy,
  isrDummy,
  isrDummy,
  isrDummy,
  isrNDMA0,
  isrNDMA0,
  isrNDMA1,
  isrNDMA3
} ;


uint32_t InterruptSourceDisable(uint32_t mask)
{
  uint32_t ime = *(volatile uint32_t *)ADDR_IME ;
  *(volatile uint32_t *)ADDR_IME = 0 ;
  uint32_t oldIE = *(volatile uint32_t *)ADDR_IE ;
  *(volatile uint32_t *)ADDR_IE = oldIE & ~mask ;
  *(volatile uint32_t *)ADDR_IME = ime ;
  return oldIE ;
}

uint32_t InterruptSourceEnable(uint32_t mask)
{
  uint32_t ime = *(volatile uint32_t *)ADDR_IME ;
  *(volatile uint32_t *)ADDR_IME = 0 ;  
  uint32_t oldIE = *(volatile uint32_t *)ADDR_IE ;
  *(volatile uint32_t *)ADDR_IE = oldIE | mask ;
  *(volatile uint32_t *)ADDR_IME = ime ;
  return oldIE ;
}

uint32_t InterruptSourceAck(uint32_t mask)
{
  uint32_t ime = *(volatile uint32_t *)ADDR_IME ;
  *(volatile uint32_t *)ADDR_IME = 0 ;
  uint32_t oldIF = *(volatile uint32_t *)ADDR_IF ;
  *(volatile uint32_t *)ADDR_IF = mask ;
  *(volatile uint32_t *)ADDR_IME = ime ;
  return oldIF ;
}

uint32_t InterruptMasterEnable()
{
  return InterruptMasterSet(1) ;
}

uint32_t InterruptMasterDisable() 
{
  return InterruptMasterSet(0) ;
}

uint32_t InterruptGetMatserEnable()
{
  return *(volatile uint32_t *)ADDR_IME ;
}

uint32_t InterruptMasterSet(uint32_t value) 
{
  uint32_t old = *(volatile uint32_t *)ADDR_IME ;
  *(volatile uint32_t *)ADDR_IME = value ;
  return old ;
}

uint32_t InterruptGetFlagged() 
{
  return *(volatile uint32_t *)ADDR_IF ;
}

uint32_t InterruptGetEnabled() 
{
  return *(volatile uint32_t *)ADDR_IME ;
}

void CpuHalt() 
{
  CpuDisableIRQ() ;
  while (true)
  {
    CpuWaitForInterrupt() ;
  }
}

void InterruptAcknowledge(uint32_t mask) 
{
  *(volatile uint32_t *)ADDR_IF = mask ;
}

void InterruptHandler() 
{
  if (!InterruptGetMatserEnable())
    return ;
  uint32_t pendingInterrupts = InterruptGetEnabled() & InterruptGetFlagged() ;
  if (pendingInterrupts)
  {
    for (int source=0;source<32;source++)
    {
      if (pendingInterrupts & (1 << source))
      {
        InterruptAcknowledge(1 << source) ;
        InterruptHandlerTable[source]() ;
      }
    }
  }
}

void InterruptSetRoutineForMask(uint32_t mask, InterruptProxyRoutineFunc routine)
{
  for (uint32_t i = 0; i < 32; i++)
  {
    if (mask & 1)
    {
      switch (i)
      {
        case INTERRUPT_SOURCE_ID_DMA0:
        case INTERRUPT_SOURCE_ID_DMA1:
        case INTERRUPT_SOURCE_ID_DMA2:
        case INTERRUPT_SOURCE_ID_DMA3:
          InterruptProxyTable[i - INTERRUPT_SOURCE_ID_DMA0 + 0].mRoutine = routine ;
          InterruptProxyTable[i - INTERRUPT_SOURCE_ID_DMA0 + 0].mInstalled = true ;
          InterruptProxyTable[i - INTERRUPT_SOURCE_ID_DMA0 + 0].mContextLink = 0 ;
          break;
        case INTERRUPT_SOURCE_ID_NDMA0:
        case INTERRUPT_SOURCE_ID_NDMA1:
        case INTERRUPT_SOURCE_ID_NDMA2:
        case INTERRUPT_SOURCE_ID_NDMA3:
          InterruptProxyTable[i - INTERRUPT_SOURCE_ID_NDMA0 + 4].mRoutine = routine ;
          InterruptProxyTable[i - INTERRUPT_SOURCE_ID_NDMA0 + 4].mInstalled = true ;
          InterruptProxyTable[i - INTERRUPT_SOURCE_ID_NDMA0 + 4].mContextLink = 0 ;
          break;
        case INTERRUPT_SOURCE_ID_TIMER0:
        case INTERRUPT_SOURCE_ID_TIMER1:
        case INTERRUPT_SOURCE_ID_TIMER2:
        case INTERRUPT_SOURCE_ID_TIMER3:
          InterruptProxyTable[i - INTERRUPT_SOURCE_ID_TIMER0 + 8].mRoutine = routine ;
          InterruptProxyTable[i - INTERRUPT_SOURCE_ID_TIMER0 + 8].mInstalled = true ;
          InterruptProxyTable[i - INTERRUPT_SOURCE_ID_TIMER0 + 8].mContextLink = 0 ;
          break;
      }
    }
    mask >>= 1 ;
  } 
}

void InterruptInitialize() 
{
#ifdef ARM9
 *(volatile uint32_t *)0x02ff3ffc = (uint32_t)&InterruptHandler ;
#endif
#ifdef ARM7
 *(volatile uint32_t *)0x0380fffC = (uint32_t)&InterruptHandler ;
#endif
}
