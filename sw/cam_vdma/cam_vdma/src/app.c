#include "app.h"

ov7670_t camInst;
XIicPs   iicInst;
XAxiVdma vdmaInst;
myTpg    tpgInst;

unsigned int srcBuffer = (XPAR_PS7_DDR_0_S_AXI_BASEADDR + 0x100000);

int setup()
{
    int status;
    uint16_t pid = 0;
    
    printf("Initializing XIicPs...\r\n");
    status = ov7670_iic_initialize(&camInst, &iicInst, XIIC_DEVICE_ID, XIIC_SCLK_RATE);
    if(status != XST_SUCCESS) {
        printf("XIicPs initialization failed!\r\n");
        return XST_FAILURE;
    }
    printf("XIicPs initialized!\r\n\n");

/*
    printf("Looking for OV7670 Product ID...");
    pid = ov7670_get_id(&camInst);
    printf("\r\nOV7670 Product ID: %d\r\n", pid);

    printf("Initializing OV7670...\r\n");
    status = ov7670_cfg_initialize(&camInst);
    if(status != XST_SUCCESS) {
        printf("OV7670 initialization failed!\r\n");
        //return XST_FAILURE;
    }

    printf("Initializing VDMA...\r\n");
    status = vdma_run_triple_buffer(&vdmaInst, 0, 640, 480, srcBuffer, 100, 0);
    if(status != XST_SUCCESS) {
        printf("VDMA initialization failed!\r\n");
        return XST_FAILURE;
    }
*/
    printf("Initializing TPG...\r\n");
    status = tpg_initialize(&tpgInst, XPAR_V_TPG_0_DEVICE_ID);
    if(status != XST_SUCCESS) {
    	printf("TPG initialization failed!\r\n");
    	return XST_FAILURE;
    }
    XV_tpg_Start(&tpgInst.Tpg);
    printf("TPG initialized!\r\n\n");

    printf("\r\nVideo Processing System Initialized!\r\n");
    return XST_SUCCESS;
}









