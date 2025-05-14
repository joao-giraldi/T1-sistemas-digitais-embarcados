#include "config.h"
#include "kernel.h"
#include "sync.h"
#include "io.h"
#include "user_app.h"
#include "mem.h"



int main()
{
    os_init();
    if(DYNAMIC_MEM == ON){SRAMInitHeap();}
    user_config();
    
    os_start();
    
    while (1);
    
    return 0;
}
