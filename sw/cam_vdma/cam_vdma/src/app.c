#include "app.h"

ov7670_t camInst;
XIicPs   iicInst;
XAxiVdma vdmaInst;

unsigned int srcBuffer = (XPAR_PS7_DDR_0_S_AXI_BASEADDR + 0x100000);

int setup()
{
    int status;
    
    printf("Initializing XIicPs...\r\n");
    status = ov7670_iic_initialize(&camInst, &iicInst, XIIC_DEVICE_ID, XIIC_SCLK_RATE);
    if(status != XST_SUCCESS) {
        printf("XIicPs initialization failed!\r\n");
        return XST_FAILURE;
    }

    printf("Initializing OV7670...\r\n");
    status = ov7670_cfg_initialize(&camInst);
    if(status != XST_SUCCESS) {
        printf("OV7670 initialization failed!\r\n");
        return XST_FAILURE;
    } 

    printf("Initializing VDMA...\r\n");
    status = vdma_run_triple_buffer(&vdmaInst, 0, 640, 480, srcBuffer, 100, 0);
    if(status != XST_SUCCESS) {
        printf("VDMA initialization failed!\r\n");
        return XST_FAILURE;
    }
}









