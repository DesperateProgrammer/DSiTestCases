#include "cp15.h"

#define CP15_CR_MPUENABLE              0x00000001
#define CP15_CR_ALIGNMENTCHECK         0x00000002
#define CP15_CR_CACHEENABLE            0x00000004
#define CP15_CR_WRITEBUFFERENABLE      0x00000008
#define CP15_CR_32BITEXCEPTION         0x00000010
#define CP15_CR_26BITALIGNCECKDISABLE  0x00000020
#define CP15_CR_LATEABORT              0x00000040
#define CP15_CR_BIGENDIAN              0x00000080
#define CP15_CR_SYSTEMPROTECT          0x00000100
#define CP15_CR_ROMPROTECT             0x00000200
#define CP15_CR_BRANCHPREDICTION       0x00000800
#define CP15_CR_INSTRUCTIONCACHEENABLE 0x00001000
#define CP15_CR_VECTORHIGH             0x00002000
#define CP15_CR_ROUNDROBINCACHE        0x00004000

#define CP15_REGIONSIZE_4k             0x0B
#define CP15_REGIONSIZE_8k             0x0C
#define CP15_REGIONSIZE_16k            0x0D
#define CP15_REGIONSIZE_32k            0x0E
#define CP15_REGIONSIZE_64k            0x0F
#define CP15_REGIONSIZE_128k           0x10
#define CP15_REGIONSIZE_256k           0x11
#define CP15_REGIONSIZE_512k           0x12
#define CP15_REGIONSIZE_1M             0x13
#define CP15_REGIONSIZE_2M             0x14
#define CP15_REGIONSIZE_4M             0x15
#define CP15_REGIONSIZE_8M             0x16
#define CP15_REGIONSIZE_16M            0x17
#define CP15_REGIONSIZE_32M            0x18
#define CP15_REGIONSIZE_64M            0x19
#define CP15_REGIONSIZE_128M           0x1A
#define CP15_REGIONSIZE_256M           0x1B
#define CP15_REGIONSIZE_512M           0x1C
#define CP15_REGIONSIZE_1G             0x1D
#define CP15_REGIONSIZE_2G             0x1E
#define CP15_REGIONSIZE_4G             0x1F

#define CP15_REGIONENABLE              1


// FunFact: The existance of register 5 access via opcode 2/3 we known it is a arm9e-s core
#define CP15_PERMISSION_NONE           0
#define CP15_PERMISSION_PRIV           1
#define CP15_PERMISSION_READONLY       2
#define CP15_PERMISSION_READWRITE      3
#define CP15_PERMISSION_PRIVREADONLY   5
#define CP15_PERMISSION_ALLREADONLY    6

#define CP15_REGION(n)                (1 << (n))
#define CP15_REGIONACCESS(n, a)       (((a) & 0x03) << ((n)*4)) 


__attribute__((naked)) uint32_t cp15ReadControlRegister() 
{
  __asm(
    "mrc p15, 0x0, r0, c1, c0, 0      \n"
    "bx lr                             \n"
  );
}

__attribute__((naked)) void cp15WriteControlRegister(uint32_t value) 
{
  __asm(
    "mcr p15, 0x0, r0, c1, c0, 0      \n"
    "bx lr                             \n"
  );
}


__attribute__((naked)) void cp15SetRegionsDataCachableRegister(uint32_t value) 
{
  __asm(
    "mcr p15, 0x0, r0, c2, c0, 0      \n"
    "bx lr                             \n"
  );
}

__attribute__((naked)) void cp15SetRegionsInstructionCachableRegister(uint32_t value) 
{
  __asm(
    "mcr p15, 0x0, r0, c2, c0, 1      \n"
    "bx lr                             \n"
  );
}

__attribute__((naked)) void cp15SetRegionsInstructionAccessRegister(uint32_t value) 
{
  __asm(
    "mcr p15, 0x0, r0, c5, c0, 3      \n"
    "bx lr                             \n"
  );
}

__attribute__((naked)) void cp15SetRegionsDataAccessRegister(uint32_t value) 
{
  __asm(
    "mcr p15, 0x0, r0, c5, c0, 2      \n"
    "bx lr                             \n"
  );
}

