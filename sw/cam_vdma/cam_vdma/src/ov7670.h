#ifndef OV7670_H
#define OV7670_H

/*************************************** INCLUDES **********************************************/
#include <stdio.h>
#include "xiicps.h"

/************************************* IIC DEFINITIONS******************************************/


/*********************************** REGISTER DEFINITIONS **************************************/
#define OV7670_ADDR 0x21 //< Default I2C address if unspecified

#define OV7670_REG_GAIN                         (0x00U)  // AGC gain bits 7:0 (9:8 in VREF)
#define OV7670_REG_BLUE                         (0x01U)  // AWB blue channel gain
#define OV7670_REG_RED                          (0x02U)  // AWB red channel gain
#define OV7670_REG_VREF                         (0x03U)  // Vert frame control bits
#define OV7670_REG_COM1                         (0x04U)  // Common control 1
#define OV7670_COM1_R656                        (0x40U)  // COM1 enable R656 format
#define OV7670_REG_BAVE                         (0x05U)  // U/B average level
#define OV7670_REG_GbAVE                        (0x06U)  // Y/Gb average level
#define OV7670_REG_AECHH                        (0x07U)  // Exposure value - AEC 15:10 bits
#define OV7670_REG_RAVE                         (0x08U)  // V/R average level
#define OV7670_REG_COM2                         (0x09U)  // Common control 2
#define OV7670_COM2_SSLEEP                      (0x10U)  // COM2 soft sleep mode
#define OV7670_REG_PID                          (0x0AU)  // Product ID MSB (read-only)
#define OV7670_REG_VER                          (0x0BU)  // Product ID LSB (read-only)
#define OV7670_REG_COM3                         (0x0CU)  // Common control 3
#define OV7670_COM3_SWAP                        (0x40U)  // COM3 output data MSB/LSB swap
#define OV7670_COM3_SCALEEN                     (0x08U)  // COM3 scale enable
#define OV7670_COM3_DCWEN                       (0x04U)  // COM3 DCW enable
#define OV7670_REG_COM4                         (0x0DU)  // Common control 4
#define OV7670_REG_COM5                         (0x0EU)  // Common control 5
#define OV7670_REG_COM6                         (0x0FU)  // Common control 6
#define OV7670_REG_AECH                         (0x10U)  // Exposure value 9:2
#define OV7670_REG_CLKRC                        (0x11U)  // Internal clock
#define OV7670_CLK_EXT                          (0x40U)  // CLKRC Use ext clock directly
#define OV7670_CLK_SCALE                        (0x3FU)  // CLKRC Int clock prescale mask
#define OV7670_REG_COM7                         (0x12U)  // Common control 7
#define OV7670_COM7_RESET                       (0x80U)  // COM7 XIicPs register reset
#define OV7670_COM7_SIZE_MASK                   (0x38U)  // COM7 output size mask
#define OV7670_COM7_PIXEL_MASK                  (0x05U)  // COM7 output pixel format mask
#define OV7670_COM7_SIZE_VGA                    (0x00U)  // COM7 output size VGA
#define OV7670_COM7_SIZE_CIF                    (0x20U)  // COM7 output size CIF
#define OV7670_COM7_SIZE_QVGA                   (0x10U)  // COM7 output size QVGA
#define OV7670_COM7_SIZE_QCIF                   (0x08U)  // COM7 output size QCIF
#define OV7670_COM7_RGB                         (0x04U)  // COM7 pixel format RGB
#define OV7670_COM7_YUV                         (0x00U)  // COM7 pixel format YUV
#define OV7670_COM7_BAYER                       (0x01U)  // COM7 pixel format Bayer RAW
#define OV7670_COM7_PBAYER                      (0x05U)  // COM7 pixel fmt proc Bayer RAW
#define OV7670_COM7_COLORBAR                    (0x02U)  // COM7 color bar enable
#define OV7670_REG_COM8                         (0x13U)  // Common control 8
#define OV7670_COM8_FASTAEC                     (0x80U)  // COM8 Enable fast AGC/AEC algo,
#define OV7670_COM8_AECSTEP                     (0x40U)  // COM8 AEC step size unlimited
#define OV7670_COM8_BANDING                     (0x20U)  // COM8 Banding filter enable
#define OV7670_COM8_AGC                         (0x04U)  // COM8 AGC (auto gain) enable
#define OV7670_COM8_AWB                         (0x02U)  // COM8 AWB (auto white balance)
#define OV7670_COM8_AEC                         (0x01U)  // COM8 AEC (auto exposure) enable
#define OV7670_REG_COM9                         (0x14U)  // Common control 9 - max AGC value
#define OV7670_REG_COM10                        (0x15U)  // Common control 10
#define OV7670_COM10_HSYNC                      (0x40U)  // COM10 HREF changes to HSYNC
#define OV7670_COM10_PCLK_HB                    (0x20U)  // COM10 Suppress PCLK on hblank
#define OV7670_COM10_HREF_REV                   (0x08U)  // COM10 HREF reverse
#define OV7670_COM10_VS_EDGE                    (0x04U)  // COM10 VSYNC chg on PCLK rising
#define OV7670_COM10_VS_NEG                     (0x02U)  // COM10 VSYNC negative
#define OV7670_COM10_HS_NEG                     (0x01U)  // COM10 HSYNC negative
#define OV7670_REG_HSTART                       (0x17U)  // Horiz frame start high bits
#define OV7670_REG_HSTOP                        (0x18U)  // Horiz frame end high bits
#define OV7670_REG_VSTART                       (0x19U)  // Vert frame start high bits
#define OV7670_REG_VSTOP                        (0x1AU)  // Vert frame end high bits
#define OV7670_REG_PSHFT                        (0x1BU)  // Pixel delay select
#define OV7670_REG_MIDH                         (0x1CU)  // Manufacturer ID high byte
#define OV7670_REG_MIDL                         (0x1DU)  // Manufacturer ID low byte
#define OV7670_REG_MVFP                         (0x1EU)  // Mirror / vert-flip enable
#define OV7670_MVFP_MIRROR                      (0x20U)  // MVFP Mirror image
#define OV7670_MVFP_VFLIP                       (0x10U)  // MVFP Vertical flip
#define OV7670_REG_LAEC                         (0x1FU)  // Reserved
#define OV7670_REG_ADCCTR0                      (0x20U)  // ADC control
#define OV7670_REG_ADCCTR1                      (0x21U)  // Reserved
#define OV7670_REG_ADCCTR2                      (0x22U)  // Reserved
#define OV7670_REG_ADCCTR3                      (0x23U)  // Reserved
#define OV7670_REG_AEW                          (0x24U)  // AGC/AEC upper limit
#define OV7670_REG_AEB                          (0x25U)  // AGC/AEC lower limit
#define OV7670_REG_VPT                          (0x26U)  // AGC/AEC fast mode op region
#define OV7670_REG_BBIAS                        (0x27U)  // B channel signal output bias
#define OV7670_REG_GbBIAS                       (0x28U)  // Gb channel signal output bias
#define OV7670_REG_EXHCH                        (0x2AU)  // Dummy pixel insert MSB
#define OV7670_REG_EXHCL                        (0x2BU)  // Dummy pixel insert LSB
#define OV7670_REG_RBIAS                        (0x2CU)  // R channel signal output bias
#define OV7670_REG_ADVFL                        (0x2DU)  // Insert dummy lines MSB
#define OV7670_REG_ADVFH                        (0x2EU)  // Insert dummy lines LSB
#define OV7670_REG_YAVE                         (0x2FU)  // Y/G channel average value
#define OV7670_REG_HSYST                        (0x30U)  // HSYNC rising edge delay
#define OV7670_REG_HSYEN                        (0x31U)  // HSYNC falling edge delay
#define OV7670_REG_HREF                         (0x32U)  // HREF control
#define OV7670_REG_CHLF                         (0x33U)  // Array current control
#define OV7670_REG_ARBLM                        (0x34U)  // Array ref control - reserved
#define OV7670_REG_ADC                          (0x37U)  // ADC control - reserved
#define OV7670_REG_ACOM                         (0x38U)  // ADC & analog common - reserved
#define OV7670_REG_OFON                         (0x39U)  // ADC offset control - reserved
#define OV7670_REG_TSLB                         (0x3AU)  // Line buffer test option
#define OV7670_TSLB_NEG                         (0x20U)  // TSLB Negative image enable
#define OV7670_TSLB_YLAST                       (0x04U)  // TSLB UYVY or VYUY, see COM13
#define OV7670_TSLB_AOW                         (0x01U)  // TSLB Auto output window
#define OV7670_REG_COM11                        (0x3BU)  // Common control 11
#define OV7670_COM11_NIGHT                      (0x80U)  // COM11 Night mode
#define OV7670_COM11_NMFR                       (0x60U)  // COM11 Night mode frame rate mask
#define OV7670_COM11_HZAUTO                     (0x10U)  // COM11 Auto detect 50/60 Hz
#define OV7670_COM11_BAND                       (0x08U)  // COM11 Banding filter val select
#define OV7670_COM11_EXP                        (0x02U)  // COM11 Exposure timing control
#define OV7670_REG_COM12                        (0x3CU)  // Common control 12
#define OV7670_COM12_HREF                       (0x80U)  // COM12 Always has HREF
#define OV7670_REG_COM13                        (0x3DU)  // Common control 13
#define OV7670_COM13_GAMMA                      (0x80U)  // COM13 Gamma enable
#define OV7670_COM13_UVSAT                      (0x40U)  // COM13 UV saturation auto adj
#define OV7670_COM13_UVSWAP                     (0x01U)  // COM13 UV swap, use w TSLB[3]
#define OV7670_REG_COM14                        (0x3EU)  // Common control 14
#define OV7670_COM14_DCWEN                      (0x10U)  // COM14 DCW & scaling PCLK enable
#define OV7670_REG_EDGE                         (0x3FU)  // Edge enhancement adjustment
#define OV7670_REG_COM15                        (0x40U)  // Common control 15
#define OV7670_COM15_RMASK                      (0xC0U)  // COM15 Output range mask
#define OV7670_COM15_R10F0                      (0x00U)  // COM15 Output range 10 to F0
#define OV7670_COM15_R01FE                      (0x80U)  // COM15              01 to FE
#define OV7670_COM15_R00FF                      (0xC0U)  // COM15              00 to FF
#define OV7670_COM15_RGBMASK                    (0x30U)  // COM15 RGB 555/565 option mask
#define OV7670_COM15_RGB                        (0x00U)  // COM15 Normal RGB out
#define OV7670_COM15_RGB565                     (0x10U)  // COM15 RGB 565 output
#define OV7670_COM15_RGB555                     (0x30U)  // COM15 RGB 555 output
#define OV7670_REG_COM16                        (0x41U)  // Common control 16
#define OV7670_COM16_AWBGAIN                    (0x08U)  // COM16 AWB gain enable
#define OV7670_REG_COM17                        (0x42U)  // Common control 17
#define OV7670_COM17_AECWIN                     (0xC0U)  // COM17 AEC window must match COM4
#define OV7670_COM17_CBAR                       (0x08U)  // COM17 DSP Color bar enable
#define OV7670_REG_AWBC1                        (0x43U)  // Reserved
#define OV7670_REG_AWBC2                        (0x44U)  // Reserved
#define OV7670_REG_AWBC3                        (0x45U)  // Reserved
#define OV7670_REG_AWBC4                        (0x46U)  // Reserved
#define OV7670_REG_AWBC5                        (0x47U)  // Reserved
#define OV7670_REG_AWBC6                        (0x48U)  // Reserved
#define OV7670_REG_REG4B                        (0x4BU)  // UV average enable
#define OV7670_REG_DNSTH                        (0x4CU)  // De-noise strength
#define OV7670_REG_MTX1                         (0x4FU)  // Matrix coefficient 1
#define OV7670_REG_MTX2                         (0x50U)  // Matrix coefficient 2
#define OV7670_REG_MTX3                         (0x51U)  // Matrix coefficient 3
#define OV7670_REG_MTX4                         (0x52U)  // Matrix coefficient 4
#define OV7670_REG_MTX5                         (0x53U)  // Matrix coefficient 5
#define OV7670_REG_MTX6                         (0x54U)  // Matrix coefficient 6
#define OV7670_REG_BRIGHT                       (0x55U)  // Brightness control
#define OV7670_REG_CONTRAS                      (0x56U)  // Contrast control
#define OV7670_REG_CONTRAS_CENTER               (0x57U)  // Contrast center
#define OV7670_REG_MTXS                         (0x58U)  // Matrix coefficient sign
#define OV7670_REG_LCC1                         (0x62U)  // Lens correction option 1
#define OV7670_REG_LCC2                         (0x63U)  // Lens correction option 2
#define OV7670_REG_LCC3                         (0x64U)  // Lens correction option 3
#define OV7670_REG_LCC4                         (0x65U)  // Lens correction option 4
#define OV7670_REG_LCC5                         (0x66U)  // Lens correction option 5
#define OV7670_REG_MANU                         (0x67U)  // Manual U value
#define OV7670_REG_MANV                         (0x68U)  // Manual V value
#define OV7670_REG_GFIX                         (0x69U)  // Fix gain control
#define OV7670_REG_GGAIN                        (0x6AU)  // G channel AWB gain
#define OV7670_REG_DBLV                         (0x6BU)  // PLL & regulator control
#define OV7670_REG_AWBCTR3                      (0x6CU)  // AWB control 3
#define OV7670_REG_AWBCTR2                      (0x6DU)  // AWB control 2
#define OV7670_REG_AWBCTR1                      (0x6EU)  // AWB control 1
#define OV7670_REG_AWBCTR0                      (0x6FU)  // AWB control 0
#define OV7670_REG_SCALING_XSC                  (0x70U)  // Test pattern X scaling
#define OV7670_REG_SCALING_YSC                  (0x71U)  // Test pattern Y scaling
#define OV7670_REG_SCALING_DCWCTR               (0x72U)  // DCW control
#define OV7670_REG_SCALING_PCLK_DIV             (0x73U)  // DSP scale control clock divide
#define OV7670_REG_REG74                        (0x74U)  // Digital gain control
#define OV7670_REG_REG76                        (0x76U)  // Pixel correction
#define OV7670_REG_SLOP                         (0x7AU)  // Gamma curve highest seg slope
#define OV7670_REG_GAM_BASE                     (0x7BU)  // Gamma register base (1 of 15)
#define OV7670_GAM_LEN                          (15)     // Number of gamma registers
#define OV7670_R76_BLKPCOR                      (0x80U)  // REG76 black pixel corr enable
#define OV7670_R76_WHTPCOR                      (0x40U)  // REG76 white pixel corr enable
#define OV7670_REG_RGB444                       (0x8CU)  // RGB 444 control
#define OV7670_R444_ENABLE                      (0x02U)  // RGB444 enable
#define OV7670_R444_RGBX                        (0x01U)  // RGB444 word format
#define OV7670_REG_DM_LNL                       (0x92U)  // Dummy line LSB
#define OV7670_REG_LCC6                         (0x94U)  // Lens correction option 6
#define OV7670_REG_LCC7                         (0x95U)  // Lens correction option 7
#define OV7670_REG_HAECC1                       (0x9FU)  // Histogram-based AEC/AGC ctrl 1
#define OV7670_REG_HAECC2                       (0xA0U)  // Histogram-based AEC/AGC ctrl 2
#define OV7670_REG_SCALING_PCLK_DELAY           (0xA2U)  // Scaling pixel clock delay
#define OV7670_REG_BD50MAX                      (0xA5U)  // 50 Hz banding step limit
#define OV7670_REG_HAECC3                       (0xA6U)  // Histogram-based AEC/AGC ctrl 3
#define OV7670_REG_HAECC4                       (0xA7U)  // Histogram-based AEC/AGC ctrl 4
#define OV7670_REG_HAECC5                       (0xA8U)  // Histogram-based AEC/AGC ctrl 5
#define OV7670_REG_HAECC6                       (0xA9U)  // Histogram-based AEC/AGC ctrl 6
#define OV7670_REG_HAECC7                       (0xAAU)  // Histogram-based AEC/AGC ctrl 7
#define OV7670_REG_BD60MAX                      (0xABU)  // 60 Hz banding step limit
#define OV7670_REG_ABLC1                        (0xB1U)  // ABLC enable
#define OV7670_REG_THL_ST                       (0xB3U)  // ABLC target
#define OV7670_REG_SATCTR                       (0xC9U)  // Saturation control

