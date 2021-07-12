#include "wram.h"

/*******************************************************************************

    Defines / constants

*******************************************************************************/

typedef void (* WRAMSETCONFIG_FUNC)(uint8_t page, uint8_t owner, uint8_t offsetIn32Pages, bool enable) ;
typedef void (* WRAMSETOWNER_FUNC)(uint8_t page, uint8_t owner) ;
typedef void (* WRAMSETENABLE_FUNC)(uint8_t page, bool enable) ;
typedef uint8_t (* WRAMGETOWNER_FUNC)(uint8_t page) ;


#define REG_WRAM_SETA_CNT(n) (*(((volatile uint8_t *)0x04004040) + (n)))
#define REG_WRAM_SETB_CNT(n) (*(((volatile uint8_t *)0x04004044) + (n)))
#define REG_WRAM_SETC_CNT(n) (*(((volatile uint8_t *)0x0400404C) + (n)))
#define REG_WRAM_SETA_WINDOW (*((volatile uint32_t *)0x04004054))
#define REG_WRAM_SETB_WINDOW (*((volatile uint32_t *)0x04004058))
#define REG_WRAM_SETC_WINDOW (*((volatile uint32_t *)0x0400405C))
#define REG_WRAM_SETA_PROTECTION (*((volatile uint8_t *)0x04004060))
#define REG_WRAM_SETB_PROTECTION (*((volatile uint8_t *)0x04004061))
#define REG_WRAM_SETC_PROTECTION (*((volatile uint8_t *)0x04004062))

/*******************************************************************************

    Functions

*******************************************************************************/

void WRAMSetControl(uint8_t value) 
{
  *(volatile uint8_t *)0x04000247 = value ;
}

void WRAMConfigSetA(uint8_t page, uint8_t owner, uint8_t offsetIn32kPages, bool enable)
{
  REG_WRAM_SETA_CNT(page) = (owner & 3) | ((offsetIn32kPages / 2) << 2) | (enable?0x80:0x00) ;
}

void WRAMConfigSetB(uint8_t page, uint8_t owner, uint8_t offsetIn32kPages, bool enable)
{
  REG_WRAM_SETB_CNT(page) = (owner & 3) | (offsetIn32kPages << 2) | (enable?0x80:0x00) ;
}

void WRAMConfigSetC(uint8_t page, uint8_t owner, uint8_t offsetIn32kPages, bool enable)
{
  REG_WRAM_SETC_CNT(page) = (owner & 3) | (offsetIn32kPages << 2) | (enable?0x80:0x00) ;
}

void WRAMConfig(uint8_t set, uint8_t page, uint8_t owner, uint8_t offsetIn32kPages, bool enable)
{
  static const WRAMSETCONFIG_FUNC function[3] = { WRAMConfigSetA, WRAMConfigSetB, WRAMConfigSetC } ;
  function[set](page, owner, offsetIn32kPages, enable) ;
}

void WRAMSetOwnerSetA(uint8_t page, uint8_t owner)
{
  REG_WRAM_SETA_CNT(page) = (REG_WRAM_SETA_CNT(page) & 0xfe) | (owner & 3);
}

void WRAMSetOwnerSetB(uint8_t page, uint8_t owner)
{
  REG_WRAM_SETB_CNT(page) = (REG_WRAM_SETB_CNT(page) & 0xfe) | (owner & 3);
}

void WRAMSetOwnerSetC(uint8_t page, uint8_t owner)
{
  REG_WRAM_SETC_CNT(page) = (REG_WRAM_SETC_CNT(page) & 0xfe) | (owner & 3);
}

void WRAMSetOwner(uint8_t set, uint8_t page, uint8_t owner)
{
  static const WRAMSETOWNER_FUNC function[3] = { WRAMSetOwnerSetA, WRAMSetOwnerSetB, WRAMSetOwnerSetC } ;
  function[set](page, owner) ;
}

void WRAMSetEnableSetA(uint8_t page, bool enable)
{
  REG_WRAM_SETA_CNT(page) = (REG_WRAM_SETA_CNT(page) & ~0x80) | (enable?0x80:0x00);
}