__attribute__((naked)) void cp15SetRegionsDataBufferableRegister(uint32_t value) 
{
  __asm(
    "mcr p15, 0x0, r0, c2, c0, 0      \n"
    "bx lr                             \n"
  );
}

__attribute__((naked)) void cp15SetProtectionRegionRegister0(uint32_t value) 
{
  __asm(
    "mcr p15, 0x0, r0, c6, c0, 0      \n"
    "bx lr                             \n"
  );
}

__attribute__((naked)) void cp15SetProtectionRegionRegister1(uint32_t value) 
{
  __asm(
    "mcr p15, 0x0, r0, c6, c1, 0      \n"
    "bx lr                             \n"
  );
}

__attribute__((naked)) void cp15SetProtectionRegionRegister2(uint32_t value) 
{
  __asm(
    "mcr p15, 0x0, r0, c6, c2, 0      \n"
    "bx lr                             \n"
  );
}

__attribute__((naked)) void cp15SetProtectionRegionRegister3(uint32_t value) 
{
  __asm(
    "mcr p15, 0x0, r0, c6, c3, 0      \n"
    "bx lr                             \n"
  );
}

__attribute__((naked)) void cp15SetProtectionRegionRegister4(uint32_t value) 
{
  __asm(
    "mcr p15, 0x0, r0, c6, c4, 0      \n"
    "bx lr                             \n"
  );
}

__attribute__((naked)) void cp15SetProtectionRegionRegister5(uint32_t value) 
{
  __asm(
    "mcr p15, 0x0, r0, c6, c5, 0      \n"
    "bx lr                             \n"
  );
}

__attribute__((naked)) void cp15SetProtectionRegionRegister6(uint32_t value) 
{
  __asm(
    "mcr p15, 0x0, r0, c6, c6, 0      \n"
    "bx lr                             \n"
  );
}

__attribute__((naked)) void cp15SetProtectionRegionRegister7(uint32_t value) 
{
  __asm(
    "mcr p15, 0x0, r0, c6, c7, 0      \n"
    "bx lr                             \n"
  );
}

__attribute__((naked)) void cp15WriteDataCacheLockdownRegister(uint32_t value) 
{
  __asm(
    "mcr p15, 0x0, r0, c9, c1, 0       \n"
    "bx lr                             \n"
  );
}

__attribute__((naked)) void cp15WriteInstructionCacheLockdownRegister(uint32_t value) 
{
  __asm(
    "mcr p15, 0x0, r0, c9, c1, 0       \n"
    "bx lr                             \n"
  );
}

void cp15DisableMPU() 
{
  uint32_t cr1 = cp15ReadControlRegister() ;
  if (cr1 & CP15_CR_MPUENABLE)
  {
    if (cr1 & CP15_CR_CACHEENABLE)
    {
      cp15ClearAllDataCachelines() ;
    }
    cp15DrainWriteBuffer() ;
  }
  
  cr1 &= ~(CP15_CR_MPUENABLE | CP15_CR_CACHEENABLE | CP15_CR_INSTRUCTIONCACHEENABLE | 0xF8000) ; // TODO: find the bit meanings here
  cr1 |= CP15_CR_VECTORHIGH | CP15_CR_LATEABORT | CP15_CR_26BITALIGNCECKDISABLE | CP15_CR_32BITEXCEPTION | CP15_CR_WRITEBUFFERENABLE ;
  
  cp15WriteControlRegister(cr1) ;
  
  cp15WriteDataCacheLockdownRegister(0x20) ;
  cp15WriteInstructionCacheLockdownRegister(0x02fe000A) ;
  
  cr1 = 0x50000 ; // TODO: find the bit meanings here
  cp15WriteControlRegister(cr1) ;  
}

__attribute__((naked)) bool cp15DisableCache() 
{
  __asm(
    "mrc p15, 0x0, r1, c1, c0, 0      \n"
    "and r0, r1, #4                   \n"
    "mov r0, r0, lsr #2               \n"
    "bic r1, r1, #4                   \n"
    "mcr p15, 0x0, r1, c1, c0, 0      \n"
    "bx lr                            \n"
  ) ;
}

