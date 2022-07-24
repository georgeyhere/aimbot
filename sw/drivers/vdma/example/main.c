
#include "tpg_utilities.h"
#include "vdma_api.h"

unsigned int srcBuffer = (XPAR_PS7_DDR_0_S_AXI_BASEADDR + 0x100000);

int main()
{
	int status;
	

	myTpg    tpgInst;
	XAxiVdma vdmaInst;

	/* Sanity check */
	printf("ARM0 initialized!\r\n");

	/* Initialize and enable the TPG. */
	status = tpg_initialize(&tpgInst, XPAR_V_TPG_0_DEVICE_ID);
	if(status != XST_SUCCESS) {
		printf("TPG initialization failed!\r\n");
		return XST_FAILURE;
	}
	XV_tpg_Start(&tpgInst.Tpg);

	/* Start continuous VDMA transfers. */
	status = vdma_run_triple_buffer(&vdmaInst, 0, 800, 600, srcBuffer, 100, 0);
	if(status != XST_SUCCESS) {
		printf("VDMA failed! Error: %d \r\n", status);
		return XST_FAILURE;
	}
	else {
		printf("VDMA continuous transfers started. \r\n");
	}


    while(1) {
    	XV_tpg_Set_bckgndId(&tpgInst.Tpg, tpgInst.BackgroundId);
    	printf("TPG background ID: %d \r\n", tpgInst.BackgroundId);
    	printf("Transferring frames...\r\n\n");

    	sleep(1);
    	if (tpgInst.BackgroundId < 20) {
    		tpgInst.BackgroundId++;
    	}
    	else {
    		tpgInst.BackgroundId = 0;
    	}
    }

	return 0;
}
