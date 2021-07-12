// IPC related

#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef void (* ipcCommandHandlerFunc)(uint8_t commandId, uint32_t data, bool flag) ;

/** Get the IPC Sync Value from remote cpu 
      The value is 4 bit unsigned (0..15)
      
      IMPLEMENTED
      TESTED
*/
uint8_t GetIPCSyncValue() ;

/** Set the IPC Sync value of local cpu 
      The value is 4 bit unsigned (0..15)
      Unlike the official SDK, this is forced here
      
      IMPLEMENTED
      TESTED
*/
void SetIPCSyncValue(uint8_t value) ;

/** wait for a specific IPC value from remote cpu 
      value is 4 bit unsigned.
      Unlike the official SDK, this is forced here
      
      IMPLEMENTED
      TESTED
*/
void WaitForIPCSyncValue(uint8_t value) ;
    
/** wait for the highest bit in IPC sync from remote is set */
void WaitForIPCSyncValueHigh() ;

/** Tries to sends values via FIFO */
int32_t TrySendByFifo(uint8_t command, uint32_t data, bool flag) ;

/** Register a Ipc FIFO Command Handler */
void IpcRegisterFIFOCommandHandler(uint8_t number, ipcCommandHandlerFunc handler) ;

/** check whether a fifo command handler was registered on the arm7 (1) or arm9 (0) */
bool IpcIsFifoCommandHandlerregisteredForCore(uint8_t number, uint8_t core) ;

/** check for new data in fifo and process it */
void IpcReadAndProcessFIFOValue() ;

void IpcSendToMessageBox(uint8_t msg) ;

void IpcInitMessageBox() ;

uint8_t IpcGetMessageBoxUsage() ;
uint8_t IpcConsumeFromMessageBox() ;
void IpcSendOneToMessageBox() ;
void IpcConsumeOneFromMessageBox() ;

void IpcSendInterrupt() ;

void IpcInitialize() ;