void WRAMSetEnableSetB(uint8_t page, bool enable)
{
  REG_WRAM_SETB_CNT(page) = (REG_WRAM_SETB_CNT(page) & ~0x80) | (enable?0x80:0x00);
}

void WRAMSetEnableSetC(uint8_t page, bool enable)
{
  REG_WRAM_SETC_CNT(page) = (REG_WRAM_SETC_CNT(page) & ~0x80) | (enable?0x80:0x00);
}

void WRAMSetEnable(uint8_t set, uint8_t page, bool enable)
{
  static const WRAMSETENABLE_FUNC function[3] = { WRAMSetEnableSetA, WRAMSetEnableSetB, WRAMSetEnableSetC } ;
  function[set](page, enable) ;
}

uint8_t WRAMGetOwnerSetA(uint8_t page)
{
  return REG_WRAM_SETA_CNT(page) & 3 ; 
}

uint8_t WRAMGetOwnerSetB(uint8_t page)
{
  return REG_WRAM_SETB_CNT(page) & 3 ; 
}

uint8_t WRAMGetOwnerSetC(uint8_t page)
{
  return REG_WRAM_SETC_CNT(page) & 3 ; 
}

uint8_t WRAMGetOwner(uint8_t set, uint8_t page)
{
  static const WRAMGETOWNER_FUNC function[3] = { WRAMGetOwnerSetA, WRAMGetOwnerSetB, WRAMGetOwnerSetC } ;
  return function[set](page) ;
}

uint8_t WRAMGetOffsetSetA(uint8_t page)
{
  return ((REG_WRAM_SETA_CNT(page) & 0x7C) >> 2) * 2; 
}

uint8_t WRAMGetOffsetSetB(uint8_t page)
{
  return ((REG_WRAM_SETB_CNT(page) & 0x7C) >> 2) ; 
}

uint8_t WRAMGetOffsetSetC(uint8_t page)
{
  return ((REG_WRAM_SETC_CNT(page) & 0x7C) >> 2) ; 
}

bool WRAMIsEnabledSetA(uint8_t page)
{
  return REG_WRAM_SETA_CNT(page) & 0x80 ; 
}

bool WRAMIsEnabledSetB(uint8_t page)
{
  return REG_WRAM_SETB_CNT(page) & 0x80 ; 
}

bool WRAMIsEnabledSetC(uint8_t page)
{
  return REG_WRAM_SETC_CNT(page) & 0x80 ; 
}

bool WRAMIsEnabled(uint8_t set, uint8_t page)
{
  switch (set)
  {
    case 0:
      return WRAMIsEnabledSetA(page) ;
    case 1:
      return WRAMIsEnabledSetB(page) ;
    case 2:
      return WRAMIsEnabledSetC(page) ;
  }
  return false;
}

uint32_t WRAMGetWindowStartSetA()
{
  return ((REG_WRAM_SETA_WINDOW & 0xff0)  >> 4)* 0x10000 + 0x3000000 ;
}

uint32_t WRAMGetWindowStartSetB()
{
  return ((REG_WRAM_SETB_WINDOW & 0xff8) >> 3) * 0x8000 + 0x3000000 ;
}

uint32_t WRAMGetWindowStartSetC()
{
  return ((REG_WRAM_SETC_WINDOW & 0xff8) >> 3) * 0x8000 + 0x3000000 ;
}

uint32_t WRAMGetWindowEndSetA()
{
  return ((REG_WRAM_SETA_WINDOW & 0x1ff00000)  >> 0x14)* 0x10000 + 0x3000000 ;
}

uint32_t WRAMGetWindowEndSetB()
{
  return ((REG_WRAM_SETB_WINDOW & 0x1ff80000) >> 0x13) * 0x8000 + 0x3000000 ;
}

uint32_t WRAMGetWindowEndSetC()
{
  return ((REG_WRAM_SETC_WINDOW & 0x1ff80000) >> 0x13) * 0x8000 + 0x3000000 ;
}

