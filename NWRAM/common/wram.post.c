// WRAM Self-Test

#include "wram.post.h"
#include "sdk.h"
#include <string.h>

void WRAMSelfTest_Read(SWRAMPOSTRESULTS *post) ;
void WRAMSelfTest_Write(SWRAMPOSTRESULTS *post) ;
void WRAMSelfTest_BankRead(SWRAMPOSTRESULTS *post) ;
void WRAMSelfTest_BankWrite(SWRAMPOSTRESULTS *post) ;

void WRAMSelfTest(SWRAMPOSTRESULTS *post) 
{
  WRAMSelfTest_Read(post) ;
  WRAMSelfTest_Write(post) ;
  #ifdef ARM9
    WRAMSelfTest_BankRead(post) ;
    WRAMSelfTest_BankWrite(post) ;
  #endif
}

void WRAMSelfTest_BankRead(SWRAMPOSTRESULTS *post) 
{
  for (int page = 0;page < 8;page ++)   
  {
    WRAMConfig(1, page, WRAM_OWNER_ARM9, 0, true) ;
  }    
  WRAMSetWindow(0, 0x03000000, 0x03010000) ;
  WRAMSetWindow(1, 0x03100000, 0x031F0000) ;
  WRAMSetWindow(2, 0x03200000, 0x032F0000) ;
  
  // set the banks to a known value
  for (int bank = 0; bank < 8; bank ++)
  {
    // enable only the desired bank 
    for (int q = 0; q< 8; q++)
      WRAMConfig(1, q, WRAM_OWNER_ARM9, 0, bank == q) ;
    *(volatile uint32_t *)0x03100000 = 0x55aa0100 | bank ;
  }
  
  // now move them all over each other and enable!
  // except when we had them already as priority
  for (int q = 0; q< 8; q++)
    WRAMConfig(1, q, WRAM_OWNER_ARM9, 0, true) ;

  for (int i=0;i<8;i++)
  {
    // get which bank is up front when reding
    uint8_t curBank = (*(volatile uint32_t *)0x03100000) & 0x07 ;
    // and disable the bank
    WRAMConfig(1, curBank, WRAM_OWNER_ARM9, 0, false) ;
    post->wramReadBankPriority[i] = curBank ;
  }
}

void WRAMSelfTest_BankWrite(SWRAMPOSTRESULTS *post) 
{
  for (int page = 0;page < 8;page ++)   
  {
    WRAMConfig(1, page, WRAM_OWNER_ARM9, 0, true) ;
  }    
  WRAMSetWindow(0, 0x03000000, 0x03010000) ;
  WRAMSetWindow(1, 0x03100000, 0x031F0000) ;
  WRAMSetWindow(2, 0x03200000, 0x032F0000) ;
  
  // set the banks to a known value
  for (int bank = 0; bank < 8; bank ++)
  {
    // enable only the desired bank 
    for (int q = 0; q< 8; q++)
      WRAMConfig(1, q, WRAM_OWNER_ARM9, 0, bank == q) ;
    *(volatile uint32_t *)0x03100000 = 0 ;
  }
  
  uint8_t foundBanks = 0 ;
  for (int i=0;i<8;i++)
  {
    // now move them all over each other and enable!
    // except when we had them already as priority
    for (int q = 0; q< 8; q++)
      WRAMConfig(1, q, WRAM_OWNER_ARM9, 0, !(foundBanks & (1 << q))) ;
    // write a new value
    *(volatile uint32_t *)0x03100000 = 0xaa550100 | i ;
    // check which bank hast that value
    uint8_t curPrio = 0 ;
    for (int bank = 0; bank < 8; bank ++)
    {
      // enable only the desired bank 
      for (int q = 0; q< 8; q++)
        WRAMConfig(1, q, WRAM_OWNER_ARM9, 0, bank == q) ;
        
      if (*(volatile uint32_t *)0x03100000 == (0xaa550100 | i))
      {
        curPrio |= (1 << bank) ;
      }
    }
    post->wramWriteBankPriority[i] = curPrio ;
    foundBanks |= curPrio ;
  }
}

