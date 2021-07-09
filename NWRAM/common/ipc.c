// IPC related

#include "ipc.h"
#include "interrupt.h"
#include "terminate.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define ADDR_IPCSYNC          0x04000180
#define ADDR_IPCFIFOCNT       0x04000184
#define ADDR_IPCFIFOSEND      0x04000188
#define ADDR_IPCFIFORECV      0x04100000

#define REG_IPC_SYNC          (*(volatile uint32_t *)ADDR_IPCSYNC)
#define REG_IPC_FIFOCNT       (*(volatile uint32_t *)ADDR_IPCFIFOCNT)
#define REG_IPC_FIFOSEND      (*(volatile uint32_t *)ADDR_IPCFIFOSEND)
#define REG_IPC_FIFORECV      (*(volatile uint32_t *)ADDR_IPCFIFORECV)

#define FIFOCNT_SENDFIFOFULL  0x00000002
#define FIFOCNT_RECVEMPTY     0x00000100
#define FIFOCNT_ERROR         0x00004000
#define FIFOCNT_ERRORACK      0x00004000
#define FIFOCNT_ENABLE        0x00008000

typedef struct SIPCMESSAGEBOX
{
  uint32_t      writePtr ;
  uint32_t      readPtr ;
  uint8_t       ringBuffer[8] ;
  uint32_t      counterOfOnes ;
  uint32_t      unknown_14 ;
  uint32_t      unknown_18 ;
  uint32_t      unknown_20 ;
  uint32_t      unknown_24 ;
  uint8_t       unknownBuffer[12] ;
} SIPCMESSAGEBOX ;

SIPCMESSAGEBOX ipcMessageBox ;

ipcCommandHandlerFunc ipcFifoHandlerTable[32] ;

volatile uint32_t *ipcArm9RegisteredFifoHandlerFlags = (volatile uint32_t *)0x02ffff88 ;
volatile uint32_t *ipcArm7RegisteredFifoHandlerFlags = (volatile uint32_t *)0x02ffff8c ;

uint8_t GetIPCSyncValue() 
{
  return REG_IPC_SYNC & 0x0F ;
}

void SetIPCSyncValue(uint8_t value) 
{
   REG_IPC_SYNC = (REG_IPC_SYNC & ~0x00000F00) | ((value & 0x0f) << 8) ;
}

void WaitForIPCSyncValue(uint8_t value) 
{
  while (GetIPCSyncValue() != (value & 0x0f))
    ;
}

void WaitForIPCSyncValueHigh() 
{
  while ((GetIPCSyncValue() & 8) != 8)
    ;
}

// There are only few hard coded values for the first parameter found, so assume it
// is either the command or command family
// Observerd values for it:
//  0x02 - in main -> this in combination with 0x01 is some kind of message box
//  0x08
//  0x0C
//  0x0E
//  0x14 -> This is related to nandDriver
//  0x17
// At 0x14 we see that the second value is an address(divided by 32), that was previously FlushedAndInvalidated
// So this seems to be data / ptr
int32_t TrySendByFifo(uint8_t command, uint32_t data, bool flag) 
{
  if ((*(volatile uint32_t *)ADDR_IPCFIFOCNT) & FIFOCNT_ERROR)
  {
    // recover from error: Ack the error and re-enable
    *(volatile uint32_t *)ADDR_IPCFIFOCNT |= FIFOCNT_ERRORACK | FIFOCNT_ENABLE;
    return -1 ;
  }
  uint32_t prevIRQ = CpuDisableIRQ() ;
  if ((*(volatile uint32_t *)ADDR_IPCFIFOCNT) & FIFOCNT_SENDFIFOFULL)
  {
    // cand send if fifo is full
    CpuSetIRQBit(prevIRQ) ; 
    return -2 ;    
  }
  // Actually send
  *(volatile uint32_t *)ADDR_IPCFIFOSEND = (command & 0x1F) | (flag?(1<<6):0) | (data << 6) ;
 
  CpuSetIRQBit(prevIRQ) ;
  return 0 ;
}

