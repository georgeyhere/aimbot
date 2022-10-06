#include "ov7670.h"


// Struct for default camera register values
static const ov7670_addr_data_t ov7670_defaultConfig [] = {

    // Set RGB565, 0-255 Output Range
    {OV7670_REG_COM7, OV7670_COM7_RGB},
    {OV7670_REG_RGB444, 0},
    {OV7670_REG_COM15, OV7670_COM15_RGB565 | OV7670_COM15_R00FF},

    // Disable Line Buffer Auto Output Window
    {OV7670_REG_TSLB, OV7670_TSLB_YLAST},    

    // Set default gamma values
    {OV7670_REG_SLOP, 0x20},
    {OV7670_REG_GAM_BASE, 0x1C},      {OV7670_REG_GAM_BASE + 1, 0x28},
    {OV7670_REG_GAM_BASE + 2, 0x3C},  {OV7670_REG_GAM_BASE + 3, 0x55},
    {OV7670_REG_GAM_BASE + 4, 0x68},  {OV7670_REG_GAM_BASE + 5, 0x76},
    {OV7670_REG_GAM_BASE + 6, 0x80},  {OV7670_REG_GAM_BASE + 7, 0x88},
    {OV7670_REG_GAM_BASE + 8, 0x8F},  {OV7670_REG_GAM_BASE + 9, 0x96},
    {OV7670_REG_GAM_BASE + 10, 0xA3}, {OV7670_REG_GAM_BASE + 11, 0xAF},
    {OV7670_REG_GAM_BASE + 12, 0xC4}, {OV7670_REG_GAM_BASE + 13, 0xD7},
    {OV7670_REG_GAM_BASE + 14, 0xE8},
    
    // Enable AEC and Banding Filter
    {OV7670_REG_COM8, OV7670_COM8_FASTAEC | OV7670_COM8_AECSTEP | OV7670_COM8_BANDING | 
                      OV7670_COM8_AGC     | OV7670_COM8_AEC},
    
    // Configure AGC / AEC
    {OV7670_REG_GAIN, 0x00},
    {OV7670_REG_COM9, 0x20},
    {OV7670_REG_AEW, 0x75}, 
    {OV7670_REG_AEB, 0x63},
    {OV7670_REG_VPT, 0xA5},
    {OV7670_REG_HAECC1, 0x78},
    {OV7670_REG_HAECC2, 0x68},

    // AGC / AEC Histogram-based Control
    {OV7670_REG_HAECC3, 0xDF}, 
    {OV7670_REG_HAECC4, 0xDF},
    {OV7670_REG_HAECC5, 0xF0},
    {OV7670_REG_HAECC6, 0x90},
    {OV7670_REG_HAECC7, 0x94},

    // Configure Banding Filter
    {OV7670_REG_BD50MAX, 0x05}, 
    {OV7670_REG_BD60MAX, 0x07},

    // Reset COM2, disable soft sleep mode
    {OV7670_COM2_SSLEEP, 0x00},

    // Enable full window 
    {OV7670_REG_COM4, 0x00},

    // Optical black line option
    {OV7670_REG_COM6, 0x4B},
    
    // Disable black sun
    {OV7670_REG_MVFP, 0x07}, 

    // Configure ADC (magic numbers)
    {OV7670_REG_ADCCTR1, 0x02},
    {OV7670_REG_ADCCTR2, 0x91},
    {OV7670_REG_ADC, 0x1D},
    {OV7670_REG_ACOM, 0x71},
    {OV7670_REG_OFON, 0x2A},
    
    // Array Current Control (magic numbers)
    {OV7670_REG_CHLF, 0x0B},
    
    // Set no HREF when VSYNC is low
    {OV7670_REG_COM12, 0x78},

    // RGB gain control
    {OV7670_REG_GFIX, 0x5D},

    // Digital gain control
    {OV7670_REG_REG74, 0x19},
    
    // Dummy Line low 8 bits
    {OV7670_REG_DM_LNL, 0x00},
    
    // Enable and Configure ABLC
    {OV7670_REG_ABLC1, 0x0C},
    {OV7670_REG_THL_ST, 0x82},
    
    // Lens Correction Config  
    {OV7670_REG_LCC3, 0x04},
    {OV7670_REG_LCC4, 0x20},
    {OV7670_REG_LCC5, 0x05},
    {OV7670_REG_LCC6, 0x04},
    {OV7670_REG_LCC7, 0x08},

    // AWB Control
    {OV7670_REG_AWBCTR0, 0x9F}, // Or use 0x9E for advance AWB
    {OV7670_REG_AWBCTR1, 0x11},
    {OV7670_REG_AWBCTR2, 0x55},
    {OV7670_REG_AWBCTR3, 0x0A},
    
    // Matrix coefficients (magic numbers)
    {OV7670_REG_MTX1, 0x80},
    {OV7670_REG_MTX2, 0x80},
    {OV7670_REG_MTX3, 0x00},
    {OV7670_REG_MTX4, 0x22},
    {OV7670_REG_MTX5, 0x5E},
    {OV7670_REG_MTX6, 0x80}, // 0x40?

    // Nrightness + Contrast Control 
    {OV7670_REG_BRIGHT, 0x00},
    {OV7670_REG_CONTRAS, 0x40},
    {OV7670_REG_CONTRAS_CENTER, 0x80}, // 0x40?

    // Reserved Registers (magic numbers)
    {OV7670_REG_AWBC1, 0x14}, {OV7670_REG_AWBC2, 0xF0}, {OV7670_REG_AWBC3, 0x34}, 
    {OV7670_REG_AWBC4, 0x58}, {OV7670_REG_AWBC5, 0x28}, {OV7670_REG_AWBC6, 0x3A},
    {OV7670_REG_COM5, 0x61}, 
    
    // More magic numbers
    {0xA1, 0x03}, {0x16, 0x02}, {0x29, 0x07}, {0x35, 0x0B},            
    {0x4D, 0x40}, {0x4E, 0x20}, {0x8D, 0x4F}, {0x8E, 0x00},            
    {0x8F, 0x00}, {0x90, 0x00}, {0x91, 0x00}, {0x96, 0x00},            
    {0x9A, 0x80}, {0xB0, 0x84}, {0xB2, 0x0E}, {0xB8, 0x0A},            
    {0x59, 0x88}, {0x5A, 0x88}, {0x5B, 0x44}, {0x5C, 0x67},            
    {0x5D, 0x49}, {0x5E, 0x0E},

    {OV7670_REG_LAST + 1, 0x00}       // End-of-data marker   
};