void WRAMSelfTest_Write(SWRAMPOSTRESULTS *post) 
{
  for (int i=0;i<sizeof(post->wramWritePrioity) / sizeof(*post->wramWritePrioity);i++)
    post->wramWritePrioity[i] = 0 ;

  // Step 1: Synchronize Arm7 and Arm9 to same state
  #ifdef ARM9
    // Wait for both cores to have the same sync ID
    SetIPCSyncValue(0) ;
    WaitForIPCSyncValue(0) ;
    // Depending on previous state, that could have been already there
    // so do again with a different value
    for (int set = 0;set < 3;set ++)   
      for (int page = 0;page < 8;page ++)   
      {
        WRAMConfig(set, page, WRAM_OWNER_ARM7, 0, true) ;
      }    
    SetIPCSyncValue(1) ;
    WaitForIPCSyncValue(1) ;
  #else
    // Wait for both cores to have the same sync ID
    WaitForIPCSyncValue(0) ;
    SetIPCSyncValue(0) ;
    // Depending on previous state, that could have been already there
    // so do again with a different value
    WaitForIPCSyncValue(1) ;
    for (int set = 0;set<3;set++)
    {
      for (int page = 0;page < 8;page ++)   
      {
        WRAMSetWriteProtection(set, page, false) ;
      }
    }
    SetIPCSyncValue(1) ;
  #endif
  
  #ifdef ARM9
    SetIPCSyncValue(2) ;
    WaitForIPCSyncValue(2) ;
  #else
    // Write a known values to each bank seperately
    WRAMSetWindow(0, 0x03000000, 0x03010000) ;
    WRAMSetWindow(1, 0x03000000, 0x03000000) ;
    WRAMSetWindow(2, 0x03000000, 0x03000000) ;
    *(volatile uint32_t *)0x03000000 = 0x55aa0000 | WRAM_TYPE_SETA ;    // Bank size is 64k for Set A
    *(volatile uint32_t *)0x03008000 = 0x55aa0080 | WRAM_TYPE_SETA ;    // so mark both
    WRAMSetWindow(1, 0x03000000, 0x03010000) ;
    WRAMSetWindow(0, 0x03000000, 0x03000000) ;
    WRAMSetWindow(2, 0x03000000, 0x03000000) ;
    *(volatile uint32_t *)0x03000000 = 0x55aa0000 | WRAM_TYPE_SETB ;    // Set B
    WRAMSetWindow(2, 0x03000000, 0x03010000) ;
    WRAMSetWindow(1, 0x03000000, 0x03000000) ;
    WRAMSetWindow(0, 0x03000000, 0x03000000) ;
    *(volatile uint32_t *)0x03000000 = 0x55aa0000 | WRAM_TYPE_SETC ;    // Set C
    WRAMSetWindow(0, 0x03000000, 0x03000000) ;
    WRAMSetWindow(1, 0x03000000, 0x03000000) ;
    WRAMSetWindow(2, 0x03000000, 0x03000000) ;
    *(volatile uint32_t *)0x03818000 = 0x55aa0000 | WRAM_TYPE_STATIC ;  // static WRAM
    *(volatile uint32_t *)0x03000000 = 0x55aa0000 | WRAM_TYPE_OLD ;  // static WRAM
    // then, blend them in at the same place (start at static wram)
    WRAMSetWindow(0, 0x03810000, 0x038F0000) ;
    WRAMSetWindow(1, 0x03810000, 0x038F0000) ;
    WRAMSetWindow(2, 0x03810000, 0x038F0000) ;
    // and now iteratively find the top priority and deactive it untile the last does not change
    int i = 0 ;
    uint32_t addr = 0x03810000 ;
    uint32_t foundMatches = 0 ;
    do
    {
        // write a value to the address and check which bank took the bait
        uint32_t curWriteTarget = 0 ;
        *(volatile uint32_t *)addr = 0xaa550000 | i ;
        WRAMSetWindow(0, 0x03000000, 0x03010000) ;
        WRAMSetWindow(1, 0x03010000, 0x03020000) ;
        WRAMSetWindow(2, 0x03020000, 0x03030000) ;
        if (*((volatile uint32_t *)addr) == (0xaa550000 | i))
        {
          curWriteTarget |= (1 << WRAM_TYPE_STATIC) ;
        }
        if (*((volatile uint32_t *)0x03000000) == (0xaa550000 | i))
        {
          curWriteTarget |= (1 << WRAM_TYPE_SETA) ;
        } else
        {
          if (!(foundMatches & (1 << WRAM_TYPE_SETA)))
            WRAMSetWindow(0, 0x03810000, 0x038F0000) ;          
        }
        if (*((volatile uint32_t *)0x03010000) == (0xaa550000 | i))
        {
          curWriteTarget |= (1 << WRAM_TYPE_SETB) ;
        } else
        {
          if (!(foundMatches & (1 << WRAM_TYPE_SETB)))
            WRAMSetWindow(1, 0x03810000, 0x038F0000) ;          
        }
        if (*((volatile uint32_t *)0x03020000) == (0xaa550000 | i))
        {
          curWriteTarget |= (1 << WRAM_TYPE_SETC) ;
        } else
        {
          if (!(foundMatches & (1 << WRAM_TYPE_SETC)))
            WRAMSetWindow(2, 0x03810000, 0x038F0000) ;          
        }
        post->wramWritePrioity[i] = curWriteTarget ;
        foundMatches |= curWriteTarget ;
        i++ ;
    } while (i*sizeof(*post->wramWritePrioity) < sizeof(post->wramWritePrioity))  ;
    // We do the same for the old shared wram region
    // Write a known values to each bank seperately
    WRAMSetWindow(0, 0x03000000, 0x03010000) ;
    WRAMSetWindow(1, 0x03000000, 0x03000000) ;
    WRAMSetWindow(2, 0x03000000, 0x03000000) ;
    *(volatile uint32_t *)0x03000000 = 0x55aa0000 | WRAM_TYPE_SETA ;    // Bank size is 64k for Set A
    *(volatile uint32_t *)0x03008000 = 0x55aa0080 | WRAM_TYPE_SETA ;    // so mark both
    WRAMSetWindow(1, 0x03000000, 0x03010000) ;
    WRAMSetWindow(0, 0x03000000, 0x03000000) ;
    WRAMSetWindow(2, 0x03000000, 0x03000000) ;
    *(volatile uint32_t *)0x03000000 = 0x55aa0000 | WRAM_TYPE_SETB ;    // Set B
    WRAMSetWindow(2, 0x03000000, 0x03010000) ;
    WRAMSetWindow(1, 0x03000000, 0x03000000) ;
    WRAMSetWindow(0, 0x03000000, 0x03000000) ;
    *(volatile uint32_t *)0x03000000 = 0x55aa0000 | WRAM_TYPE_SETC ;    // Set C
    WRAMSetWindow(0, 0x03000000, 0x03000000) ;
    WRAMSetWindow(1, 0x03000000, 0x03000000) ;
    WRAMSetWindow(2, 0x03000000, 0x03000000) ;
    *(volatile uint32_t *)0x03818000 = 0x55aa0000 | WRAM_TYPE_STATIC ;  // static WRAM
    *(volatile uint32_t *)0x03000000 = 0x55aa0000 | WRAM_TYPE_OLD ;  // static WRAM
    // then, blend them in at the same place (start at static wram)
    WRAMSetWindow(0, 0x03000000, 0x03010000) ;
    WRAMSetWindow(1, 0x03000000, 0x03010000) ;
    WRAMSetWindow(2, 0x03000000, 0x03010000) ;
    // and now iteratively find the top priority and deactive it untile the last does not change
    i = 0 ;
    addr = 0x03000000 ;
    foundMatches = 0 ;
    do
    {
        if (post->wramWritePrioity[i] & (1 << WRAM_TYPE_STATIC))
        {
          bool alreadyIn = false ;
          for (int q=0;q<i;q++)
            if (post->wramWritePrioity[q] & (1 << WRAM_TYPE_STATIC))
              alreadyIn = true ;
          if (!alreadyIn)
          {
            i++;
            continue;
          }
        }
        // write a value to the address and check which bank took the bait
        uint32_t curWriteTarget = 0 ;
        *(volatile uint32_t *)addr = 0xaa550000 | i ;
        WRAMSetWindow(0, 0x03100000, 0x03110000) ;
        WRAMSetWindow(1, 0x03110000, 0x03120000) ;
        WRAMSetWindow(2, 0x03120000, 0x03130000) ;
        if (*((volatile uint32_t *)addr) == (0xaa550000 | i))
        {
          curWriteTarget |= (1 << WRAM_TYPE_OLD) ;
        }
        if (*((volatile uint32_t *)0x03100000) == (0xaa550000 | i))
        {
          curWriteTarget |= (1 << WRAM_TYPE_SETA) ;
        } else
        {
          if (!(foundMatches & (1 << WRAM_TYPE_SETA)))
            WRAMSetWindow(0, 0x03000000, 0x03010000) ;                    
        }
        if (*((volatile uint32_t *)0x03110000) == (0xaa550000 | i))
        {
          curWriteTarget |= (1 << WRAM_TYPE_SETB) ;
        } else
        {
          if (!(foundMatches & (1 << WRAM_TYPE_SETB)))
            WRAMSetWindow(1, 0x03000000, 0x03010000) ;                    
        }
        if (*((volatile uint32_t *)0x03120000) == (0xaa550000 | i))
        {
          curWriteTarget |= (1 << WRAM_TYPE_SETC) ;
        } else
        {
          if (!(foundMatches & (1 << WRAM_TYPE_SETB)))
            WRAMSetWindow(2, 0x03000000, 0x03010000) ;                    
        }        
        if (post->wramWritePrioity[i] != curWriteTarget)
        {
          // move the existing entries back by one
          for (int q= sizeof(post->wramWritePrioity) / sizeof(*post->wramWritePrioity)-1;q>i;q--)
            post->wramWritePrioity[q] = post->wramWritePrioity[q-1] ;
          //insert here
          post->wramWritePrioity[i] = curWriteTarget ;
        }        
        i++ ;
        foundMatches |= curWriteTarget ;
    } while (i*sizeof(*post->wramWritePrioity) < sizeof(post->wramWritePrioity))  ;
    
    
    // clean up multiple entries at the end, since we continued to observe the
    // value read from the memory location even after we could no longer disable
    // any bank
    for (int i= (sizeof(post->wramWritePrioity) / sizeof(*post->wramWritePrioity)) -1 ; i>0;i--)
    {
      if (post->wramWritePrioity[i-1] == post->wramWritePrioity[i])
        post->wramWritePrioity[i] = 0 ;
      else
        break;
    }
    // we now have the priority list for reading!
    // And we put that into nwram set a so the arm9 can read it
    WRAMSetWindow(0, 0x03000000, 0x03010000) ;
    memcpy((void *)0x03000000, post, sizeof(*post)) ;
    WaitForIPCSyncValue(2) ;    
    SetIPCSyncValue(2) ;
  #endif
  
  // retreive the results on the arm9
  #ifdef ARM9
    for (int set = 0;set < 3;set ++)   
      for (int page = 0;page < 8;page ++)   
      {
        WRAMConfig(set, page, WRAM_OWNER_ARM9, 0, true) ;
      }    
    WRAMSetWindow(0, 0x03000000, 0x03010000) ;
    WRAMSetWindow(1, 0x03010000, 0x03020000) ;
    WRAMSetWindow(2, 0x03020000, 0x03030000) ;
    memcpy(post, (void *)0x03000000, sizeof(*post)) ;
    SetIPCSyncValue(3) ;    
    WaitForIPCSyncValue(3) ;
  #else
    WaitForIPCSyncValue(3) ;    
    SetIPCSyncValue(3) ;
  #endif
  
}

