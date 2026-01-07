
#include "xparameters.h"
#include "xaxivdma.h"
#include "xvidc.h"
#include "xvtc.h"
#include "xv_tpg.h"
#include "xil_printf.h"

int main() { 
    xil_printf("Hello World app_tpg_vdma_demo!\n\r");

    int Status;

    // Initialize VDMA
    XAxiVdma VdmaInstance;
    XAxiVdma_Config *VdmaConfig;
    VdmaConfig = XAxiVdma_LookupConfig(XPAR_AXI_VDMA_0_BASEADDR);
    if (!VdmaConfig) {
        xil_printf("No VDMA configuration found!\n\r");
        return XST_FAILURE;
    }
    Status = XAxiVdma_CfgInitialize(&VdmaInstance, VdmaConfig, VdmaConfig->BaseAddress);
    if (Status != XST_SUCCESS) {
        xil_printf("VDMA initialization failed!\n\r");
        return XST_FAILURE;
    }
    xil_printf("VDMA initialized successfully!\n\r");

    // Initialize VTC
    XVtc        VtcInstance;
    XVtc_Config *VtcConfig;
    VtcConfig = XVtc_LookupConfig(XPAR_XVTC_0_BASEADDR);
    if (!VtcConfig) {
        xil_printf("No VTC configuration found!\n\r");
        return XST_FAILURE;
    }
    Status = XVtc_CfgInitialize(&VtcInstance, VtcConfig, VtcConfig->BaseAddress);
    if (Status != XST_SUCCESS) {
        xil_printf("VTC initialization failed!\n\r");
        return XST_FAILURE;
    }
    xil_printf("VTC initialized successfully!\n\r");

    // Initialize TPG
    XV_tpg TpgInstance;
    XV_tpg_Config *TpgConfig;
    TpgConfig = XV_tpg_LookupConfig(XPAR_V_TPG_0_BASEADDR);
    if (!TpgConfig) {
        xil_printf("No TPG configuration found!\n\r");
        return XST_FAILURE;
    }
    Status = XV_tpg_CfgInitialize(&TpgInstance, TpgConfig, TpgConfig->BaseAddress);
    if (Status != XST_SUCCESS) {    
        xil_printf("TPG initialization failed!\n\r");
        return XST_FAILURE;
    }
    xil_printf("TPG initialized successfully!\n\r");

    return 0;
}