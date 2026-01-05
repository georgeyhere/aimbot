

# aimbot

This is a sort of monorepo containing various projects pertaining to CV on Zynq 7000 (specifically Digilent Zybo Z7-20).

## Dependencies
* Ubuntu 22.04 
* Vivado 2025.2
* Vitis 2025.2

## Cloning

This repository uses git submodules. 
Either clone with `--recursive`, or initialize submodules with:

```bash
git submodule update --init --recursive
```


## Projects in this Repo

### tpg_vdma
HW design + FW demonstrating Xilinx VDMA, VTC, and TPG IP usage with HDMI output via Digilent RGB2DVI IP.

Bare-metal and Petalinux apps planned.

## Run Flow

Setup the environment. You may need to edit it to point to your Vivado install path. 

This is required before proceeding with either of the steps below.

```bash
source ./setup.sh
```

### Running FW
To regenerate the Vitis workspace (BSP + applications) using the prebuilt `.xsa` and sources under `prj/tpg_vdma`:
```bash
make vitis_init PROJECT=tpg_vdma
```

The workspace will be created at `lib/tpg_vdma_vitis`.

To build application `app_tpg_vdma_demo`:
```bash
make vitis_build PROJECT=tpg_vdma APP=app_tpg_vdma_demo
```

### Regenerating HW design
To recreate Vivado project and regenerate XSA:
```bash
make project PROJECT=tpg_vdma
```

To recreate Vivado project without kicking synthesis / PnR runs:
```bash
make project PROJECT=tpg_vdma STOP_AFTER=xdc
```

To run Vivado flow in GUI mode, set GUI=1:
```bash
make project PROJECT=tpg_vdma STOP_AFTER=xdc GUI=1
```



