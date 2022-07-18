
#include "tpg_utilities.h"
#include "vdma_api.h"

#define TPG_DEVICEID        XPAR_V_TPG_0_DEVICE_ID

int main()
{
	int status;
	unsigned int srcBuffer = (XPAR_PS7_DDR_0_S_AXI_BASEADDR + 0x100000);

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
		printf("Transfer of frames failed with error: %d \r\n", status);
		return XST_FAILURE;
	}
	else {
		printf("Transfer of frames started. \r\n");
	}

	return 0;
}
