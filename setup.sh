#!/bin/bash

#XILINX_VERSION="2024.1"
XILINX_VERSION="2025.2"

# Setup Xilinx tools
unset XILINX_VIVADO
unset XILINX_VITIS
if [ -z "$XILINX_VIVADO" ]; then
  if [ "$XILINX_VERSION" = "2025.2" ]; then
    VIVADO_SETTINGS="/tools/Xilinx/2025.2/Vivado/settings64.sh"
    VITIS_SETTINGS="/tools/Xilinx/2025.2/Vitis/settings64.sh"
  else
    VIVADO_SETTINGS="/tools/Xilinx/Vivado/$XILINX_VERSION/settings64.sh"
    VITIS_SETTINGS="/tools/Xilinx/Vitis/$XILINX_VERSION/settings64.sh"
  fi

  if [ -f "$VIVADO_SETTINGS" ]; then
    echo "Sourcing Vivado settings from $VIVADO_SETTINGS"
    source "$VIVADO_SETTINGS"
  else
    echo "Error: Vivado settings not found at $VIVADO_SETTINGS."
    echo "Please install Vivado or set XILINX_VIVADO manually."
    exit 1
  fi

  if [ -f "$VITIS_SETTINGS" ]; then
    echo "Sourcing Vitis settings from $VITIS_SETTINGS"
    source "$VITIS_SETTINGS"
  else
    echo "Warning: Vitis settings not found at $VITIS_SETTINGS."
    echo "Vitis tools may not be available."
  fi
fi
if [ -z "$XILINX_VIVADO" ]; then
  echo "Error: XILINX_VIVADO is not set."
  exit 1
fi

# Path setup
export WORKROOT=$(pwd)

# Toolchain Configuration
export XVLOG=$XILINX_VIVADO/bin/xvlog
export XELAB=$XILINX_VIVADO/bin/xelab
export XSIM=$XILINX_VIVADO/bin/xsim

export UNISIM_PATH=$XILINX_VIVADO/data/verilog/src/unisims

echo "####################################################"
echo "WORKROOT      : $WORKROOT"
echo ""
echo "XILINX_VIVADO : $XILINX_VIVADO"
echo "XILINX_VITIS  : $XILINX_VITIS"
echo "XVLOG         : $XVLOG"
echo "XELAB         : $XELAB"
echo "XSIM          : $XSIM"
echo "UNISIM_PATH   : $UNISIM_PATH"
echo "####################################################"

