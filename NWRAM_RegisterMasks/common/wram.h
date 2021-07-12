// WRAM related 

#pragma once

#include <stdint.h>
#include <stdbool.h>

#define WRAM_OWNER_ARM9                   0
#define WRAM_OWNER_ARM7                   1
#define WRAM_OWNER_DSP                    2

#define WRAM_CONFIG_OFFSETINBLOCKS(n)     ((n) / (32 * 1024))

#define WRAM_SET_A                        0
#define WRAM_SET_B                        1
#define WRAM_SET_C                        2

void WRAMSetControl(uint8_t value) ;

void WRAMConfigSetA(uint8_t page, uint8_t owner, uint8_t offsetIn32kPages, bool enable) ;
void WRAMConfigSetB(uint8_t page, uint8_t owner, uint8_t offsetIn32kPages, bool enable) ;
void WRAMConfigSetC(uint8_t page, uint8_t owner, uint8_t offsetIn32kPages, bool enable) ;
void WRAMConfig(uint8_t set, uint8_t page, uint8_t owner, uint8_t offsetIn32kPages, bool enable) ;


void WRAMSetOwnerSetA(uint8_t page, uint8_t owner) ;
void WRAMSetOwnerSetB(uint8_t page, uint8_t owner) ;
void WRAMSetOwnerSetC(uint8_t page, uint8_t owner) ;
void WRAMSetOwner(uint8_t set, uint8_t page, uint8_t owner) ;


void WRAMSetEnableSetA(uint8_t page, bool enable) ;
void WRAMSetEnableSetB(uint8_t page, bool enable) ;
void WRAMSetEnableSetC(uint8_t page, bool enable) ;
void WRAMSetEnable(uint8_t set, uint8_t page, bool enable) ;


uint8_t WRAMGetOwnerSetA(uint8_t page) ;
uint8_t WRAMGetOwnerSetB(uint8_t page) ;
uint8_t WRAMGetOwnerSetC(uint8_t page) ;
uint8_t WRAMGetOwner(uint8_t set, uint8_t page) ;


uint8_t WRAMGetOffsetSetA(uint8_t page) ;
uint8_t WRAMGetOffsetSetB(uint8_t page) ;
uint8_t WRAMGetOffsetSetC(uint8_t page) ;


bool WRAMIsEnabledSetA(uint8_t page) ;
bool WRAMIsEnabledSetB(uint8_t page) ;
bool WRAMIsEnabledSetC(uint8_t page) ;
bool WRAMIsEnabled(uint8_t set, uint8_t page) ;


uint32_t WRAMGetWindowStartSetA() ;
uint32_t WRAMGetWindowStartSetB() ;
uint32_t WRAMGetWindowStartSetC() ;

uint32_t WRAMGetWindowEndSetA() ;
uint32_t WRAMGetWindowEndSetB() ;
uint32_t WRAMGetWindowEndSetC() ;

void WRAMSetWindowSetA(uint32_t start, uint32_t end) ;
void WRAMSetWindowSetB(uint32_t start, uint32_t end) ;
void WRAMSetWindowSetC(uint32_t start, uint32_t end) ;
void WRAMSetWindow(uint8_t set, uint32_t start, uint32_t end) ;

uint32_t WRAMGetMirrorSizeSetA() ;
uint32_t WRAMGetMirrorSizeSetB() ;
uint32_t WRAMGetMirrorSizeSetC() ;


bool WRAMGetWriteProtectionSetA(uint8_t page) ;
bool WRAMGetWriteProtectionSetB(uint8_t page) ;
bool WRAMGetWriteProtectionSetC(uint8_t page) ;

void WRAMSetWriteProtectionSetA(uint8_t page, bool protect) ;
void WRAMSetWriteProtectionSetB(uint8_t page, bool protect) ;
void WRAMSetWriteProtectionSetC(uint8_t page, bool protect) ;
void WRAMSetWriteProtection(uint8_t set, uint8_t page, bool protect) ;
