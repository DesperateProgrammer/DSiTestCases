/*---------------------------------------------------------------------------------

	Simple console print demo
	-- dovoto

---------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>
#include <string.h>

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
  WRAMSelfTest(&wramPostResult) ;
  //     |                            |
  
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

  printf("WRAM Window Masks\n") ;
  for (int i=0;i<3;i++)
  {
    printf(" %08x", wramPostResult.wramWindowSetMask[i] & wramPostResult.wramWindowClearMask[i]) ;
  }
  printf("\n") ;  

  for (int i=0;i<3;i++)
  {
    printf(" %08x", wramPostResult.wramWindowMask[i]) ;
  }
  printf("\n") ;  
  
  printf("Disabling NWRAM in SCFG_EXT!\n") ;
  *((volatile uint32_t *)0x04004008) &= ~(1 << 25) ;
  WRAMSelfTest(&wramPostResult) ;
  //     |                            |
  
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

  printf("WRAM Window Masks\n") ;
  for (int i=0;i<3;i++)
  {
    printf(" %08x", wramPostResult.wramWindowSetMask[i] & wramPostResult.wramWindowClearMask[i]) ;
  }
  printf("\n") ;  

  for (int i=0;i<3;i++)
  {
    printf(" %08x", wramPostResult.wramWindowMask[i]) ;
  }
  printf("\n") ;  
  int count = 0;
  
 
	while(1) {

		iprintf("\x1b[23;0HCount = %04i", count++);
		cp15WaitForInterrupt() ;    
	}

	Terminate() ;
}