#define OV7670_REG_LAST OV7670_REG_SATCTR //< Maximum register address

/************************************* EXTERNAL VARS *******************************************/

/**************************************** STRUCTS **********************************************/

// Supported resolutions
typedef enum {
    OV7670_RES_640 = 0, // 640x480
    OV7670_RES_320,     // 320x240
    OV7670_RES_160,     // 160x120
    OV7670_RES_80,      // 80x60
    OV7670_RES_40       // 40x30
} ov7670_res;

// Test pattern definitions
typedef enum {
    OV7670_TEST_PATTERN_NONE = 0,      // No test pattern
    OV7670_TEST_PATTERN_SHIFTING_1,    // 'Shifting 1' pattern
    OV7670_TEST_PATTERN_COLOR_BAR,     // 8 color bars
    OV7670_TEST_PATTERN_COLOR_BAR_FADE // color bars w/ fade to white
} ov7670_pattern;

// Night mode definitions
typedef enum {
    OV7670_NIGHT_MODE_OFF = 0,
    OV7670_NIGHT_MODE_HALF,
    OV7670_NIGHT_MODE_QUARTER,
    OV7670_NIGHT_MODE_EIGHTH
} ov7670_nightMode;

// Struct to store register address - value pairings
typedef struct {
    uint8_t addr;  // register address
    uint8_t value; // register data
} ov7670_addr_data_t;