__attribute__((naked)) bool cp15DisableInstructionCache() 
{
  __asm(
    "mrc p15, 0x0, r1, c1, c0, 0      \n"
    "and r0, r1, #0x1000              \n"
    "mov r0, r0, lsr #12              \n"
    "bic r1, r1, #0x1000              \n"
    "mcr p15, 0x0, r1, c1, c0, 0      \n"
    "bx lr                            \n"
  ) ;
}

__attribute__((naked)) static void cp15CleanDataCacheBySETINDEX(uint32_t setindex)
{
  __asm(
    "mcr p15, 0x0, r0, c7, c10, 2     \n"
    "bx lr                            \n"
  ) ;
} 

__attribute__((naked)) static void cp15CleanAndInvaidateDataCacheBySETINDEX(uint32_t setindex)
{
  __asm(
    "mcr p15, 0x0, r0, c7, c14, 2     \n"
    "bx lr                            \n"
  ) ;
} 

__attribute__((naked)) static void cp15WriteCacheDebugIndexRegister(uint32_t index)
{
  __asm(
    "mcr p15, 0x3, r0, c15, c0, 0     \n"
    "bx lr                            \n"
  ) ;
} 

__attribute__((naked)) static void cp15WriteDataTag(uint32_t tag)
{
  __asm(
    "mcr p15, 0x3, r0, c15, c2, 0     \n"
    "bx lr                            \n"
  ) ;
} 

__attribute__((naked)) static void cp15WriteInstructionCacheTag(uint32_t tag)
{
  __asm(
    "mcr p15, 0x3, r0, c15, c2, 0     \n"
    "bx lr                            \n"
  ) ;
} 

__attribute__((naked)) static void cp15WriteInstructionCache(uint32_t tag)
{
  __asm(
    "mcr p15, 0x3, r0, c15, c3, 0     \n"
    "bx lr                            \n"
  ) ;
} 


__attribute__((naked)) static void cp15WriteDataCache(uint32_t data)
{
  __asm(
    "mcr p15, 0x3, r0, c15, c4, 0     \n"
    "bx lr                            \n"
  ) ;
} 

__attribute__((naked)) void cp15DrainWriteBuffer()
{
  __asm(
    "eor r0, r0, r0                   \n"
    "mcr p15, 0x0, r0, c7, c10, 4     \n"
    "bx lr                            \n"
  ) ;
} 

__attribute__((naked)) void cp15InvalidateInstructionCache() 
{
  __asm(
    "eor r0, r0, r0                   \n"
    "mcr p15, 0x0, r0, c7, c5, 0      \n"
    "bx lr                            \n"
  ) ;
}

__attribute__((naked)) void cp15InvalidateDataCache() 
{
  __asm(
    "eor r0, r0, r0                   \n"
    "mcr p15, 0x0, r0, c7, c6, 0      \n"
    "bx lr                            \n"
  ) ;
}


void cp15ClearDataCachlineByIndex(uint8_t set, uint8_t index) 
{
  cp15CleanDataCacheBySETINDEX(((set & 0x03) << 30) | ((index & 0x1F) << 5)) ;
}

void cp15ClearAndInvalidateDataCachlineByIndex(uint8_t set, uint8_t index) 
{
  cp15CleanDataCacheBySETINDEX(((set & 0x03) << 30) | ((index & 0x1F) << 5)) ;
}

void cp15ClearAllDataCachelines() 
{
  for (uint8_t set = 0;set < 4; set++)
    for (uint8_t line = 0;line < 32; line++)
      cp15ClearDataCachlineByIndex(set, line) ;
}

void cp15WriteTagToAllDataCacheLines(uint32_t tag) 
{
  for (uint8_t set = 0;set < 4; set++)
    for (uint8_t line = 0;line < 32; line++)
    {
      cp15WriteCacheDebugIndexRegister(((set & 0x03) << 30) | ((line & 0x1F) << 5)) ;
      cp15WriteDataTag(tag) ;
    }
}

