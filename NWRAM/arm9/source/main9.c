/*---------------------------------------------------------------------------------

	Simple console print demo
	-- dovoto

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>

#include "sdk.h"
#include "wram.post.h"

volatile bool ipcSyncIRQHappened = false ;

void isrIpcSync()
{
  ipcSyncIRQHappened = true ;
}

int main(void) 
{	
  IpcInitialize() ;
  // Test IPC Sync
  SetIPCSyncValue(7) ;
  WaitForIPCSyncValue(7) ;

	consoleDemoInit();  //setup the sub screen for printing

	iprintf("NWRAM Test:\n");
	iprintf("-----------\n");
  
  /*******************************************
  *    Test IPC Sync                         *
  *******************************************/
  //     |                            |
  printf("IPC Sync Values             ") ;
  for (int i=0;i<16;i++)
  {
    SetIPCSyncValue(i) ;
    WaitForIPCSyncValue(i) ;
  }
  printf("OK\n");

  /*******************************************
  *    WRAM Control                          *
  *******************************************/
  SWRAMPOSTRESULTS wramPostResult ;
  //     |                            |
  printf("WRAM Read Priorities       \n") ;
  WRAMSelfTest(&wramPostResult) ;
  for (int i=0;i<sizeof(wramPostResult.wramReadPrioity) / sizeof(*wramPostResult.wramReadPrioity);i++)
  {
    switch (wramPostResult.wramReadPrioity[i])
    {
      case WRAM_TYPE_SETA:
        printf("   %i - new WRAM Set A\n", i) ;
        break;
      case WRAM_TYPE_SETB:
        printf("   %i - new WRAM Set B\n", i) ;
        break;
      case WRAM_TYPE_SETC:
        printf("   %i - new WRAM Set C\n", i) ;
        break;
      case WRAM_TYPE_OLD:
        printf("   %i - shared old WRAM\n", i) ;
        break;
      case WRAM_TYPE_STATIC:
        printf("   %i - static Arm7 WRAM\n", i) ;
        break;        
      case WRAM_TYPE_NONE:
        i = sizeof(wramPostResult.wramReadPrioity) ;
        break;
    }
  }
  //     |                            |
  printf("WRAM Write Priorities      \n") ;
  for (int i=0;i<sizeof(wramPostResult.wramWritePrioity) / sizeof(*wramPostResult.wramWritePrioity);i++)
  {
    for (int q=0;q<32;q++)
    {
      if (wramPostResult.wramWritePrioity[i] & (1 << q))
      {
        switch (q)
        {
          case WRAM_TYPE_SETA:
            printf("   %i - new WRAM Set A\n", i) ;
            break;
          case WRAM_TYPE_SETB:
            printf("   %i - new WRAM Set B\n", i) ;
            break;
          case WRAM_TYPE_SETC:
            printf("   %i - new WRAM Set C\n", i) ;
            break;
          case WRAM_TYPE_OLD:
            printf("   %i - shared old WRAM\n", i) ;
            break;
          case WRAM_TYPE_STATIC:
            printf("   %i - static Arm7 WRAM\n", i) ;
            break;        
        }
      }
    }
  }
  
  printf("WRAM Bank Read Priorities  \n") ;
  for (int i=0;i<8;i++)
  {
    printf(" %i", wramPostResult.wramReadBankPriority[i]) ;
  }
  printf("\n") ;  

  printf("WRAM Bank Write Priorities \n") ;
  for (int i=0;i<8;i++)
  {
    printf(" %02x", wramPostResult.wramWriteBankPriority[i]) ;
  }
  printf("\n") ;
  
  printf("WRAM Bank Masks\n") ;
  for (int i=0;i<4;i++)
  {
    printf(" %02x", wramPostResult.wramBankSingleSetMask[i] & wramPostResult.wramBankSingleClearMask[i]) ;
  }
  printf("\n") ;  
  for (int i=0;i<8;i++)
  {
    printf(" %02x", wramPostResult.wramBankSingleSetMask[i+4] & wramPostResult.wramBankSingleClearMask[i+4]) ;
  }
  printf("\n") ;  
  for (int i=0;i<8;i++)
  {
    printf(" %02x", wramPostResult.wramBankSingleSetMask[i+12] & wramPostResult.wramBankSingleClearMask[i+12]) ;
  }
  printf("\n") ;  
  
 
/*****************************************************************************
  * Test HW Registers via overlay. 
  *   Check if the NWRAM below HW registers show through, if there is no
  *   physical register present
  *****************************************************************************/
 
  int count = 0;
 
  WRAMSetWindow(0, 0x03FF0000, 0x04000000) ;
  memset((void *)0x03FF0000, 0xAA, 0x10000) ;
 
  WRAMSetWindow(0, 0x03FF0000, 0x04FF0000) ;
  for (int i=0;i<0x0ff0000;i++)
  {
    uint8_t val = ((volatile uint8_t *)0x04000000)[i] ;
    if (val == 0xAA)
    {
      printf("| %08x ", i) ;
      count++;
      if (count > 20)
        break ;
    }
  }
  printf("Read HW Regs Fallthrough Check: %i\n", count) ;
  count = 0;
  memset((void *)0x03FF0000, 0xAA, 0x10000) ;
  for (int i=0;i<0x0ff0000;i++)
  {
    ((volatile uint8_t *)0x04000000)[i] |= 0;
    if (((volatile uint8_t *)0x03FF0000)[i & 0xFFFF] == ((volatile uint8_t *)0x04000000)[i])
    {
      printf("| %08x:%02x ", i, ((volatile uint8_t *)0x03FF0000)[i & 0xFFFF]) ;
      count++;
      if (count > 20)
        break ;
    }
  }
  printf("Write HW Regs Fallthrough Check: %i\n", count) ;
  WRAMSetWindow(0, 0x03FF0000, 0x04000000) ;
  
	while(1) {

		iprintf("\x1b[23;0HCount = %04i", count++);
		cp15WaitForInterrupt() ;    
	}

	Terminate() ;
}
