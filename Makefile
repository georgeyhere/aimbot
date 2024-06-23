SHELL := /bin/bash

############
# Tooling 
############
VIVADO_PATH := /tools/Xilinx/Vivado/2024.1

############
# Config
############
LIB_DIR=./lib

PROTO_PATH :=$(shell pwd)
PRJ_NAME   :=""
PRJ_PART   :="xc7z020clg400-1"

BRD_PART   :="digilentinc.com:zybo-z7-20:part0:1.1"


############
export PROTO_PATH
export PRJ_NAME
export PRJ_PART
############

vivado: setup init
	@if [ -z "$(target)" ]; then \
		echo "Usage: make vivado target=<project script name>"; \
		exit 1; \
	fi
	@echo "PROTO_PATH=$(PROTO_PATH)"
	@export PRJ_SCRIPT=$(target)
	@cd lib
	@vivado -mode batch -source $(PROTO_PATH)/scripts/run_vivado.tcl

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