void IpcRegisterFIFOCommandHandler(uint8_t number, ipcCommandHandlerFunc handler)
{
  uint32_t prevIRQ = CpuDisableIRQ() ;
  ipcFifoHandlerTable[number] = handler ;
  if (handler)
  {
#ifdef ARM9
    *ipcArm9RegisteredFifoHandlerFlags |= (1 << number) ;
#else
    *ipcArm7RegisteredFifoHandlerFlags |= (1 << number) ;
#endif
  } else
  {
#ifdef ARM9
    *ipcArm9RegisteredFifoHandlerFlags &= ~(1 << number) ;
#else
    *ipcArm7RegisteredFifoHandlerFlags &= ~(1 << number) ;
#endif
  }
  CpuSetIRQBit(prevIRQ) ;  
}

bool IpcIsFifoCommandHandlerregisteredForCore(uint8_t number, uint8_t core)
{
  if (core == 0)
  {
    return *ipcArm9RegisteredFifoHandlerFlags & (1 << number) ;
  } else
  {
    return *ipcArm7RegisteredFifoHandlerFlags & (1 << number) ;
  }
}

void IpcReadAndProcessFIFOValue()
{
  int status ;
  uint32_t prevIrq, savedFifoValue = 0;
  while (true)
  {
  if ((REG_IPC_FIFOCNT != 0x8501) && (REG_IPC_FIFOCNT != 0xC501))
    printf("FIFO: %04x\n", REG_IPC_FIFOCNT) ;
    // Get Value or error
    if (!(REG_IPC_FIFOCNT & FIFOCNT_ERROR))
    {
      prevIrq = CpuDisableIRQ() ;
      savedFifoValue = REG_IPC_FIFORECV ;
      if (!(REG_IPC_FIFOCNT & FIFOCNT_RECVEMPTY))
      {
        CpuSetIRQBit(prevIrq) ;
        status = 0 ;
      } else
      {
        CpuSetIRQBit(prevIrq) ;
        status = -4 ;      
      }
    } else
    {
      status = -3 ;
      REG_IPC_FIFOCNT |= FIFOCNT_ERRORACK | FIFOCNT_ENABLE ;
    }
    // break if that was the alst value
    if (status == -4)
      break ;
    if (status != -3)
    {
      if (savedFifoValue & 0x1f) 
      {
        if (!ipcFifoHandlerTable[savedFifoValue & 0x1f])
        {
          if (!(savedFifoValue & 0x20))
          {
            if (!(REG_IPC_FIFOCNT & FIFOCNT_ERROR))
            {
              savedFifoValue |= 0x20 ;
              prevIrq = CpuDisableIRQ() ;
              if (!(REG_IPC_FIFOCNT & FIFOCNT_SENDFIFOFULL))
              {
                REG_IPC_FIFOSEND = savedFifoValue ;
                CpuSetIRQBit(prevIrq) ;
              } else
              {
                CpuSetIRQBit(prevIrq) ;
              }
            } else
            {
              REG_IPC_FIFOCNT |= FIFOCNT_ERRORACK | FIFOCNT_ENABLE ;
            }
          }
        } else
        {
          ipcFifoHandlerTable[savedFifoValue & 0x1f](savedFifoValue & 0x1f, savedFifoValue >> 6, savedFifoValue & 0x20) ;
        }
      }
    }
  }
}

void IpcSendToMessageBox(uint8_t msg)
{
  while (TrySendByFifo(2, msg, false) < 0) 
    ;
}

uint8_t IpcGetMessageBoxUsage()
{
    uint32_t prevIrq = CpuDisableIRQ() ;
    uint8_t count = ((ipcMessageBox.writePtr - ipcMessageBox.readPtr) + sizeof(ipcMessageBox.ringBuffer)) % sizeof(ipcMessageBox.ringBuffer) ;
    CpuSetIRQBit(prevIrq) ;
}