uint32_t WRAMGetMirrorSizeSetA()
{
  uint8_t tmp = (uint8_t)((REG_WRAM_SETA_WINDOW & 0x3000)  >> 0x12) ;
  if (!tmp)
    return 1;
  return tmp;
}

uint32_t WRAMGetMirrorSizeSetB()
{
  return (uint8_t)((REG_WRAM_SETB_WINDOW & 0x3000)  >> 0x12) ;
}

uint32_t WRAMGetMirrorSizeSetC()
{
  return (uint8_t)((REG_WRAM_SETC_WINDOW & 0x3000)  >> 0x12) ;
}

bool WRAMGetWriteProtectionSetA(uint8_t page)
{
  return REG_WRAM_SETA_PROTECTION & (1 << (page & 0xff)) ;
}

bool WRAMGetWriteProtectionSetB(uint8_t page)
{
  return REG_WRAM_SETB_PROTECTION & (1 << (page & 0xff)) ;
}

bool WRAMGetWriteProtectionSetC(uint8_t page)
{
  return REG_WRAM_SETC_PROTECTION & (1 << (page & 0xff)) ;
}

void WRAMSetWriteProtectionSetA(uint8_t page, bool protect) 
{
  if (protect)
    REG_WRAM_SETA_PROTECTION |= (1 << (page & 0xff) );
  else
    REG_WRAM_SETA_PROTECTION &= ~(1 << (page & 0xff)) ;
}

void WRAMSetWriteProtectionSetB(uint8_t page, bool protect)
{
  if (protect)
    REG_WRAM_SETB_PROTECTION |= (1 << (page & 0xff) );
  else
    REG_WRAM_SETB_PROTECTION &= ~(1 << (page & 0xff)) ;
}

void WRAMSetWriteProtectionSetC(uint8_t page, bool protect)
{
  if (protect)
    REG_WRAM_SETC_PROTECTION |= (1 << (page & 0xff) );
  else
    REG_WRAM_SETC_PROTECTION &= ~(1 << (page & 0xff)) ;
}

void WRAMSetWriteProtection(uint8_t set, uint8_t page, bool protect)
{
  switch (set)
  {
    case 0:
      WRAMSetWriteProtectionSetA(page, protect) ;
      return ;
    case 1:
      WRAMSetWriteProtectionSetB(page, protect) ;
      return ;
    case 2:
      WRAMSetWriteProtectionSetC(page, protect) ;
      return ;
  }
}

void WRAMSetWindowSetA(uint32_t start, uint32_t end) 
{
  start -= 0x03000000 ;
  end -= 0x03000000;
  uint16_t sIndex = (start & 0x00FF0000) >> 16 ;
  uint16_t eIndex = (end & 0x01FF0000) >> 16 ;
  REG_WRAM_SETA_WINDOW = (sIndex << 4) | (eIndex << 20) | (1 << 12) ;
}

void WRAMSetWindowSetB(uint32_t start, uint32_t end) 
{
  start -= 0x03000000 ;
  end -= 0x03000000;
  uint16_t sIndex = (start & 0x00FF8000) >> 15 ;
  uint16_t eIndex = (end & 0x01FF8000) >> 15 ;
  REG_WRAM_SETB_WINDOW = (sIndex << 3) | (eIndex << 19) | (0 << 12) ;
}

void WRAMSetWindowSetC(uint32_t start, uint32_t end) 
{
  start -= 0x03000000 ;
  end -= 0x03000000;
  uint16_t sIndex = (start & 0x00FF8000) >> 15 ;
  uint16_t eIndex = (end & 0x01FF8000) >> 15 ;
  REG_WRAM_SETC_WINDOW = (sIndex << 3) | (eIndex << 19) | (0 << 12) ;
}

void WRAMSetWindow(uint8_t set, uint32_t start, uint32_t end) 
{
  switch (set)
  {
    case 0:
      WRAMSetWindowSetA(start, end) ;
      break;
    case 1:
      WRAMSetWindowSetB(start, end) ;
      break;
    case 2:
      WRAMSetWindowSetC(start, end) ;
      break;
  }
}
