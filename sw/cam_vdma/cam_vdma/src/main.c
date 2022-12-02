#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"


int main()
{
    init_platform();
    printf("\r\nDevice Initialized!\r\n");

    setup();

    while(1){
        
    }

    cleanup_platform();
    return 0;
}
