#include "kernel.h"
#include "sync.h"
#include "io.h"
#include "user_app.h"


int main()
{
    os_init();
    user_config();
    
    os_start();
    
    while (1);
    
    return 0;
}
