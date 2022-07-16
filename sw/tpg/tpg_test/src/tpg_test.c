#include "xparameters.h"
#include "tpg_utilities.h"
#include "sleep.h"

#define TPG_DEVICEID        XPAR_V_TPG_0_DEVICE_ID

int main()
{
	int status;

	myTpg tpgInst;

	/* Sanity check */
	printf("ARM0 initialized!\r\n");

	/* Initialize and enable the TPG. */
	status = tpg_initialize(&tpgInst, XPAR_V_TPG_0_DEVICE_ID);
	if (status != XST_SUCCESS) {
		printf("TPG initialization failed!\r\n");
		return XST_FAILURE;
	}
	XV_tpg_Start(&tpgInst.Tpg);

	while(1)
	{
		XV_tpg_Set_bckgndId(&tpgInst.Tpg, tpgInst.BackgroundId);

		sleep(2);

		if (tpgInst.BackgroundId < 20) tpgInst.BackgroundId++;
		else tpgInst.BackgroundId = 0;
	}

}
