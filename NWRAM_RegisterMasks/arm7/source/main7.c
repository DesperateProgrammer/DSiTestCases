/*---------------------------------------------------------------------------------

	default ARM7 core

		Copyright (C) 2005 - 2010
		Michael Noland (joat)
		Jason Rogers (dovoto)
		Dave Murphy (WinterMute)

	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any
	damages arising from the use of this software.

	Permission is granted to anyone to use this software for any
	purpose, including commercial applications, and to alter it and
	redistribute it freely, subject to the following restrictions:

	1.	The origin of this software must not be misrepresented; you
		must not claim that you wrote the original software. If you use
		this software in a product, an acknowledgment in the product
		documentation would be appreciated but is not required.

	2.	Altered source versions must be plainly marked as such, and
		must not be misrepresented as being the original software.

	3.	This notice may not be removed or altered from any source
		distribution.

---------------------------------------------------------------------------------*/
#include "sdk.h"
#include "wram.post.h"

#include <nds.h>

void isrIpcSync()
{
}

int main() {
  IpcInitialize();
  CpuDisableIRQ();
  
  SetIPCSyncValue(7) ;
  WaitForIPCSyncValue(7) ;
  
  /*******************************************
  *    Test IPC Sync                         *
  *******************************************/
  // check all values in range
  for (int i=0;i<16;i++)
  {
    WaitForIPCSyncValue(i) ;
    SetIPCSyncValue(i) ;
  }

  /*******************************************
  *    WRAM Control                          *
  *******************************************/
  SWRAMPOSTRESULTS wramPostResult ;
  WRAMSelfTest(&wramPostResult) ;
  *((volatile uint32_t *)0x04004008) &= ~(1 << 25) ;
  WRAMSelfTest(&wramPostResult) ;
  

  
  int count = 0 ;

	while (1) {
    count++ ;
	}
  
	Terminate();
}