void cp15ClearTagOfAllDataCacheLines() 
{
  cp15WriteTagToAllDataCacheLines(0) ;
}

void cp15WriteTagToAllInstructionCacheLines(uint32_t tag) 
{
  for (uint8_t set = 0;set < 4; set++)
    for (uint8_t line = 0;line < 32; line++)
    {
      cp15WriteCacheDebugIndexRegister(((set & 0x03) << 30) | ((line & 0x1F) << 5)) ;
      cp15WriteInstructionCacheTag(tag) ;
    }
}

void cp15ClearTagOfAllInstructionCacheLines() 
{
  cp15WriteTagToAllInstructionCacheLines(0) ;
}

void cp15WriteToAllDataCacheLines(uint32_t data) 
{
  for (uint8_t set = 0;set < 4; set++)
    for (uint8_t line = 0;line < 32; line++)
    {
      cp15WriteCacheDebugIndexRegister(((set & 0x03) << 30) | ((line & 0x1F) << 5)) ;
      cp15WriteDataCache(data) ;
    }
}

void cp15ClearOfAllDataCache() 
{
  cp15WriteToAllDataCacheLines(0) ;
}

void cp15WriteToAllInstructionCacheLines(uint32_t data) 
{
  for (uint8_t set = 0;set < 4; set++)
    for (uint8_t line = 0;line < 32; line++)
    {
      cp15WriteCacheDebugIndexRegister(((set & 0x03) << 30) | ((line & 0x1F) << 5)) ;
      cp15WriteInstructionCache(data) ;
    }
}

void cp15ClearOfAllInstructionCache()
{
  cp15WriteToAllInstructionCacheLines(0) ;
}

void cp15FlushAndInvalidateAllDataCachelines()
{
  for (uint8_t set = 0;set < 4; set++)
    for (uint8_t line = 0;line < 32; line++)
    {
      cp15DrainWriteBuffer() ;
      cp15ClearAndInvalidateDataCachlineByIndex(set, line) ;
    }
}

__attribute__((naked)) void cp15InvalidateDatacacheByAddress(uint32_t address) 
{
  __asm(
    "mcr p15, 0x0, r0, c7, c6, 1      \n"
    "bx lr                            \n"
  ) ;
}

void cp15InvalidateDatacacheByAddressRange(uint32_t address, uint32_t length) 
{
  uint32_t curAddress = address & ~0x1F ;
  do
  {
    cp15InvalidateDatacacheByAddress(curAddress) ;
    curAddress += 0x20 ;
  } while (curAddress < address + length) ;
}

__attribute__((naked)) void cp15InvalidateInstructionCacheByAddress(uint32_t address) 
{
  __asm(
    "mcr p15, 0x0, r0, c7, c5, 1      \n"
    "bx lr                            \n"
  ) ;
}

void cp15InvalidateInstructionCacheByAddressRange(uint32_t address, uint32_t length) 
{
  uint32_t curAddress = address & ~0x1F ;
  do
  {
    cp15InvalidateInstructionCacheByAddress(curAddress) ;
    curAddress += 0x20 ;
  } while (curAddress < address + length) ;
}

__attribute__((naked)) void cp15CleanDatacacheByAddress(uint32_t address) 
{
  __asm(
    "mcr p15, 0x0, r0, c7, c10, 1     \n"
    "bx lr                            \n"
  ) ;
}

void cp15CleanDatacacheByAddressRange(uint32_t address, uint32_t length) 
{
  uint32_t curAddress = address & ~0x1F ;
  do
  {
    cp15CleanDatacacheByAddress(curAddress) ;
    curAddress += 0x20 ;
  } while (curAddress < address + length) ;
}

void cp15FlushAndInvalidateDatacacheByAddressRange(uint32_t address, uint32_t length) 
{
  uint32_t curAddress = address & ~0x1F ;
  do
  {
    cp15DrainWriteBuffer() ;
    cp15InvalidateDatacacheByAddress(curAddress) ;
    curAddress += 0x20 ;
  } while (curAddress < address + length) ;
}

