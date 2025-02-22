.PHONY: default
default: sim_regr

.PHONY: help
help:
	@echo "-----------------------------------------------------------------------------"
	@echo "Syntax:"
	@echo "make [make target] [options]"
	@echo "-----------------------------------------------------------------------------"
	@echo "Targets:"
	@echo "    sim_regr XSIM_OPTS=[opts]:                 Run unit test regressions."
	@echo "    sim      TB_NAME=[name] XSIM_OPTS=[opts]:  Run a specific unit test."
	@echo "    syn:                                       Run synthesis."
	@echo "-----------------------------------------------------------------------------"
	@echo "Options:"
	@echo "    SIMULATOR=[xilinx, siemens, vcs]: Select simulation toolchain."
	@echo "-----------------------------------------------------------------------------"

#------------#
# Path setup #
#------------#
PRJ_ROOT=$(shell pwd)
export PRJ_ROOT
PRJ_SCRIPTS=$(PRJ_ROOT)/scripts
export PRJ_SCRIPTS
UNISIM_PATH=$(XILINX_VIVADO)/data/verilog/src/unisims

#-------------------------#
# Toolchain Configuration #
#-------------------------#

# Simulator selection:
# [xilinx, siemens, vcs]
SIMULATOR="siemens"
SIM_GUI=0
export SIM_GUI


#-----------#
# VCS Setup #
#-----------#
DW_HOME=
VCS=vcs
VCS_OPTS=
SIMV_OPTS=

#---------------#
# Siemens Setup #
#---------------#
SIEMENS_PATH=~/opt/intel/modelsim_ase
VLOG=$(SIEMENS_PATH)/bin/vlog
VOPT=$(SIEMENS_PATH)/bin/vopt
VSIM=$(SIEMENS_PATH)/bin/vsim

VLOG_OPTS=-sv 
VSIM_OPTS += $(if $(filter 0, $(SIM_GUI)), -c)

#------------------------------------------#
# Vivado Simulation Setup                  #
# SystemVerilog VCD dumping not supported! #
#------------------------------------------#
XVLOG=$(XILINX_VIVADO)/bin/xvlog
XELAB=$(XILINX_VIVADO)/bin/xelab
XSIM=$(XILINX_VIVADO)/bin/xsim

XVLOG_OPTS=-sv -log xvlog.log
XELAB_OPTS=
XSIM_OPTS=

# 
XSIM_OPTS += $(if $(filter 1, $(SIM_GUI)), -gui)

#-------------------#
# Utility Functions #
#-------------------#
MKDIR = bash $(PRJ_SCRIPTS)/make_utils.sh util_mkdir


#--------------------#
# Environment Checks #
#--------------------#
setup:
	@echo "PRJ_ROOT:      $(PRJ_ROOT)"
	@echo "XILINX_VIVADO: $(XILINX_VIVADO)"
	@echo "SIMULATOR:     $(SIMULATOR)"
	@if [ -z "$(XILINX_VIVADO)" ]; then \
		echo "\nError- Environment variable XILINX_VIVADO not found"; \
	fi 
	$(MKDIR) foo

#-----------------------#
# TPG VDMA Demo Project #
#-----------------------#
setup_tpg_vdma: setup
	$(call make-dir)




.PHONY: clean
clean:
	rm -rf xvlog*
	rm -rf xelab*
	rm -rf xsim*
	rm -rf *.log
	rm -rf *wdb
	rm -rf transcript
	rm -rf work