// Device struct
typedef struct {
    ov7670_res        res;
    ov7670_pattern    pattern;
    ov7670_nightMode  nightMode;

    uint8_t           address;
    XIicPs            *iic;
} ov7670_t;

/*********************************** FUNCTION PROTOTYPES ***************************************/

/**
 * @brief Function to write an 8-bit value to an OV7670 register.
 * 
 * @param camInst is a pointer to an ov7670_t instance.
 * @param regAddr is the register address to write to.
 * @param data is the data to write to the specified register address.
 * @return int OV7670_STATUS_OK if successful, else OV7670_STATUS_ERROR
 */
int ov7670_writeReg(ov7670_t *camInst, uint8_t regAddr, uint8_t data);

/**
 * @brief Function to read an 8-bit value from an OV7670 register.
 * 
 * @param camInst is a pointer to an ov7670_t instance.
 * @param regAddr is the register address to read from.
 * @return uint8_t register value.
 */
uint8_t ov7670_readReg(ov7670_t *camInst, uint8_t regAddr);

/**
 * @brief Function to initialize an OV7670 instance to default values.
 * 
 * @param camInst is a pointer to an ov7670_t instance.
 * @param iicInst is a pointer to an XIicPs instance.
 * @param addr is the IIC address of the ov7670.
 * @return int OV7670_STATUS_OK if successful, else OV7670_STATUS_ERROR
 */
int ov7670_initialize(ov7670_t *camInst, XIicPs *iicInst, uint8_t addr);



#endif
