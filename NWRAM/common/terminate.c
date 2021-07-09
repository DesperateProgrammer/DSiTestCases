
#include "terminate.h"

#include "interrupt.h"
#ifdef ARM9
  #include "cp15.h"
#endif

void Terminate()
{
  CpuDisableIRQ() ;
  while (true)
  {
#ifdef ARM9
    cp15WaitForInterrupt() ;
#endif
  }
}
