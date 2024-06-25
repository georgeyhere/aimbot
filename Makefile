SHELL := /bin/bash

############
# Tooling 
############
export VIVADO_PATH := /tools/Xilinx/Vivado/2024.1

############
# Config
############
export LIB_DIR    :=./lib
export PROTO_PATH :=$(shell pwd)
export PRJ_NAME   :=""
export PRJ_PART   :="xc7z020clg400-1"
export PRJ_SCRIPT := tpg_vdma_sys.tcl

############

vivado: setup init
	@echo "PROTO_PATH=$(PROTO_PATH)"
	@echo "PRJ_SCRIPT=$(PRJ_SCRIPT)"
	@cd lib
	@vivado -mode batch -source $(PROTO_PATH)/scripts/run_vivado.tcl -tclargs $(PROTO_PATH) $(PRJ_SCRIPT) $(PRJ_PART)

setup:
	source $(VIVADO_PATH)/settings64.sh

init:
	@if [ -d "$(LIB_DIR)" ]; then \
		echo "lib exists, not remaking it"; \
	else \
		echo "lib doesn't exist, creating it now."; \
		mkdir -p "$(LIB_DIR)"; \
	fi

clean:
	rm -rf lib
	rm -rf *.log
	rm -rf *.jou