uint8_t IpcConsumeFromMessageBox()
{
  uint32_t prevIrq ;
  uint8_t result ;
  while (true)
  {
    IpcReadAndProcessFIFOValue() ;
    prevIrq = CpuDisableIRQ() ;
    if (ipcMessageBox.readPtr != ipcMessageBox.writePtr)
      break ;
    CpuSetIRQBit(prevIrq) ;
  }
  // we got at least one entry into the message box here
  result = ipcMessageBox.ringBuffer[ipcMessageBox.readPtr] ;
  ipcMessageBox.readPtr = (ipcMessageBox.readPtr + 1) & 7 ;
  CpuSetIRQBit(prevIrq) ;
  return result ;
}

void IpcSendOneToMessageBox()
{
  IpcSendToMessageBox(1) ;
}

void IpcFifoCommandHandler_01(uint8_t commandId, uint32_t data, bool flag)
{
  if (ipcMessageBox.unknown_14 == 0)
  {
    ipcMessageBox.unknown_14 = data ;
    ipcMessageBox.unknown_18 = 0 ;
    return ;
  }
  if (ipcMessageBox.unknown_18 < ipcMessageBox.unknown_14)
  {
    uint32_t localValue = data ;
    uint8_t *pLocalValue = &data;
    for (int i=0;i<2;i++)
    {
      if (ipcMessageBox.unknown_18 >= ipcMessageBox.unknown_14)
        break ;
      uint32_t prevUnknown18 = ipcMessageBox.unknown_18 ;
      ipcMessageBox.unknown_18++ ;
      ipcMessageBox.unknownBuffer[prevUnknown18-8] = *pLocalValue ;
      pLocalValue++ ;
    }
  }
}

void IpcFifoCommandHandler_02(uint8_t commandId, uint32_t data, bool flag) 
{
  if (data != 1)
  {
    uint8_t nextWritePtr = (ipcMessageBox.writePtr + 1) & 7 ;
    if (nextWritePtr != ipcMessageBox.readPtr)
    {
      ipcMessageBox.writePtr = nextWritePtr ;
      ipcMessageBox.ringBuffer[nextWritePtr] = (uint8_t)data ;
    }
  } else
  {
    ipcMessageBox.counterOfOnes++ ;
    if (ipcMessageBox.counterOfOnes > 7)
    {
      Terminate() ;
    }
  }
}

void IpcConsumeOneFromMessageBox()
{
  uint32_t prevIrq ;
  while (true)
  {
    prevIrq = CpuDisableIRQ() ;
    if (ipcMessageBox.counterOfOnes != 0)
      break ;
    CpuSetIRQBit(prevIrq) ;
  }
  ipcMessageBox.counterOfOnes-- ;
  CpuSetIRQBit(prevIrq) ;
}

void IpcInitMessageBox()
{
  if (!(REG_IPC_FIFOCNT & FIFOCNT_ENABLE))
    REG_IPC_FIFOCNT |= FIFOCNT_ENABLE | FIFOCNT_ERRORACK;
  memset(&ipcMessageBox, 0, sizeof(ipcMessageBox)) ;
  // wait for the arm7 to initialize the message box first
  #ifdef ARM9
  while (!IpcIsFifoCommandHandlerregisteredForCore(2, 1)) 
    ;
  #endif
  IpcRegisterFIFOCommandHandler(1, IpcFifoCommandHandler_01) ;
  IpcRegisterFIFOCommandHandler(2, IpcFifoCommandHandler_02) ;
  #ifdef ARM7
  while (!IpcIsFifoCommandHandlerregisteredForCore(2, 0)) 
    ;
  #endif
}

void IpcSendInterrupt() 
{
  REG_IPC_SYNC |= 0x2000 ;
}

void IpcInitialize()
{
  REG_IPC_SYNC = 0x4000 ;
  REG_IPC_FIFOCNT = 0x8000 ;
}