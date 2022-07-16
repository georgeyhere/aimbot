#include "xparameters.h"
#include "xv_tpg.h"
#include "xvidc.h"
#include "sleep.h"

#define TPG_DEVICEID XPAR_V_TPG_0_DEVICE_ID

typedef struct {
	u32 height;
	u32 width;
	u32 colorFormat;
	u32 maskId;
	u32 motionSpeed;
	u32 backgroundId;
} tpgSettings;

int main()
{
	u32         Status;
	XV_tpg      tpgInst;
	tpgSettings tpgSettingsInst;

	tpgSettingsInst.height       = 600;
	tpgSettingsInst.width        = 800;
	tpgSettingsInst.colorFormat  = XVIDC_CSF_RGB;
	tpgSettingsInst.maskId       = 0x0;
	tpgSettingsInst.motionSpeed  = 0x4;
	tpgSettingsInst.backgroundId = XTPG_BKGND_CHECKER_BOARD;

	printf("\r\nARM0 initialized!\r\n");
	XV_tpg_Initialize(&tpgInst, TPG_DEVICEID);

	Status = XV_tpg_IsReady(&tpgInst);
	printf("TPG Status: %u\r\n", Status);

	Status = XV_tpg_IsIdle(&tpgInst);
	printf("TPG Status: %u\r\n", Status);

	XV_tpg_Set_height      (&tpgInst, tpgSettingsInst.height);
	XV_tpg_Set_width       (&tpgInst, tpgSettingsInst.width);
	XV_tpg_Set_colorFormat (&tpgInst, tpgSettingsInst.colorFormat);
	XV_tpg_Set_maskId      (&tpgInst, tpgSettingsInst.maskId);
	XV_tpg_Set_motionSpeed (&tpgInst, tpgSettingsInst.motionSpeed);

	XV_tpg_EnableAutoRestart(&tpgInst);
	XV_tpg_Start(&tpgInst);
	printf("Test Pattern Generator initialized!\r\n");

	while(1)
	{
		XV_tpg_Set_bckgndId(&tpgInst, tpgSettingsInst.backgroundId);

		sleep(2);

		if(tpgSettingsInst.backgroundId < 20) tpgSettingsInst.backgroundId++;
		else tpgSettingsInst.backgroundId = 0;
	}
}