/*************************************** FUNCTIONS *********************************************/

/**
 * @brief Function to write an 8-bit value to an OV7670 register.
 * 
 * @param camInst is a pointer to an ov7670_t instance.
 * @param regAddr is the register address to write to.
 * @param data is the data to write to the specified register address.
 * @return int OV7670_STATUS_OK if successful, else OV7670_STATUS_ERROR
 */
int ov7670_writeReg(ov7670_t *camInst, uint8_t regAddr, uint8_t data)
{
    int status;
    uint8_t cmd [2];

    cmd[0] = regAddr;
    cmd[1] = data;

    // Send register address + data
    status = XIicPs_MasterSendPolled(camInst->iic, cmd, 2, camInst->address);
    if(status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    // Wait for the bus to idle before returning
    if(!(camInst->iic->IsRepeatedStart)) {
        while(XIicPs_BusIsBusy(camInst->iic));
    }

    return XST_SUCCESS;
}


/**
 * @brief Function to read an 8-bit value from an OV7670 register.
 * 
 * @param camInst is a pointer to an ov7670_t instance.
 * @return uint8_t register value.
 */
uint8_t ov7670_readReg(ov7670_t *camInst, uint8_t regAddr)
{
    int status;
    uint8_t recv = 0;

    // Set repeated start
    XIicPs_SetOptions(camInst->iic, XIICPS_REP_START_OPTION);

    // Send register address
    status = XIicPs_MasterSendPolled(camInst->iic, &regAddr, 1, camInst->address);
    if(status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    // Disable repeated start if it is enabled
    XIicPs_ClearOptions(camInst->iic, XIICPS_REP_START_OPTION);

    // Read a byte
    XIicPs_MasterRecvPolled(camInst->iic, &recv, 1, camInst->address);
    if(status != XST_SUCCESS) {
        return XST_FAILURE;
    }

    // Wait for bus to idle before returning
    while(XIicPs_BusIsBusy(camInst->iic));

    return recv;
}


