# OV7670_capture

## Overview
The OV7670_capture IP core captures video from an OV7670 which can be processed by downstream video processing cores. The core features parameterized capture resolution, AXI Stream output, and independent clock support.
<br />
<br />

## Performance 

#### Maximum Frequencies
There are no IP specific restrictions on the maximum achievable clock frequency. The restriction is dependent on if the design can meet timing or not. 

#### Resource Utilization


## Port Descriptions
| Signal Name         | Direction | Width | Description                   |
|---------------------|-----------|-------|-------------------------------|
| i_resetn            | In        | 1     | Asynchronous active-low reset |
| i_enable            | In        | 1     | Active-high enable            |
| i_pclk              | In        | 1     | OV7670 Pixel Clock            |
| i_vsync             | In        | 1     | OV7670 vsync                  |
| i_href              | In        | 1     | OV7670 href                   |
| i_data              | In        | 8     | OV7670 D[7:0]                 |
| M_AXIS_VIDEO_TDATA  | Out       | 16    | Output Video Data             |
| M_AXIS_TVALID       | Out       | 1     | Output Valid                  |
| M_AXIS_VIDEO_TREADY | In        | 1     | Output Ready                  |
| M_AXIS_VIDEO_TUSER  | Out       | 1     | Output Video Start of Frame   |
| M_AXIS_VIDEO_TLAST  | Out       | 1     | Output Video End of Line      |