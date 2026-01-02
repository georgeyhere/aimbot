

# aimbot

This is a sort of monorepo containing various projects pertaining to CV on Zynq 7000 (specifically my Zybo Z7-20).

## Cloning

This repository uses git submodules. To clone with dependencies:

```bash
git clone --recursive <repository-url>
```

If you already cloned without `--recursive`, initialize submodules with:

```bash
git submodule update --init --recursive
```

## Dependencies
* Ubuntu 22.04 
* Vivado 2025.2
* Vitis 2025.2


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
The Vitis workspace (application and platform projects) are checked in under `prj/<prj_name>/vitis`.

It can be built and run directly without rerunning Vivado.

Vitis 2025.2 has Git integration. However, some files have absolute pathing that's not very friendly.  
The Vitis workspaces in this repo have those absolute paths scrubbed. To restore them based on your current repo path, 
use the following make target before opening the workspace in Vitis.
```bash
make vitis_setup
```

The absolute paths can be re-scrubbed with the following make target:
```bash
make vitis_clean
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



