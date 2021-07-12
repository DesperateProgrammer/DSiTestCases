// CoProc 15 related

#pragma once

#include <stdint.h>
#include <stdbool.h>

void cp15DisableMPU() ;

void cp15InitRegions() ;

/** Set protection region register 0..7 */
void cp15SetProtectionRegionRegister0(uint32_t value) ;
void cp15SetProtectionRegionRegister1(uint32_t value) ;
void cp15SetProtectionRegionRegister2(uint32_t value) ;
void cp15SetProtectionRegionRegister3(uint32_t value) ;
void cp15SetProtectionRegionRegister4(uint32_t value) ;
void cp15SetProtectionRegionRegister5(uint32_t value) ;
void cp15SetProtectionRegionRegister6(uint32_t value) ;
void cp15SetProtectionRegionRegister7(uint32_t value) ;

void cp15SetRegionsDataCachableRegister(uint32_t value) ;
void cp15SetRegionsInstructionCachableRegister(uint32_t value) ;

void cp15SetRegionsDataBufferableRegister(uint32_t value) ;

void cp15SetRegionsInstructionAccessRegister(uint32_t value) ;
void cp15SetRegionsDataAccessRegister(uint32_t value) ;

/** Disable caching for the arm9 */
bool cp15DisableCache() ;

/** Disable instruction caching */
bool cp15DisableInstructionCache() ;

/** Invalidate instruction cache */
void cp15InvalidateInstructionCache() ;

/** Invalidate data cache */
void cp15InvalidateDataCache() ;

/** Clear a cache entry by set and index */
void cp15ClearDataCachlineByIndex(uint8_t set, uint8_t index) ;

/** Clear all data cache entries */
void cp15ClearAllDataCachelines() ;

/** Clear all data cache entries after writing back */
void cp15FlushAndInvalidateAllDataCachelines() ;

/** Invalidate data cache for address */
void cp15InvalidateDatacacheByAddress(uint32_t address) ;

/** Invalidate data cache for address range*/
void cp15InvalidateDatacacheByAddressRange(uint32_t address, uint32_t length) ;

/** Invalidate instruction cache for address */
void cp15InvalidateInstructionCacheByAddress(uint32_t address) ;

/** Invalidate instruction cache for address range*/
void cp15InvalidateInstructionCacheByAddressRange(uint32_t address, uint32_t length) ;

/** Clean data cache for address */
void cp15CleanDatacacheByAddress(uint32_t address) ;

/** Clean data cache for address range*/
void cp15CleanDatacacheByAddressRange(uint32_t address, uint32_t length) ;

/** Clean data cache for address range*/
void cp15FlushAndInvalidateDatacacheByAddressRange(uint32_t address, uint32_t length) ;

/** write back all */
void cp15DrainWriteBuffer() ;

void cp15WriteTagToAllDataCacheLines(uint32_t tag) ;
void cp15ClearTagOfAllDataCacheLines() ;
void cp15WriteToAllDataCacheLines(uint32_t data) ;
void cp15ClearOfAllDataCache() ;
void cp15WriteTagToAllinstructionCacheLines(uint32_t tag) ;
void cp15ClearTagOfAllInstructionCacheLines() ;
void cp15WriteToAllInstructionCacheLines(uint32_t data) ;
void cp15ClearOfAllInstructionCache() ;

void cp15WaitForInterrupt() ;
