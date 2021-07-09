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
 
 int count = 0;
	while(1) {

		iprintf("\x1b[23;0HCount = %04i", count++);
		cp15WaitForInterrupt() ;    
	}

	Terminate() ;
}