void WRAMSelfTest_Read(SWRAMPOSTRESULTS *post) 
{
  for (int i=0;i<sizeof(post->wramReadPrioity) / sizeof(*post->wramReadPrioity);i++)
    post->wramReadPrioity[i] = WRAM_TYPE_NONE ;
  
  // Step 1: Synchronize Arm7 and Arm9 to same state
  #ifdef ARM9
    // Wait for both cores to have the same sync ID
    SetIPCSyncValue(0) ;
    WaitForIPCSyncValue(0) ;
    // Depending on previous state, that could have been already there
    // so do again with a different value
    SetIPCSyncValue(1) ;
    WaitForIPCSyncValue(1) ;
  #else
    // Wait for both cores to have the same sync ID
    WaitForIPCSyncValue(0) ;
    SetIPCSyncValue(0) ;
    // Depending on previous state, that could have been already there
    // so do again with a different value
    WaitForIPCSyncValue(1) ;
    for (int set = 0;set<3;set++)
    {
      for (int page = 0;page < 8;page ++)   
      {
        WRAMSetWriteProtection(set, page, false) ;
      }
    }
    SetIPCSyncValue(1) ;
  #endif
  
  // Step 2: Set up the smallest Window for all new wam blocks assigned to arm7
  #ifdef ARM9
    for (int set = 0;set<3;set++)
    {
      for (int page = 0;page < 8;page ++)   
      {
        WRAMConfig(set, page, WRAM_OWNER_ARM7, 0, true) ;
      }    
    }
    SetIPCSyncValue(2) ;
    WaitForIPCSyncValue(2) ;
  #else
    WaitForIPCSyncValue(2) ;    
    SetIPCSyncValue(2) ;
  #endif
  
  // Step 3: Do the actual priority test
  #ifdef ARM9
    SetIPCSyncValue(3) ;
    WaitForIPCSyncValue(3) ;
  #else
    // Write individual data to all new pages
    // for this, we do not like overlapping to ensure we really write to the bank
    // we intended. So we only blend in the desired bank one after another
    WRAMSetWindow(0, 0x03000000, 0x03010000) ;
    WRAMSetWindow(1, 0x03000000, 0x03000000) ;
    WRAMSetWindow(2, 0x03000000, 0x03000000) ;
    *(volatile uint32_t *)0x03000000 = 0x55aa0000 | WRAM_TYPE_SETA ;    // Bank size is 64k for Set A
    *(volatile uint32_t *)0x03008000 = 0x55aa0080 | WRAM_TYPE_SETA ;    // so mark both
    WRAMSetWindow(1, 0x03000000, 0x03010000) ;
    WRAMSetWindow(0, 0x03000000, 0x03000000) ;
    WRAMSetWindow(2, 0x03000000, 0x03000000) ;
    *(volatile uint32_t *)0x03000000 = 0x55aa0000 | WRAM_TYPE_SETB ;    // Set B
    WRAMSetWindow(2, 0x03000000, 0x03010000) ;
    WRAMSetWindow(1, 0x03000000, 0x03000000) ;
    WRAMSetWindow(0, 0x03000000, 0x03000000) ;
    *(volatile uint32_t *)0x03000000 = 0x55aa0000 | WRAM_TYPE_SETC ;    // Set C
    WRAMSetWindow(0, 0x03000000, 0x03000000) ;
    WRAMSetWindow(1, 0x03000000, 0x03000000) ;
    WRAMSetWindow(2, 0x03000000, 0x03000000) ;
    *(volatile uint32_t *)0x03818000 = 0x55aa0000 | WRAM_TYPE_STATIC ;  // static WRAM
    *(volatile uint32_t *)0x03000000 = 0x55aa0000 | WRAM_TYPE_OLD ;  // static WRAM
    // Blend all (but the old) in at the same address
    // from here we can check which data we read and then blend that page out to 
    // reveal the bank with the next highest priority.
    // Since old WRAM and static ARM7 wram do not overlap, we
    // will do this test twice and insert banks into the list on the second run.
    // This can not determinate the priority of the old wram and the static
    // First we do the static wram memory range
    WRAMSetWindow(0, 0x03810000, 0x038F0000) ;
    WRAMSetWindow(1, 0x03810000, 0x038F0000) ;
    WRAMSetWindow(2, 0x03810000, 0x038F0000) ;
    // and now iteratively find the top priority and deactive it untile the last does not change
    int i = 0 ;
    uint32_t addr = 0x03818000 ;
#if 1
    post->wramReadPrioity[0] = (*(volatile uint32_t *)addr) & 0x7f ;
    do
    {
      switch (post->wramReadPrioity[i])
      {
        case WRAM_TYPE_SETA:
          WRAMSetWindow(0, 0x03000000, 0x03000000) ;
          break;
        case WRAM_TYPE_SETB:
          WRAMSetWindow(1, 0x03000000, 0x03000000) ;
          break;
        case WRAM_TYPE_SETC:     
          WRAMSetWindow(2, 0x03000000, 0x03000000) ;
          break;
        case WRAM_TYPE_STATIC:
        case WRAM_TYPE_OLD:
          break;
      }      
      i++ ;
      post->wramReadPrioity[i] = (*(volatile uint32_t *)addr) & 0x7f ;
    } while (i*sizeof(*post->wramReadPrioity) < sizeof(post->wramReadPrioity))  ;
    post->wramReadPrioity[i] = WRAM_TYPE_NONE ;
    // we now have testet all but the old shared wram
    // So we check again in the old wram memory region and check wwhich priority there is
#endif
    // And at the second step we test the same in the old shared wram range
#if 1
    i = 0 ;
    addr = 0x03008000 ;
    WRAMSetWindow(0, 0x03000000, 0x03100000) ;
    WRAMSetWindow(1, 0x03000000, 0x03100000) ;
    WRAMSetWindow(2, 0x03000000, 0x03100000) ;      
    do
    {
      if (post->wramReadPrioity[i] == WRAM_TYPE_STATIC)
      {
        bool alreadyIn = false ;
        for (int q=0;q<i;q++)
          if (post->wramReadPrioity[q] == WRAM_TYPE_STATIC)
            alreadyIn = true ;
        if (!alreadyIn)
        {
          i++;
          continue;
        }
      }
      uint8_t curPrio = (*(volatile uint32_t *)addr) & 0x7f ;
      if (curPrio != post->wramReadPrioity[i])
      {
          // insert the old wram into the priority list
          for (int q= sizeof(post->wramReadPrioity) / sizeof(*post->wramReadPrioity)-1;q>i;q--)
            post->wramReadPrioity[q] = post->wramReadPrioity[q-1] ;
          post->wramReadPrioity[i] = curPrio ;
      }
      switch (curPrio)
      {
        case WRAM_TYPE_SETA:
          WRAMSetWindow(0, 0x03200000, 0x03200000) ;
          break;
        case WRAM_TYPE_SETB:
          WRAMSetWindow(1, 0x03200000, 0x03200000) ;
          break;
        case WRAM_TYPE_SETC:     
          WRAMSetWindow(2, 0x03200000, 0x03200000) ;
          break;
      }
      i++ ;
    } while (i*sizeof(*post->wramReadPrioity) < sizeof(post->wramReadPrioity))  ;
    
    // clean up multiple entries at the end, since we continued to observe the
    // value read from the memory location even after we could no longer disable
    // any bank
    for (int i= (sizeof(post->wramReadPrioity) / sizeof(*post->wramReadPrioity)) -1 ; i>0;i--)
    {
      if (post->wramReadPrioity[i-1] == post->wramReadPrioity[i])
        post->wramReadPrioity[i] = WRAM_TYPE_NONE ;
      else
        break;
    }
#endif
    // we now have the priority list for reading!
    // And we put that into nwram set a so the arm9 can read it
    WRAMSetWindow(0, 0x03000000, 0x03010000) ;
    memcpy((void *)0x03000000, post, sizeof(*post)) ;
    WaitForIPCSyncValue(3) ;    
    SetIPCSyncValue(3) ;
  #endif
  
  // retreive the results on the arm9
  #ifdef ARM9
    for (int set = 0;set < 3;set ++)   
      for (int page = 0;page < 8;page ++)   
      {
        WRAMConfig(set, page, WRAM_OWNER_ARM9, 0, true) ;
      }    
    WRAMSetWindow(0, 0x03000000, 0x03010000) ;
    WRAMSetWindow(1, 0x03010000, 0x03020000) ;
    WRAMSetWindow(2, 0x03020000, 0x03030000) ;
    memcpy(post, (void *)0x03000000, sizeof(*post)) ;
    SetIPCSyncValue(4) ;    
    WaitForIPCSyncValue(4) ;
  #else
    WaitForIPCSyncValue(4) ;    
    SetIPCSyncValue(4) ;
  #endif
}