void cp15InitRegions() 
{
  // define regions
  cp15SetProtectionRegionRegister0(0x04000000 | (CP15_REGIONSIZE_64M << 1) | CP15_REGIONENABLE) ;
  cp15SetProtectionRegionRegister1(0x02000000 | (CP15_REGIONSIZE_32M << 1) | CP15_REGIONENABLE) ;
  cp15SetProtectionRegionRegister2(0x02f80000 | (CP15_REGIONSIZE_512k << 1) | CP15_REGIONENABLE) ;
  cp15SetProtectionRegionRegister3(0x0d000000 | (CP15_REGIONSIZE_16M << 1) | CP15_REGIONENABLE) ;
  cp15SetProtectionRegionRegister4(0x02fe0000 | (CP15_REGIONSIZE_16k << 1) | CP15_REGIONENABLE) ;  
  cp15SetProtectionRegionRegister5(0x01000000 | (CP15_REGIONSIZE_16M << 1) | CP15_REGIONENABLE) ;
  cp15SetProtectionRegionRegister6(0xffff0000 | (CP15_REGIONSIZE_32k << 1) | CP15_REGIONENABLE) ;
  cp15SetProtectionRegionRegister6(0x02ffc000 | (CP15_REGIONSIZE_16k << 1) | CP15_REGIONENABLE) ;
  
  // Allow caches on regions
  cp15SetRegionsDataCachableRegister(CP15_REGION(1) | CP15_REGION(2) | CP15_REGION(3) | CP15_REGION(6)) ;
  cp15SetRegionsInstructionCachableRegister(CP15_REGION(1) | CP15_REGION(3) | CP15_REGION(6)) ;
  
  // Allow buffering on regions
  cp15SetRegionsDataBufferableRegister(CP15_REGION(1) | CP15_REGION(2) | CP15_REGION(3)) ;
  
  // Set access rights to regions  
  cp15SetRegionsInstructionAccessRegister(
    CP15_REGIONACCESS(0, CP15_PERMISSION_PRIV) |
    CP15_REGIONACCESS(1, CP15_PERMISSION_PRIV) |
    CP15_REGIONACCESS(2, CP15_PERMISSION_NONE) |
    CP15_REGIONACCESS(3, CP15_PERMISSION_PRIV) |
    CP15_REGIONACCESS(4, CP15_PERMISSION_NONE) |
    CP15_REGIONACCESS(5, CP15_PERMISSION_PRIV) |
    CP15_REGIONACCESS(6, CP15_PERMISSION_PRIVREADONLY) |
    CP15_REGIONACCESS(7, CP15_PERMISSION_NONE) 
  ) ;
  cp15SetRegionsDataAccessRegister(
    CP15_REGIONACCESS(0, CP15_PERMISSION_PRIV) |
    CP15_REGIONACCESS(1, CP15_PERMISSION_PRIV) |
    CP15_REGIONACCESS(2, CP15_PERMISSION_PRIV) |
    CP15_REGIONACCESS(3, CP15_PERMISSION_PRIV) |
    CP15_REGIONACCESS(4, CP15_PERMISSION_PRIV) |
    CP15_REGIONACCESS(5, CP15_PERMISSION_PRIV) |
    CP15_REGIONACCESS(6, CP15_PERMISSION_PRIVREADONLY) |
    CP15_REGIONACCESS(7, CP15_PERMISSION_PRIV) 
  ) ;

  uint32_t cr1 = cp15ReadControlRegister() ;
  cr1 |= CP15_CR_INSTRUCTIONCACHEENABLE | CP15_CR_ROUNDROBINCACHE | CP15_CR_CACHEENABLE | CP15_CR_MPUENABLE ;
  cp15WriteControlRegister(cr1) ;  
  
  cp15InvalidateInstructionCache() ;
  cp15InvalidateDataCache() ;
}

__attribute__((naked)) void cp15WaitForInterrupt()
{
  __asm(
    "mov r0, #0                       \n"
    "mcr p15, 0x0, r0, c7, c0, 4      \n"
    "bx lr                            \n"
  ) ;}