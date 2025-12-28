#!/bin/bash

#VIVADO_VERSION="2024.1"
VIVADO_VERSION="2025.2"

# Setup Vivado
unset XILINX_VIVADO
if [ -z "$XILINX_VIVADO" ]; then
  if [ "$VIVADO_VERSION" = "2025.2" ]; then
    SETTINGS_PATH="/tools/Xilinx/2025.2/Vivado/settings64.sh"
  else
    SETTINGS_PATH="/tools/Xilinx/Vivado/$VIVADO_VERSION/settings64.sh"
  fi

  if [ -f "$SETTINGS_PATH" ]; then
    echo "Sourcing Vivado settings from $SETTINGS_PATH"
    source "$SETTINGS_PATH"
  else
    echo "Error: Vivado settings not found at $SETTINGS_PATH."
    echo "Please install Vivado or set XILINX_VIVADO manually."
    exit 1
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
echo "XVLOG         : $XVLOG"
echo "XELAB         : $XELAB"
echo "XSIM          : $XSIM"
echo "UNISIM_PATH   : $UNISIM_PATH"
echo "####################################################"

