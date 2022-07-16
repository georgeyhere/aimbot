/* @file vdma_api.c
 *
 * https://github.com/Xilinx/embeddedsw/blob/master/XilinxProcessorIPLib/drivers/axivdma/examples/vdma_api.c
 *
 */
/******************************* Include Files **************************************/
#include "vdma_api.h"


/***************************** Function Definitions *********************************/

/**
 * Function to configure the read channel of the VDMA.
 *
 * @param vdma_context is the context pointer to the VDMA.
 * @return XST_SUCCESS if successful, else XST_FAILURE.
 */
static int vdma_read_setup(VdmaHandle *vdma_context)
{
	u32 addr;
	int status;

	/* Initialize ReadCfg. */
	vdma_context->ReadCfg.VertSizeInput       = vdma_context->vsize;
	vdma_context->ReadCfg.HoriSizeInput       = vdma_context->hsize;
	vdma_context->ReadCfg.Stride              = vdma_context->hsize;
	vdma_context->ReadCfg.FrameDelay          = 0;
	vdma_context->ReadCfg.EnableCircularBuf   = 1;
	vdma_context->ReadCfg.EnableSync          = 1; // Gen-lock
	vdma_context->ReadCfg.PointNum            = 0;
	vdma_context->ReadCfg.EnableFrameCounter  = 0;
	vdma_context->ReadCfg.FixedFrameStoreAddr = 0;

	/* Apply ReadCfg to the VDMA instance. */
	status = XAxiVdma_DmaConfig(vdma_context->InstancePtr, XAXIVDMA_READ, &vdma_context->ReadCfg);
	if(status != XST_SUCCESS) {
		printf("VDMA read channel config failed: %d \r\n", status);
		return XST_FAILURE;
	}

	/* Initialize memory addresses for the triple frame buffers. */
	addr = vdma_context->buffer_address;
	for(int index=0; index<vdma_context->InstancePtr->MaxNumFrames; index++) {
		vdma_context->ReadCfg.FrameStoreStartAddr[index] = addr;

#if DEBUG_MODE
		{
			u8  *src;
			u32 total_pixel;

			total_pixel = vdma_context->stride * vdma_context->vsize;
			src = (unsigned char *) addr;
			printf("Read Buffer %d address: 0x%x \r\n", index, addr);
			for(u32 i; i<total_pixel; i++) {
				src[i] = i & 0xFF;
			}
		}
#endif
		addr += vdma_context->hsize * vdma_context->vsize;
	}

	/* Apply the frame buffer addresses to the VDMA instance. */
	status = XAxiVdma_DmaSetBufferAddr(vdma_context->InstancePtr, XAXIVDMA_READ,
			                           vdma_context->ReadCfg.FrameStoreStartAddr);
	if(status != XST_SUCCESS) {
		printf("VDMA read channel buffer address init failed: %d \r\n", status);
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}


/**
 * Function to configure the write channel of the VDMA.
 *
 * @param vdma_context is the context pointer to the VDMA.
 * @return XST_SUCCESS if successful, else XST_FAILURE.
 */
static int vdma_write_setup(VdmaHandle *vdma_context)
{
	u32 addr;
	int status;

	/* Initialize WriteCfg. */
	vdma_context->WriteCfg.VertSizeInput       = vdma_context->vsize;
	vdma_context->WriteCfg.HoriSizeInput       = vdma_context->hsize;
	vdma_context->WriteCfg.Stride              = vdma_context->hsize;
	vdma_context->WriteCfg.FrameDelay          = 0;
	vdma_context->WriteCfg.EnableCircularBuf   = 1;
	vdma_context->WriteCfg.EnableSync          = 0;
	vdma_context->WriteCfg.PointNum            = 0;
	vdma_context->WriteCfg.EnableFrameCounter  = 0; // endless transfers
	vdma_context->WriteCfg.FixedFrameStoreAddr = 0;

	/* Apply WriteCfg to the VDMA instance. */
	status = XAxiVdma_DmaConfig(vdma_context->InstancePtr, XAXIVDMA_WRITE, &vdma_context->WriteCfg);
	if(status != XST_SUCCESS) {
		printf("VDMA write channel config failed: %d \r\n", status);
		return XST_FAILURE;
	}


}























